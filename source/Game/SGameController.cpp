//
//  SGameController.cpp
//  Sunk Cost
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors of Referenced File: Austin and Crystal
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

    _firstLayer = scene2::PolygonNode::alloc();
    _scene->addChild(_firstLayer);
    _secondLayer = scene2::PolygonNode::alloc();
    _scene->addChild(_secondLayer);
    _thirdLayer = scene2::PolygonNode::alloc();
    _scene->addChild(_thirdLayer);
    _fourthLayer = scene2::PolygonNode::alloc();
    _scene->addChild(_fourthLayer);
    _fifthLayer = scene2::PolygonNode::alloc();
    _scene->addChild(_fifthLayer);

    _background = scene2::PolygonNode::allocWithPoly(
        cugl::Rect(0, 0, _scene->getSize().width, _scene->getSize().height));
    _background->setColor(Color4::BLACK);
    _background->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _firstLayer->addChild(_background);

    _tilemap = std::make_shared<TilemapController>();
    _tilemap->addChildToNode(_firstLayer);

    _obstacleNode = scene2::PolygonNode::alloc();
    _secondLayer->addChild(_obstacleNode);

    // Initialize PortraitSetController
    _portraits = std::make_shared<PortraitSetController>(_assets, _scene, 0,
                                                         displaySize);

    // Initialize HunterController

    //        _hunter.updatePosition(_level->getPlayerPosition());

    // Initialize SpiritController
    _spirit = SpiritController(_assets, _scene, _portraits, _scene->getSize());
    _level = _assets->get<LevelModel>(LEVEL_FINAL_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }
    _blocked = false;
    _hunterAdded = false;
    _serializer = NetcodeSerializer::alloc();
    _deserializer = NetcodeDeserializer::alloc();
    _status = Status::START;
    _alertTimer = 0;
    _font = assets->get<Font>("gamefont");
    string minutes = std::to_string(_timeLeft / 60 / 60);
    string seconds = std::to_string(_timeLeft / 60 % 60);
    seconds = seconds.length() <= 1 ? "0" + seconds : seconds;
    _timerLabel = cugl::scene2::Label::allocWithText(
        Vec2(0, 0), "              ", _assets->get<Font>("gamefont"));
    _timerScale = _textHeight / _timerLabel->getSize().height * 1.5;
    _fifthLayer->addChild(_timerLabel);
    _endScene = std::make_shared<EndScene>(_scene, assets, true, true);

    _trapTriggered = false;
    _doorUnlocked = false;
    _treasureStolen = false;
    _trapPos = Vec2::ZERO;
    _selection = false;

    _alertLabel = cugl::scene2::Label::allocWithText(
        Vec2(0, displaySize.height / 2), "The treasure has been STOLEN",
        _assets->get<Font>("gamefont"));
    _alertLabel->setPosition(_scene->getCamera()->getPosition() +
                             Vec2(350, 350));
    _alertLabel->setForeground(cugl::Color4f::RED);

    _selectionPhase = false;
    _buttonHeight = 350;

    _viewButton = std::make_shared<Button>(_assets->get<Texture>("eye_button"),
                                           _scene, _selectionPhase, _portraits);
    _viewButton->setDefaultPosition(Vec2(_buttonHeight, _buttonHeight) / 2);
    _viewButton->setVisible(true);
    _viewButton->setInteractive(true);
    //    _viewButton->addChildTo(_scene);
    _viewButton->addChildToNode(_fourthLayer);

    while (!_levelLoaded) {
        checkLevelLoaded();
    }
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

void SGameController::update(float dt) {
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    inputController->readInput();

    // Disable indicators by default
    for (auto& indicator : _indicators) {
        indicator->setVisible(false);
    }

    if (_gameStatus == 0) {

        bool preSelection = _selection;
        // Not in selection phase
        if (!_viewButton->update()) {
            _selection = false;
            _spirit.getView()->setVisible(true);
            if (_viewButton->getCameraIndex() != -1) {
                _portraits->setIndex(_viewButton->getCameraIndex());
                _scene->getCamera()->setPosition(
                    _portraits->getPosition(_portraits->getIndex()));
                transmitActiveCamIndex(_portraits->getIndex());
                std::dynamic_pointer_cast<OrthographicCamera>(
                    _scene->getCamera())
                    ->setZoom(1.5);
            }
        }
        // In selection phase
        else {
            _selection = true;
            _spirit.getView()->setVisible(false);
            _scene->getCamera()->setPosition(_portraits->getPosition(0));
            std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
                ->setZoom(0.3);
            auto worldPos = _scene->getCamera()->screenToWorldCoords(
                inputController->getTouchPos());
            auto nearestCameraPos =
                _portraits->getPosition(_viewButton->getCameraIndex());
            if (worldPos.distance(nearestCameraPos) <= 400) {
                _indicators[_viewButton->getCameraIndex() - 1]->setVisible(
                    true);
            }
            CULog("%i", _viewButton->getCameraIndex());
        }

        if (preSelection != _selection) {
            // start select- remove hunter
            if (_selection) {
                for (int n = 0; n < _hunterNodes.size(); n++) {
                    _obstacleNode->removeChild(_hunterNodes.at(n));
                }
            }
            // end select - add hunter
            else {
                for (int n = 0; n < _hunterNodes.size(); n++) {
                    _obstacleNode->addChild(_hunterNodes.at(n));
                }
                _viewButton->setCooldown(_viewButton->getMaxCool());
                _viewButton->setButtonFrame(0);
            }
        }

        _scene->getCamera()->update();
        // Draw background
        _background->setScale(1 / getZoom());
        _background->setPosition(_scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height)));
        sortNodes();

        AudioEngine::get()->play("theme", _theme, false, 0.5, false);
        bool canSwitch = true;
        bool didSwitch = false;

        if (_treasureStolen) {
            _alertLabel->setText("The treasure has been stolen");
        }
        if (_treasureStolen && _alertTimer < 6) {
            AudioEngine::get()->play("treasureSound", _treasureSound, false,
                                     0.8, true);
        }
        if (_alertTimer == 0 && _treasureStolen) {
            _fifthLayer->addChild(_alertLabel);
            _alertTimer++;
        } else if (_alertTimer > 0 && _alertTimer != 300) {
            _alertTimer++;
        }

        if (_alertTimer == 300) {
            _alertTimer = 0;
            _treasureStolen = false;
            _fifthLayer->removeChild(_alertLabel);
        }

        if (inputController->didPressReset()) {
            reset();
        }

        bool start = inputController->didPress();
        bool release = inputController->didRelease();
        Vec2 cameraPos = _scene->getCamera()->screenToWorldCoords(
            inputController->getTouchPos());

        // logic for door lock
        if ((inputController->isTouchDown() || _spirit.getModel()->isOnLock) &&
            _spirit.getModel()->doors >= 0 && !_blocked &&
            !_spirit.getModel()->isOnTrap && !_spirit.getModel()->isOnKill) {
            if (_spirit.getModel()->isOnLock ||
                _spirit.touchInLockBound(cameraPos)) {
                canSwitch = false;
                if (!_spirit.getModel()->isOnLock) {
                    _spirit.getView()->removeLastLockExtraTo(_fourthLayer);
                    _spirit.getModel()->setLockState(true);
                }
                bool isLocked = false;
                _spirit.updateMovingLock(cameraPos);
                for (int i = 0; i < _doors.size(); i++) {
                    if (_doors.at(i)->update(start, release, cameraPos)) {
                        isLocked = true;
                        transmitLockedDoor(i);
                    }
                }
                if (release) {
                    _spirit.getModel()->setLockState(false);
                    if (isLocked) {
                        _spirit.removeLastLock(_fourthLayer);

                    } else {
                        _spirit.getView()->addLastLockExtraTo(_fourthLayer);
                    }
                }
            }
        } else if (_blocked && _spirit.getModel()->isOnLock) {
            _spirit.getModel()->setLockState(false);
            for (int i = 0; i < _doors.size(); i++) {
                _doors.at(i)->resetToUnlock();
            }
        }

        // logic for trap placement
        if ((inputController->isTouchDown() || _spirit.getModel()->isOnTrap) &&
            _spirit.getModel()->traps >= 0 && !_spirit.getModel()->isOnLock &&
            !_blocked && !_spirit.getModel()->isOnKill) {
            if (_spirit.getModel()->isOnTrap ||
                _spirit.touchInTrapBound(cameraPos)) {
                canSwitch = false;
                if (!_spirit.getModel()->isOnTrap) {
                    _spirit.getView()->removeLastTrapExtraTo(_fourthLayer);

                    _spirit.getModel()->setTrapState(true);
                }
                _spirit.getModel()->setLastTrapPos(cameraPos);
                _spirit.updateMovingTrap(cameraPos);
            }
            if (_spirit.getModel()->isOnTrap && release) {
                _spirit.getModel()->setTrapState(false);

                // A trap has been placed
                if (_spirit.placeTrap(_tilemap, _spirit.getModel()->lastTrapPos,
                                      _secondLayer)) {
                    _spirit.removeLastTrapBtn(_fourthLayer);
                } else {
                    _spirit.getView()->addLastTrapExtraTo(_fourthLayer);
                }
            }
        } else if (_blocked && _spirit.getModel()->isOnTrap) {
            _spirit.getModel()->setTrapState(false);
        }

        // logic for killing hunter
        if (_spirit.getModel()->isKilling()) {
            for (int i = 0; i < _hunterNodes.size(); i++) {
                _obstacleNode->removeChild(_hunterNodes.at(i));
            }
            _spirit.getModel()->updateOnKill(_hunterNodes);
            for (int i = 0; i < _hunterNodes.size(); i++) {
                _obstacleNode->addChild(_hunterNodes.at(i));
            }
            if (!_spirit.getModel()->isKilling() &&
                _spirit.getModel()->health == 0) {
                // hunter has been killed, end
                _gameStatus = 1;
                _endScene =
                    std::make_shared<EndScene>(_scene, _assets, true, true);
                _endScene->addChildTo(_scene);
            }
        }

        if (!_spirit.getModel()->isKilling() &&
            (inputController->isTouchDown() || _spirit.getModel()->isOnKill) &&
            _spirit.getModel()->isKillable() && !_spirit.getModel()->isOnLock &&
            !_spirit.getModel()->isOnTrap && !_blocked) {
            canSwitch = false;
            if (_spirit.getModel()->isOnKill ||
                _spirit.touchInKillBound(cameraPos)) {
                if (!_spirit.getModel()->isOnKill) {
                    _spirit.getModel()->setKillState(true);
                }
                _spirit.getView()->updateKillInProgress(cameraPos);
            }
            if (_spirit.getModel()->isOnKill && release) {
                _spirit.getModel()->setKillState(false);

                if (_spirit.getModel()->hunterAdded &&
                    _spirit.hunterInBound(cameraPos)) {
                    // TODO: networking; transmit to hunter for losing 1 health
                    transmitKill();
                    _spirit.getModel()->setHealth(_spirit.getModel()->health -
                                                  1);

                    for (int i = 0; i < _hunterNodes.size(); i++) {
                        _obstacleNode->removeChild(_hunterNodes.at(i));
                    }

                    _spirit.getModel()->updateHeart(_hunterNodes);

                    for (int i = 0; i < _hunterNodes.size(); i++) {
                        _obstacleNode->addChild(_hunterNodes.at(i));
                    }

                    _spirit.getModel()->setKillCooldown(
                        _spirit.getModel()->getMaxKillCool());
                    _spirit.getView()->setKillFrame(0);
                }
            }
        } else if (_blocked && _spirit.getModel()->isOnKill) {
            _spirit.getModel()->setKillState(false);
        }

        // update block screen
        if (_blocked) {
            _portraits->removeBlock(_thirdLayer);
            _blocked = false;
        }
        if (!_portraits->getCurState() && !_selection) {
            _portraits->addBlock(_thirdLayer);
            _blocked = true;
        }

        if (_doorUnlocked && _doorToUnlock != -1) {
            if (_doors.at(_doorToUnlock)->isLocked()) {
                _doors.at(_doorToUnlock)->resetHunterUnlock();
                _spirit.addNewLock(_fourthLayer);
                _doorUnlocked = false;
                _doorToUnlock = -1;
            }
        }

        // detect if a trap or door on the map has been removed, add a new trap
        // button to the scene
        int result = _spirit.update(_trapTriggered, _trapPos, _secondLayer);
        if (_trapTriggered) {
            _trapTriggered = false;
            _trapPos = Vec2::ZERO;
        }
        if (result == 1) {
            _spirit.addNewTrapBtn(_fourthLayer);
        } else if (result == 2) {
            _spirit.addNewTrapBtn(_fourthLayer);
            _spirit.addNewTrapBtn(_fourthLayer);
        }

        if (_network) {
            _network->receive([this](const std::string source,
                                     const std::vector<std::byte>& data) {
                processData(source, data);
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

        _spirit.updateLocksPos(_selection);
        _spirit.updateTrapBtnsPos(_selection);
        _spirit.updateKillBtnsPos(_selection);
        _spirit.updateKillFrame();

        if (!didSwitch) {
            _spirit.decreaseCameraCool();
        }

        // Draw battery (has to come after the minimap update)
        _portraits->updateBattery(_selection);

        _portraits->updateBatteryNode(_fifthLayer, 50, _selection);

        // Draw timer and alert labels
        string minutes = std::to_string(_timeLeft / 60 / 60);
        string seconds = std::to_string(_timeLeft / 60 % 60);

        if (_timeLeft / 60 / 60 == 0) {
            AudioEngine::get()->play("tension", _tension, false, 0.5, false);
        }

        seconds = seconds.length() <= 1 ? "0" + seconds : seconds;
        _timerLabel->setText(minutes + ":" + seconds);
        _timerLabel->setScale(_timerScale / getZoom());
        float vPos = _scene->getSize().height - 20 -
                     _timerLabel->getSize().height * getZoom() / 2;
        float hPos = _scene->getSize().width / 2 -
                     _timerLabel->getSize().width * getZoom() / 2;
        _timerLabel->setPosition(
            _scene->getCamera()->screenToWorldCoords(Vec2(hPos, vPos)));
        vPos = _scene->getSize().height / 2 -
               _alertLabel->getSize().height * getZoom() / 2;
        hPos = _scene->getSize().width / 2 -
               _alertLabel->getSize().width * getZoom() / 2;
        _alertLabel->setPosition(
            _scene->getCamera()->screenToWorldCoords(Vec2(hPos, vPos)));
        _timerLabel->setForeground(cugl::Color4::WHITE);
        if (_timeLeft <= 0) {
            _gameStatus = 1;
            transmitSpiritWin();
            _endScene = std::make_shared<EndScene>(_scene, _assets, true, true);
            _endScene->addChildTo(_scene);
        }
        _scene->getCamera()->update();
    } else if (_gameStatus == -1) {
        if (!_endScene->isAdded()) {
            _endScene =
                std::make_shared<EndScene>(_scene, _assets, true, false);
            _endScene->addChildTo(_scene);
            _endScene->setAdded(true);
        } else if (_endScene->sUpdate()) {
            CULog("Switch to reset screen!");
            AudioEngine::get()->clear("tension", 1);
            AudioEngine::get()->clear("theme", 1);
            _status = ABORT;
        }
    } else if (_gameStatus == 1) {
        if (!_endScene->isAdded()) {
            _endScene = std::make_shared<EndScene>(_scene, _assets, true, true);
            _endScene->addChildTo(_scene);
            _endScene->setAdded(true);
        } else if (_endScene->sUpdate()) {
            CULog("Switch to reset screen!");
            AudioEngine::get()->clear("tension", 1);
            AudioEngine::get()->clear("theme", 1);
            _status = ABORT;
        }
    }
    _timeLeft--;
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
    if (!_levelLoaded) {
        return;
    }
    _scene->render(batch);
}

void SGameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_FINAL_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }

    // Check to see if new level loaded yet
    if (!_levelLoaded && _assets->complete()) {
        _level = nullptr;

        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_FINAL_KEY);
        _level->setAssets(_assets);

        // sounds
        _theme = _assets->get<Sound>("theme");
        _tension = _assets->get<Sound>("tension");
        _trapSound = _assets->get<Sound>("trapSound");
        _treasureSound = _assets->get<Sound>("treasureSound");

        int width = _level->getDimensions().x;
        int height = _level->getDimensions().y;

        _tilemap->updatePosition(_scene->getSize() / 2);
        _tilemap->updateDimensions(_level->getDimensions());
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(_level->getTileSize());

        std::vector<int> tiles = _level->getTileTextures();
        for (int i = 0; i < tiles.size(); i++) {
            int c = i % width;
            int r = i / width;
            int type = tiles[i];
            addFloorTile(type, c, height - 1 - r);
        }

        std::vector<std::vector<int>> details = _level->getDetails();

        for (int n = 0; n < details.size(); n++) {
            for (int m = 0; m < details.at(n).size(); m++) {
                int type = details[n][m];
                if (!(type == 0 || type >= 641 || type == 129 + 16 ||
                      type == 129 + 24)) {
                    int c = m % width;
                    int r = m / width;
                    addDetails(type, c, height - 1 - r);
                }
            }
        }

        //        std::vector<std::vector<int>> walls =
        //        _level->getWallTextures(); height = walls[0].size(); width =
        //        walls.size(); for (int i = 0; i < walls.size() *
        //        walls[0].size(); ++i) {
        //            int c = i % walls[0].size();
        //            int r = i / walls[0].size();
        //            int type = walls[r][c];
        //            addWallTile(type, c, width - 1 - r);
        //        }
        //
        //        walls = _level->getWallUpperTextures();
        //        height = walls[0].size();
        //        width = walls.size();
        //        for (int i = 0; i < height * width; ++i) {
        //            int c = i % height;
        //            int r = i / height;
        //            int type = walls[r][c];
        //            addWallUpper(type, c, width - 1 - r);
        //        }
        //
        //        walls = _level->getWallGrimeTextures();
        //        height = walls[0].size();
        //        width = walls.size();
        //        for (int i = 0; i < height * width; ++i) {
        //            int c = i % height;
        //            int r = i / height;
        //            int type = walls[r][c];
        //            addWallGrime(type, c, width - 1 - r);
        //        }
        //
        //        walls = _level->getWallLowerTextures();
        //        height = walls[0].size();
        //        width = walls.size();
        //        for (int i = 0; i < height * width; ++i) {
        //            int c = i % height;
        //            int r = i / height;
        //            int type = walls[r][c];
        //            addWallLower(type, c, width - 1 - r);
        //        }
        //
        //        walls = _level->getFurnitureTextures();
        //        height = walls[0].size();
        //        width = walls.size();
        //        for (int i = 0; i < height * width; ++i) {
        //            int c = i % height;
        //            int r = i / height;
        //            int type = walls[r][c];
        //            addFurnitures(type, c, width - 1 - r);
        //        }

        std::sort(_obstacles.begin(), _obstacles.end(),
                  [](std::shared_ptr<TileController>& a,
                     std::shared_ptr<TileController>& b) {
                      return a->getPosition().x < b->getPosition().x;
                  });

        std::vector<std::shared_ptr<TileController>> tmp;
        tmp.emplace_back(_obstacles.at(0));
        for (int i = 1; i < _obstacles.size(); i++) {
            if (_obstacles.at(i)->getPosition().x !=
                _obstacles.at(i - 1)->getPosition().x) {
                _sortedObstacles.emplace_back(tmp);
                tmp.clear();
            }
            tmp.emplace_back(_obstacles.at(i));
        }
        _obstacles.clear();

        for (int i = 0; i < _sortedObstacles.size(); i++) {
            std::sort(_sortedObstacles.at(i).begin(),
                      _sortedObstacles.at(i).end(),
                      [](std::shared_ptr<TileController>& a,
                         std::shared_ptr<TileController>& b) {
                          return a->getYPos() > b->getYPos();
                      });
        }

        for (int i = 0; i < _holes.size(); i++) {
            _holes[i]->removeChildFrom(_obstacleNode);
            _holes[i]->addChildTo(_obstacleNode);
        }

        for (int i = 0; i < _carpets.size(); i++) {
            _carpets[i]->removeChildFrom(_obstacleNode);
            _carpets[i]->addChildTo(_obstacleNode);
        }

        for (int n = 0; n < _sortedObstacles.size(); n++) {
            for (int m = 0; m < _sortedObstacles.at(n).size(); m++) {
                if (_sortedObstacles[n][m]->isObstacle()) {
                    _sortedObstacles[n][m]->removeChildFrom(_obstacleNode);
                    _sortedObstacles[n][m]->addChildTo(_obstacleNode);
                }
            }
        }

        for (int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits->addPortrait(_portraitNodes, i,
                                    _level->getPortaits()[i][0],
                                    _level->getPortaits()[i][1], Vec3(0, 0, -1),
                                    Vec2::ZERO, false, _level->getBattery());

            // Add camera selection indicator
            if (i <= 0)
                continue;
            auto indicator = cugl::scene2::PolygonNode::allocWithTexture(
                _assets->get<Texture>("indicator" + to_string(i)));
            indicator->setPosition(_level->getPortaits()[i][2]);
            indicator->setScale(2);
            indicator->setVisible(false);
            _fifthLayer->addChild(indicator);
            _indicators.emplace_back(indicator);
        }
        _portraits->setMaxbattery(_level->getBattery());

        _portraits->initializeSheets(_assets->get<Texture>("greenBattery"),
                                     _assets->get<Texture>("redBattery"),
                                     _assets->get<Texture>("noBattery"));
        _portraits->initializeBatteryNodes(_fifthLayer);

        _spirit.getView()->addLocksTo(_fourthLayer);
        _spirit.getView()->addTrapButtonsTo(_fourthLayer);
        _spirit.getView()->addKillButtonTo(_fourthLayer);

        initDoors();

        std::sort(_doorNodes.begin(), _doorNodes.end(),
                  [](std::shared_ptr<scene2::PolygonNode>& a,
                     std::shared_ptr<scene2::PolygonNode>& b) {
                      return a->getPositionY() < b->getPositionY();
                  });

        std::sort(_portraitNodes.begin(), _portraitNodes.end(),
                  [](std::shared_ptr<scene2::PolygonNode>& a,
                     std::shared_ptr<scene2::PolygonNode>& b) {
                      return a->getPositionY() < b->getPositionY();
                  });

        for (int i = 0; i < _doorNodes.size(); i++) {
            _obstacleNode->addChild(_doorNodes.at(i));
        }

        for (int i = 0; i < _portraitNodes.size(); i++) {
            _obstacleNode->addChild(_portraitNodes.at(i));
        }

        _portraits->setIndex(4);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
            ->setZoom(0.85);
        _viewButton->setCameraIndex(4);
        _spirit.updateLocksPos(false);
        _spirit.updateTrapBtnsPos(false);
        _spirit.updateKillBtnsPos(false);
        _levelLoaded = true;
    }
}

void SGameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
}

void SGameController::initDoors() {
    std::vector<std::pair<Vec2, int>> doors = _level->getDoors();
    for (int i = 0; i < doors.size(); i++) {
        _doors.emplace_back(std::make_shared<DoorController>(
            _assets, doors[i].first, doors[i].second, 1));
        _doors.at(i)->addChildToVector(_doorNodes);
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

void SGameController::processData(const std::string source,
                                  const std::vector<std::byte>& data) {
    if (source != _network->getHost()) {
        _deserializer->receive(data);
        std::vector<float> mes =
            std::get<std::vector<float>>(_deserializer->read());
        if (mes[0] == 0 && !_hunterAdded) {
            _spirit.addHunter(Vec2(mes[1], mes[2]), _hunterNodes);
            for (int i = 0; i < _hunterNodes.size(); i++) {
                _obstacleNode->addChild(_hunterNodes.at(i));
            }
            _spirit.moveHunter(Vec2(400, 400));
            _hunterAdded = true;
        } else if (mes[0] == 0) {
            _spirit.moveHunter(Vec2(mes[1], mes[2]));
            _hunterXPos = mes[1];
            _hunterYPos = mes[2];
        }

        // lol i'll work out more coherent codes for each message later oops
        if (mes[0] == 4) {
            // Treasure picked up alert (not sure if position will be used)
            // idt position will be used ?
            _treasureStolen = true;
            //_spirit.treasureAlert(pos);
        }

        if (mes[0] == 6) {
            // handle index of door for unlocking
            _doorUnlocked = true;
            _doorToUnlock = static_cast<int>(mes[1]);
        }

        if (mes[0] == 7) {
            if (_neverPlayed) {
                AudioEngine::get()->play("trapSound", _trapSound, false, 0.8,
                                         true);
                _neverPlayed = false;
            }
            _trapTriggered = true;
            _trapPos = Vec2(mes[1], mes[2]);
        }

        // Win alert for spirit
        if (mes[0] == 10) {
            _gameStatus = 1;
        }

        // Lose alert for spirit
        if (mes[0] == 8) {
            _gameStatus = -1;
        }

        //        CULog("%f", mes[0]);
        _deserializer->reset();
    }
}

void SGameController::transmitTrap(std::vector<float> pos) {
    _serializer->writeFloatVector(pos);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::transmitActiveCamIndex(int i) {
    std::vector<float> idx = std::vector<float>();
    idx.push_back(3);
    idx.push_back(i);
    _serializer->writeFloatVector(idx);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::transmitLockedDoor(int i) {
    std::vector<float> idx = std::vector<float>();
    idx.push_back(5);
    idx.push_back(i);
    _serializer->writeFloatVector(idx);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::transmitKill() {
    std::vector<float> idx = std::vector<float>();
    idx.push_back(9);
    _serializer->writeFloatVector(idx);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::transmitSpiritWin() {
    std::vector<float> idx = std::vector<float>();
    idx.push_back(10);
    _serializer->writeFloatVector(idx);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::transmitHunterWin() {
    std::vector<float> idx = std::vector<float>();
    idx.push_back(8);
    _serializer->writeFloatVector(idx);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void SGameController::addFloorTile(int type, int c, int r) {
    if (type == 0) {
        _tilemap->addTile(c, r, Color4::BLACK, false,
                          _assets->get<Texture>("black"));
        //        Vec2 pos(128 * c, 128 * r);
        //        std::shared_ptr<TileController> tile =
        //        std::make_shared<TileController>(
        //            pos, Size(128, 128), Color4::WHITE, false,
        //            _assets->get<Texture>("black"), pos.y + 12);
        //        _obstacles.emplace_back(tile);
        //        tile->addChildTo(_obstacleNode);
    } else {
        std::shared_ptr<Texture> texture = getTexture(type);
        _tilemap->addTile(c, r, Color4::WHITE, true, texture);
    }
}

// void SGameController::addWallTile(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
//     if (_tilemap->getDimensions().width > c &&
//         _tilemap->getDimensions().height > r) {
//         _tilemap->setTileTraversable(c, r, false);
//     }
//
//     int index = type - 1;
//     std::shared_ptr<Texture> wall = _assets->get<Texture>("wall");
//     modifyTexture(wall, index, 8, 8);
//     Vec2 pos(128 * c, 128 * r);
//     int yPos = pos.y + 11;
//     if (index == 0 || index == 1 || index == 8 || index == 9 || index == 10
//     ||
//         index == 11 || index == 20 || index == 21 || index == 22 ||
//         index == 34 || index == 35) {
//         yPos -= 256;
//     } else if (index == 32 || index == 33) {
//         yPos -= 128;
//     } else if (index == 41 || index == 42 || index == 48 || index == 49) {
//         yPos += 128;
//     }
//
//     std::shared_ptr<TileController> tile = std::make_shared<TileController>(
//         pos, Size(128, 128), Color4::WHITE, false, wall, yPos);
//     _obstacles.emplace_back(tile);
//     tile->addChildTo(_obstacleNode);
// }
//
// void SGameController::addWallUpper(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
//     std::shared_ptr<Texture> wall = _assets->get<Texture>("wall_upper");
//     modifyTexture(wall, type - 329, 8, 8);
//     Vec2 pos(128 * c, 128 * r + 16 * 128);
//     int ind = type - 329;
//     int yPos = pos.y + 10;
//     if (ind >= 16 && ind <= 63) {
//         yPos += 128;
//     }
//     std::shared_ptr<TileController> tile = std::make_shared<TileController>(
//         pos, Size(128, 128), Color4::WHITE, false, wall, yPos);
//     _obstacles.emplace_back(tile);
//     tile->addChildTo(_obstacleNode);
// }
//
// void SGameController::addWallGrime(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
//     std::shared_ptr<Texture> wall = _assets->get<Texture>("wall_grime");
//     modifyTexture(wall, type - 193, 8, 8);
//     Vec2 pos(128 * c, 128 * r + 16 * 128);
//     std::shared_ptr<TileController> tile = std::make_shared<TileController>(
//         pos, Size(128, 128), Color4::WHITE, false, wall, pos.y + 9);
//     _obstacles.emplace_back(tile);
//     tile->addChildTo(_obstacleNode);
// }
//
// void SGameController::addWallLower(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
//     std::shared_ptr<Texture> wall = _assets->get<Texture>("wall_lower");
//     modifyTexture(wall, type - 393, 8, 8);
//     Vec2 pos(128 * c, 128 * r + 16 * 128);
//     std::shared_ptr<TileController> tile = std::make_shared<TileController>(
//         pos, Size(128, 128), Color4::WHITE, false, wall, pos.y + 8);
//     _obstacles.emplace_back(tile);
//     tile->addChildTo(_obstacleNode);
// }
//
// void SGameController::addFurnitures(int type, int c, int r) {
//     if (type == 0 || type - 129 == 0) {
//         return;
//     }
//     int idx = type - 129;
//     std::shared_ptr<Texture> furnitures =
//     _assets->get<Texture>("furnitures"); modifyTexture(furnitures, idx, 8,
//     8); Vec2 pos(128 * c, 128 * r + 16 * 128); float yPos = pos.y + 7; if
//     (idx == 6 || idx == 7) {
//         yPos -= 256;
//     } else if (idx == 14 || idx == 15) {
//         yPos -= 128;
//     }
//     std::shared_ptr<TileController> tile = std::make_shared<TileController>(
//         pos, Size(128, 128), Color4::WHITE, false, furnitures, yPos);
//     _obstacles.emplace_back(tile);
//     tile->addChildTo(_obstacleNode);
// }
//
// void SGameController::addCandles(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
//     std::shared_ptr<Texture> candleTexture = _assets->get<Texture>("candle");
//     std::shared_ptr<scene2::SpriteNode> candle =
//         scene2::SpriteNode::allocWithSheet(candleTexture, 1, 8, 8);
//     candle->setFrame(type - 321);
//     candle->setPosition(Vec2(128 * c + 16 * 128 + 32, 128 * r + 32 * 128 +
//     32)); _candleNodes.emplace_back(candle);
//     _obstacleNode->addChild(_candleNodes.at(_candleNodes.size() - 1));
// }

void SGameController::modifyTexture(std::shared_ptr<Texture>& texture,
                                    int index) {
    float x = 1.0 / 8;
    float y = 1.0 / 8;
    int c = index % 8;
    int r = index / 8;
    texture = texture->getSubTexture(c * y, (c + 1) * y, r * x, (r + 1) * x);
}

void SGameController::sortNodes() {
    if (!_selection && _spirit.getModel()->hunterAdded) {
        for (int n = 0; n < _hunterNodes.size(); n++) {
            _obstacleNode->removeChild(_hunterNodes.at(n));
            _obstacleNode->addChild(_hunterNodes.at(n));
        }
    }

    for (int i = 0; i < _sortedObstacles.size(); i++) {
        float xDiff =
            abs(_hunterXPos - _sortedObstacles[i][0]->getPosition().x);
        if (xDiff < 128 * 2) {
            for (int n = 0; n < _sortedObstacles.at(i).size(); n++) {
                if (_hunterYPos > _sortedObstacles[i][n]->getYPos() &&
                    _sortedObstacles[i][n]->isObstacle()) {
                    _sortedObstacles[i][n]->removeChildFrom(_obstacleNode);
                    _sortedObstacles[i][n]->addChildTo(_obstacleNode);
                }
            }
        }
    }

    for (int i = 0; i < _doorNodes.size(); i++) {
        if (_hunterYPos > _doorNodes.at(i)->getPositionY() + 32) {
            _obstacleNode->removeChild(_doorNodes.at(i));
            _obstacleNode->addChild(_doorNodes.at(i));
        }
    }

    for (int i = 0; i < _portraitNodes.size(); i++) {
        if (_hunterYPos > _portraitNodes.at(i)->getPositionY() - 64) {
            _obstacleNode->removeChild(_portraitNodes.at(i));
            _obstacleNode->addChild(_portraitNodes.at(i));
        } else {
            return;
        }
    }
}

std::shared_ptr<Texture> SGameController::getTexture(int type) {
    std::shared_ptr<Texture> texture;
    int idx = type;
    if (type < 65) {
        idx -= 1;
        texture = _assets->get<Texture>("final_wall");
    } else if (type < 129) {
        idx -= 65;
        texture = _assets->get<Texture>("final_floor");
    } else if (type < 193) {
        idx -= 129;
        texture = _assets->get<Texture>("final_decor");
    } else if (type < 257) {
        idx -= 193;
        texture = _assets->get<Texture>("final_wall_grime");
    } else if (type < 321) {
        return nullptr;
    } else if (type < 385) {
        idx -= 321;
        texture = _assets->get<Texture>("final_wall_upper");
    } else if (type < 449) {
        idx -= 385;
        texture = _assets->get<Texture>("final_floor_ao");
    } else if (type < 513) {
        idx -= 449;
        texture = _assets->get<Texture>("final_decor2");
    } else if (type < 577) {
        idx -= 513;
        texture = _assets->get<Texture>("final_env");
    } else {
        idx -= 577;
        texture = _assets->get<Texture>("final_env2");
    }
    modifyTexture(texture, idx);
    return texture;
}

void SGameController::addDetails(int type, int c, int r) {
    std::shared_ptr<Texture> texture = getTexture(type);
    Vec2 pos(_level->getTileWidth() * c, _level->getTileWidth() * r);
    std::shared_ptr<TileController> tile = std::make_shared<TileController>(
        pos, _level->getTileSize(), Color4::WHITE, false, texture, pos.y);
    float yPos = getYPos(type, pos.y, tile);
    if (yPos != -FLT_MAX) {
        tile->setYPos(yPos);
    } else {
        tile->setObstacle(false);
    }
    _obstacles.emplace_back(tile);
    tile->addChildTo(_obstacleNode);
}

float SGameController::getYPos(int type, float pos,
                               std::shared_ptr<TileController>& tile) {
    float yPos = pos;
    int tileSize = _level->getTileWidth();
    int index = type;
    if (type < 65) {
        // wall
        index -= 1;
        if (index == 0 || index == 1 || index == 8 || index == 9 ||
            index == 10 || index == 11 || index == 20 || index == 21 ||
            index == 22 || index == 34 || index == 35) {
            yPos -= 2 * tileSize;
        } else if (index == 32 || index == 33) {
            yPos -= tileSize;
        }
    } else if (type < 129) {
        // floor
        return -FLT_MAX;
    } else if (type < 193) {
        // dector
        index -= 129;
        if (type == 16 || type == 24) {
            return FLT_MAX;
        }
        if (index == 6 || index == 7) {
            yPos -= 2 * tileSize;
        } else if (index == 14 || index == 15 || index == 1 || index == 2 ||
                   index == 3 || index == 4 || index == 17 || index == 18 ||
                   index == 19 || index == 20 || index == 32 || index == 33 ||
                   index == 34 || index == 35 || index == 36 || index == 37) {
            yPos -= tileSize;
        }
    } else if (type < 257) {
        // grime
        index -= 193;
    } else if (type < 321) {
        // placeholder
        return -FLT_MAX;
    } else if (type < 385) {
        // wall upper
        index -= 321;
        if (index >= 16 && index <= 63) {
            yPos += tileSize;
        }
    } else if (type < 449) {
        // ao
        return -FLT_MAX;
    } else if (type < 513) {
        // dector2
        index -= 449;
        if (index == 36 || index == 37 || index == 38 || index == 44 ||
            index == 45 || index == 46 || index == 52 || index == 53 ||
            index == 54) {
            _carpets.emplace_back(tile);
            return -FLT_MAX;
        }
        if (index < 8 || (index >= 16 && index <= 23) || index == 32 ||
            index == 48 || index == 49) {
            yPos -= tileSize;
        }
    } else if (type < 577) {
        // env
        index -= 513;
        if (index == 24 || index == 25 || index == 32 || index == 33 ||
            index == 40 || index == 41 || index == 35 || index == 36 ||
            index == 37 || index == 48 || index == 49 || index == 50 ||
            index == 54 || index == 55 || index == 62) {
            _holes.emplace_back(tile);
            return -FLT_MAX;
        }
    } else {
        // env2
        index -= 577;
        if (index == 0 || index == 1 || index == 6 || index == 7 ||
            (index >= 16 && index <= 21)) {
            yPos -= tileSize;
        }
    }
    return yPos;
}
