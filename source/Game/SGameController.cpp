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
        
    _hunterAdded = false;
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

int cnt = 0;
bool blocked = false;

void SGameController::update(float dt) {
    if(_gameStatus == 0){
        bool canSwitch = true;
        bool didSwitch = false;
        
        Vec2 minimapOffset = Vec2(_scene->getSize().width, 0) - (_miniMap == nullptr ? Vec2::ZERO : Vec2(_miniMap->getSize().width, 0)) - Vec2(60, -30);
        
        if (!_levelLoaded) {
            checkLevelLoaded();
            _portraits->setIndex(4);
            std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
            ->setZoom(1);
        }
        auto inputController = InputController::getInstance();
        inputController->update(dt);
        inputController->readInput();
        if (inputController->didPressReset()) {
            reset();
            CULog("Reset!");
        }
        
        bool start = inputController->didPress();
        bool release = inputController->didRelease();
        Vec2 cameraPos = _scene->getCamera()->screenToWorldCoords(inputController->getTouchPos());
        
        //logic for door lock
        if ((inputController->isTouchDown() || _spirit.getModel()->isOnLock) && _spirit.getModel()->doors >= 0 && !blocked){
            if(_spirit.getModel()->isOnLock || _spirit.touchInLockBound(cameraPos)){
                canSwitch = false;
                _spirit.getModel()->setLockState(true);
                bool isLocked = false;
                _spirit.updateMovingLock(cameraPos);
                for (int i=0; i<_doors.size();i++){
                    if(_doors.at(i)->update(start,release, cameraPos)){
                        isLocked = true;
                    }
                }
                if (release){
                    _spirit.getModel()->setLockState(false);
                    if (isLocked){
                        _spirit.removeLastLock(_scene);
                    }
                }
            }
        } else if (blocked&&_spirit.getModel()->isOnLock){
            _spirit.getModel()->setLockState(false);
            for (int i=0; i<_doors.size();i++){
                _doors.at(i)->resetToUnlock();
            }
        }
        
        //logic for trap placement
        if ((inputController->isTouchDown() || _spirit.getModel()->isOnTrap) && _spirit.getModel()->traps >= 0 && !_spirit.getModel()->isOnLock && !blocked){
            if(_spirit.getModel()->isOnTrap || _spirit.touchInTrapBound(cameraPos)){
                canSwitch = false;
                _spirit.getModel()->setTrapState(true);
                _spirit.getModel()->setLastTrapPos(cameraPos);
                _spirit.updateMovingTrap(cameraPos);

            }
            if ( _spirit.getModel()->isOnTrap && release){
                _spirit.getModel()->setTrapState(false);
                
                // A trap has been placed
                if (_spirit.placeTrap(_tilemap, _spirit.getModel()->lastTrapPos)){
                    _spirit.removeLastTrapBtn(_scene);
                }
            }
        } else if (blocked&&_spirit.getModel()->isOnTrap){
            _spirit.getModel()->setTrapState(false);
        }
        
        //logic for camera switching & battery
        if (inputController->isTouchDown()) {
            auto screenPos = inputController->getTouchPos();
            
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
            if (inBound(screenPos) && canSwitch) {
                auto miniMapPos =
                Vec2(screenPos.x - _miniMap->getSize().width / 2 * getZoom() -
                     minimapOffset.x,
                     _miniMap->getSize().height / 2 * getZoom() +
                     minimapOffset.y - screenPos.y);
                auto mapPos = miniMapPos / _miniMap->getScale() / getZoom();
                int idx = _portraits->getNearest(mapPos);
                if (_spirit.isSwitchable() && _portraits->getIndex() != idx) {
                    _portraits->setIndex(idx);
                    std::dynamic_pointer_cast<OrthographicCamera>(
                                                                  _scene->getCamera())
                    ->setZoom(1);
                    _spirit.resetCameraCool();
                    didSwitch = true;
                } else if (!_spirit.isSwitchable() &&
                           _portraits->getIndex() != idx) {
                    _portraits->resetScale();
                }
            }
            
        }
        if (!didSwitch) {
            _spirit.decreaseCameraCool();
        }
        
        Vec3 offset = Vec3(_assets->get<Texture>("map")->getSize()/2);
        _scene->getCamera()->setPosition(
                                         _portraits->getPosition(_portraits->getIndex()) + offset);
        
        // Draw battery
        _portraits->updateBattery();
        _portraits->updateBatteryNode(_scene, _miniMap->getSize().width);
        
        // Black screen
        if (!_portraits->getCurState() && _portraits->getPrevState()) {
            CULog("Black screen drawn!");
            // Redraw doors
            _portraits->addBlock(_scene);
            _portraits->refreshBatteryNodes(_scene);
            blocked = true;
        } else if (_portraits->getCurState() && !_portraits->getPrevState()) {
            // Redraw doors
            _portraits->removeBlock(_scene);
            blocked = false;
        }
        
        _portraits->setPrevState(_portraits->getCurState());
        // detect if a trap on the map has been removed, add a new trap button to the scene
        if(_spirit.update()){
            _spirit.addNewTrapBtn(_scene);
            
        }
        
        // Draw minimap
        _miniMap->setPosition(_scene->getCamera()->screenToWorldCoords(
                                                                       _miniMap->getSize() / 2 * getZoom() + minimapOffset));
        _scene->removeChild(_miniMap);
        _scene->addChild(_miniMap);
        
        // Draw locks
        _spirit.updateLocksPos(_scene);
        _spirit.updateTrapBtnsPos(_scene);
        
        if(!blocked){
            _scene->getCamera()->update();
        }
        
        if (_network) {
            _network->receive([this](const std::string source,
                                     const std::vector<std::byte>& data) {
                processData(source,data);
            });
            checkConnection();
            
            if (_spirit.getTrapAdded()) {
                std::vector<float> pos = std::vector<float>();
                pos.push_back(1);
                pos.push_back(_spirit.getLastTrapPos().x);
                pos.push_back(_spirit.getLastTrapPos().y);
                transmitTrap(pos);
                _spirit.setTrapAdded(false);
            }
        }
    }else if(_gameStatus == 1){
        // Spirit won
        _endScene->update();
    }else{
        // Spirit lost
    }
//    cnt++;
//    if(cnt == 100) {
//        _gameStatus = 1;
//        _endScene = make_shared<EndScene>(_assets, true);
//        _endScene->addChildTo(_scene);
//    }
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
        _tilemap->updateTileSize(Size(128, 128));
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
        }
        _portraits->setMaxbattery(_level->getBattery());
        
        _portraits->initializeSheets(_assets->get<Texture>("greenBattery"),
                                     _assets->get<Texture>("redBattery"),
                                     _assets->get<Texture>("noBattery"));
        _portraits->initializeBatteryNodes(_scene);
        _spirit.getView()->addLocksTo(_scene);
        _spirit.getView()->addTrapButtonsTo(_scene);
        
        initDoors();
    }
}
    
    void SGameController::generateLevel() {
        _tilemap->updateDimensions(_level->getDimensions());
    }

void SGameController::initDoors(){
    std::vector<std::pair<Vec2, int>> doors = _level->getDoors();
    for (int i=0; i<doors.size(); i++){
        _doors.emplace_back(std::make_shared<DoorController>(_assets, doors[i].first, doors[i].second, 1));
        _doors.at(i)->addChildTo(_scene);
    }
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
    if (source != _network->getHost()) {
        _deserializer->receive(data);
        std::vector<float> mes = std::get<std::vector<float>>(_deserializer->read());
        if (!_hunterAdded) {
            _spirit.addHunter(Vec2(mes[1], mes[2]));
            _spirit.moveHunter(Vec2(400, 400));
            _hunterAdded = true;
        } else if (mes[0] == 0) {
            _spirit.moveHunter(Vec2(mes[1], mes[2]));
        }
        
        // Treasure picked up alert (not sure if position will be used)
        //_spirit.treasureAlert(pos);
        
        // Win alert for spirit
        // _gameStatus = 1;
        
        // Lose alert for spirit
        // _gameStatus = -1;
        
//        CULog("%f", mes[0]);
        _deserializer->reset();
    }
}

void SGameController::transmitTrap(std::vector<float> pos) {
    _serializer->writeFloatVector(pos);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}
