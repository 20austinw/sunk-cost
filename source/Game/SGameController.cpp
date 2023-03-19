//
//  SGameController.cpp
//  Sunk Cost
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors of Referenced File: Walker White and Gonzalo Gonzalez
//  Version: 2/22/23
//
// This is in the same directory
#include "SGameController.hpp"
#include "LevelConstants.h"

using namespace cugl;
using namespace cugl::net;
using namespace std;

#pragma mark Main Methods
/**
 * Creates the game controller.
 *
 * This constructor will procedurally generate a tilemap immediately
 * on creation.
 *
 * @param displaySize   The display size of the game window
 * @param randoms        Reference to the random number generator
 */
SGameController::SGameController(
    const Size displaySize, const std::shared_ptr<cugl::AssetManager>& assets)
    : _assets(assets) {
    /// Initialize the tilemap and add it to the scene
    _scene = cugl::Scene2::alloc(displaySize);
    std::shared_ptr<scene2::PolygonNode> background =
        scene2::PolygonNode::allocWithPoly(cugl::Rect(0, 0, 20000, 20000));
    background->setColor(Color4::BLACK);
    background->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setPosition(-1 * Size(9000, 9000) / 2);
    _scene->addChild(background);
    _tilemap = std::make_shared<TilemapController>();
    _tilemap->addChildTo(_scene);
    // Initialize PortraitSetController
    _portraits = std::make_shared<PortraitSetController>(_assets, _scene, 0,
                                                         displaySize);
    
    // Initialize HunterController
    //        _hunter.updatePosition(_level->getPlayerPosition());

    // Initialize SpiritController
    _spirit = SpiritController(_assets, _scene, _portraits, _scene->getSize());
    _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }
    
    _serializer = NetcodeSerializer::alloc();
    _deserializer = NetcodeDeserializer::alloc();
    _status = Status::START;
}
    
#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void SGameController::reset() { CULog("reset"); }

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
float SGameController::getZoom() {
    return std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
        ->getZoom();
}
void SGameController::update(float dt) {
    bool canPlaceTrap = true;
    Vec2 minimapOffset = Vec2(20, 20);
    if (!_levelLoaded) {
        CULog("Level not loaded!");
        checkLevelLoaded();
        _portraits->setIndex(4);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
            ->setZoom(0.4);
    }
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    inputController->readInput();
    if (inputController->didPressReset()) {
        reset();
        CULog("Reset!");
    }

    if (inputController->isMouseClicked()) {
        auto screenPos = inputController->getLastMousePos();
        // Check if click is minimap
        auto inBound = [&](Vec2 pos) {
            if (pos.x >= minimapOffset.x && pos.y >= minimapOffset.y &&
                pos.x <=
                    _miniMap->getSize().width * getZoom() + minimapOffset.x &&
                pos.y <=
                    _miniMap->getSize().height * getZoom() + minimapOffset.y) {
                return true;
            }
            return false;
        };
        // Logic for switching cameras
        if (inBound(screenPos)) {
            canPlaceTrap = false;
            auto miniMapPos =
                Vec2(screenPos.x - _miniMap->getSize().width / 2 * getZoom() -
                         minimapOffset.x,
                     _miniMap->getSize().height / 2 * getZoom() +
                         minimapOffset.y - screenPos.y);
            CULog("%f, %f", miniMapPos.x, miniMapPos.y);
            auto mapPos = miniMapPos / _miniMap->getScale() / getZoom();
            int idx = _portraits->getNearest(mapPos);
            if (_spirit.isSwitchable() && _portraits->getIndex() != idx) {
                _portraits->setIndex(idx);
                std::dynamic_pointer_cast<OrthographicCamera>(
                    _scene->getCamera())
                    ->setZoom(0.4);
                _spirit.resetCameraCool();
            } else if (!_spirit.isSwitchable() &&
                       _portraits->getIndex() != idx) {
                _portraits->resetScale();
            }
        } else {
            _spirit.decreaseCameraCool();
        }
    } else {
        _spirit.decreaseCameraCool();
    }

    Vec3 offset = Vec3(_assets->get<Texture>("map")->getSize() / 2);
    _scene->getCamera()->setPosition(
        _portraits->getPosition(_portraits->getIndex()) + offset);
    _miniMap->setPosition(_scene->getCamera()->screenToWorldCoords(
        _miniMap->getSize() / 2 * getZoom() + minimapOffset));
    _scene->removeChild(_miniMap);
    _scene->addChild(_miniMap);
    
    _portraits->updateBattery();
    _portraits->updateBatteryNode(offset, _scene);
    _scene->getCamera()->update();


    if (!_portraits->getCurState() && _portraits->getPrevState()) {
        _portraits->addBlock(_scene);
        _portraits->refreshBatteryNodes(_scene);
    } else if (_portraits->getCurState() && !_portraits->getPrevState()) {
        _portraits->removeBlock(_scene);
    }
    _portraits->setPrevState(_portraits->getCurState());
    // CULog("%f, %f, %f", _scene->getCamera()->getPosition().x,
    // _scene->getCamera()->getPosition().y,
    // _scene->getCamera()->getPosition().z);

    // Will crash the program because the constructor doesn't set up the
    // model/view yet (delete this comment later)
    //    _hunter.update();
    _spirit.update(_tilemap, canPlaceTrap);
    // TODO: update direction index for portraits on spirit control
    //    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int index#>)

    if (_network) {
        _network->receive([this](const std::string source,
                                 const std::vector<std::byte>& data) {
            processData(source,data);
        });
        checkConnection();
    }
}

/**
 * Draws all this scene to the given SpriteBatch.
 *
 * The default implemen3tation of this method simply draws the scene graph
 * to the sprite batch.  By overriding it, you can do custom drawing
 * in its place.
 *
 * @param batch     The SpriteBatch to draw with.
 */
void SGameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    // CULog("Rendering!");
    _scene->render(batch);
    displayBattery(_portraits->getCurBattery(), _portraits->getCurState(),
                   batch);
}

void SGameController::displayBattery(
    float battery, bool state, std::shared_ptr<cugl::SpriteBatch>& batch) {
    //    CULog("%f", battery);
}

void SGameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }

    // Check to see if new level loaded yet
    if (!_levelLoaded && _assets->complete()) {
        _level = nullptr;

        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
        _level->setAssets(_assets);

        CULog("Loading level!");

        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::BLACK);
        _tilemap->updateTileSize(Size(256, 256));
        for (int i = 0; i < tiles.size() * tiles[0].size(); ++i) {
            int c = i % tiles[0].size();
            int r = i / tiles[0].size();
            if (tiles[r][c] == "black") {
                _tilemap->addTile(c, r, Color4::BLACK, false,
                                  _assets->get<Texture>("black"));
            } else if (tiles[r][c] == "green") {
                _tilemap->addTile(c, r, Color4::GREEN, true,
                                  _assets->get<Texture>("green"));
            } else if (tiles[r][c] == "door") {
                _tilemap->addDoor(c, r, _assets->get<Texture>("fulldoor"));
            }
        }

        _map =
            scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("map"));
        _map->setPolygon(Rect(0, 0, 4608, 4608));
        
        _scene->addChild(_map);
        _miniMap = scene2::PolygonNode::allocWithTexture(
            _assets->get<Texture>("minimap"));
        _miniMap->setScale(0.1);
        _scene->addChild(_miniMap);
        _tilemap->addDoorTo(_scene);
        for (int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits->addPortrait(i + 1, _level->getPortaits()[i].first,
                                    _level->getPortaits()[i].second,
                                    Vec3(0, 0, -1), Vec2::ZERO,
                                    _level->getBattery());
            
            _levelLoaded = true;
            _portraits->setMaxbattery(_level->getBattery());
            
            _portraits->initializeSheets(_assets->get<Texture>("greenBattery"),
                                         _assets->get<Texture>("redBattery"),
                                         _assets->get<Texture>("noBattery"));
            _portraits->initializeBatteryNodes(_scene);
        }
    }
}
    
    void SGameController::generateLevel() {
        _tilemap->updateDimensions(_level->getDimensions());
    }

bool SGameController::checkConnection() {
    // IMPLEMENT ME
    NetcodeConnection::State network_state = _network->getState();
    switch (network_state) {
        case NetcodeConnection::State::FAILED:
        case NetcodeConnection::State::DISCONNECTED:
            disconnect();
            _quit = true;
            return false;
            break;
        case NetcodeConnection::State::CONNECTED:
            break;
        default:
            break;
    }
    return true;
}

void SGameController::processData(const std::string source, const std::vector<std::byte> &data) {
    if (source == _network->getHost()) {
        
    }
}

void SGameController::transmitTrap(std::vector<float> pos) {
    _serializer->writeFloatVector(pos);
    _network->broadcast(_serializer->serialize());
}
