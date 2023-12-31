//
//  GameController.cpp
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
#include "HGameController.h"
#include "LevelConstants.h"
#include <box2d/b2_collision.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_world.h>
#include <cstdlib>
#include <cugl/cugl.h>
#include <random>
using namespace std;
using namespace cugl;
using namespace cugl::net;

#pragma mark Main Methods
HGameController::HGameController() {
    _hunter = std::make_shared<HunterController>();
    // Initialize SpiritController
    //    _spirit = SpiritController();
    //    _trap = TrapController();
    _treasure = TreasureController();
    //    auto inputController = InputController::getInstance();
    //    inputController->initListeners();
}

/**
 * Creates the game controller.
 *
 * This constructor will procedurally generate a tilemap immediately
 * on creation.
 *
 * @param displaySize   The display size of the game window
 * @param randoms        Reference to the random number generator
 */
HGameController::HGameController(
    const Size displaySize, const std::shared_ptr<cugl::AssetManager>& assets)
    : _scene(cugl::Scene2::alloc(displaySize)), _assets(assets) {
/// Initialize the tilemap and add it to the scene
/// //    SCENE_WIDTH = 1024;
//    SCENE_HEIGHT = 576;
/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH 100.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT 100.0f
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY 0.0f
#define DEBUG_ON 0
#define PLAYER_SIZE Vec2(40, 40)

    Vec2 gravity(0, 0);
    _gameStatus = 0;
    _count = 0;
    _status = START;
    _triggered = false;
    _inprogress = false;

    _frameNumDoor = 12;

    _doortrigger = false;
    _currdoor = 0;
    _tick = 0;
    _frameNumClose = 0;
    _didLose = false;
    _animates = true;
    _dimen = Application::get()->getDisplaySize();
    //    _offset = Vec3((_dimen.width)/2.0f,(_dimen.height)/2.0f,50);
    _offset = Vec3(0, 0, 50);

    std::shared_ptr<scene2::PolygonNode> background =
        scene2::PolygonNode::allocWithPoly(cugl::Rect(0, 0, 20000, 20000));
    background->setColor(Color4::BLACK);
    background->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setPosition(-1 * Size(9000, 9000) / 2);
    _scene->addChild(background);

    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
    _frameNumClam = 0;

    _obstacleNode = scene2::PolygonNode::alloc();
    _scene->addChild(_obstacleNode);

    _unlockbutton = std::dynamic_pointer_cast<scene2::Button>(
        _assets->get<scene2::SceneNode>("menu_host"));

    _unlockbutton->addListener(
        [=](const std::string& name, bool down) { this->_active = down; });
    _unlockbutton->setVisible(true);
    _unlockbutton->activate();

    _inputController = InputController::getInstance();
    _inputController->initListeners();

    // Initialize SpiritController
    _spirit = SpiritController();

    //    _trap.setAsObstacle(_world);

    // Add callbacks for entering/leaving collisions
    //

    //    _world->onEndContact = [this](b2Contact* contact) {
    //    _collision.endContact(contact); };
    // Create the world node
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(_offset);

    // Create the debug node. Note: Debug node draws in PHYSICS coordinates
    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(_scale);
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setPosition(_offset);
    _debugnode->setVisible(DEBUG_ON);

    _portraits = std::make_shared<PortraitSetController>(_assets, _scene, 0,
                                                         displaySize);

    _level = _assets->get<LevelModel>(LEVEL_FINAL_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }

    //        _tilemap->updatePosition(_scene->getSize() / 2);
    //    std::vector<std::vector<int>> tiles = _level->getTileTextures();
    //    int height = tiles[0].size();
    //    int width = tiles.size();
    //    _tilemap->updateDimensions(Vec2(height, width));
    //    _tilemap->updateColor(Color4::WHITE);
    //    _tilemap->updateTileSize(Size(128, 128));
    //    for (int i = 0; i < tiles.size() * tiles[0].size(); ++i) {
    //        int c = i % tiles[0].size();
    //        int r = i / tiles[0].size();
    //        int type = tiles[r][c];
    //        addFloorTile(type, c, width - 1 - r);
    //    }
    //
    //    std::vector<std::vector<int>> walls = _level->getWallTextures();
    //    height = walls[0].size();
    //    width = walls.size();
    //    for (int i = 0; i < walls.size() * walls[0].size(); ++i) {
    //        int c = i % walls[0].size();
    //        int r = i / walls[0].size();
    //        int type = walls[r][c];
    //        addWallTile(type, c, width - 1 - r);
    //    }
    //
    //    walls = _level->getWallUpperTextures();
    //    height = walls[0].size();
    //    width = walls.size();
    //    for (int i = 0; i < height * width; ++i) {
    //        int c = i % height;
    //        int r = i / height;
    //        int type = walls[r][c];
    //        addWallUpper(type, c, width - 1 - r);
    //    }
    //
    //    walls = _level->getWallGrimeTextures();
    //    height = walls[0].size();
    //    width = walls.size();
    //    for (int i = 0; i < height * width; ++i) {
    //        int c = i % height;
    //        int r = i / height;
    //        int type = walls[r][c];
    //        addWallGrime(type, c, width - 1 - r);
    //    }
    //
    //    walls = _level->getWallLowerTextures();
    //    height = walls[0].size();
    //    width = walls.size();
    //    for (int i = 0; i < height * width; ++i) {
    //        int c = i % height;
    //        int r = i / height;
    //        int type = walls[r][c];
    //        addWallLower(type, c, width - 1 - r);
    //    }
    //
    //    walls = _level->getFurnitureTextures();
    //    height = walls[0].size();
    //    width = walls.size();
    //    for (int i = 0; i < height * width; ++i) {
    //        int c = i % height;
    //        int r = i / height;
    //        int type = walls[r][c];
    //        addFurnitures(type, c, width - 1 - r);
    //    }

    //        walls = _level->getCandleTextures();
    //        height = walls[0].size();
    //        width = walls.size();
    //        for (int i = 0; i < height*width; ++i) {
    //            int c = i % height;
    //            int r = i / height;
    //            int type = walls[r][c];
    //            addCandles(type, c, width-1-r);
    //        }

    Rect rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    _offset = Vec2((_dimen.width - 1024) / 2.0f, (_dimen.height - 576) / 2.0f);
    _scale = _dimen.width == 1024 ? _dimen.width / rect.size.width
                                  : _dimen.height / rect.size.height;

    // TODO: Replace with networking request to find other players ID
    for (int i = 0; i < 3; i++) {
        initHunter(i);
    }

    _didWin = false;

    _hunter = _hunterSet[0];
    //    _hunter->setPosition(Vec2(10000,10000)*_scale);

    initCamera();
    _oneheart = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("oneheart"));
    _oneheart->setScale(1);
    _oneheart->setAnchor(Vec2::ANCHOR_CENTER);
    _oneheart->setPosition(_scene->getCamera()->getPosition() +
                                Vec2(-150 - 1000, 500));
    _oneheart->setVisible(false);
//    _scene->addChild(_oneheart);
        
    _twohearts = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("twohearts"));
    _twohearts->setScale(1);
    _twohearts->setAnchor(Vec2::ANCHOR_CENTER);
    _twohearts->setPosition(_scene->getCamera()->getPosition() +
                                Vec2(-150 - 1000, 500));
    _twohearts->setVisible(false);
//    _scene->addChild(_twohearts);
        
    _threehearts = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("threehearts"));
    _threehearts->setScale(1);
    _threehearts->setAnchor(Vec2::ANCHOR_CENTER);
    _threehearts->setPosition(_scene->getCamera()->getPosition() +
                                Vec2(-150 - 1000, 500));
    _threehearts->setVisible(true);
//    _scene->addChild(_threehearts);
        

//    for (int i = 0; i < 3; i++) {
//        _livehearts.push_back(scene2::SpriteNode::allocWithSheet(
//            assets->get<Texture>("heart_live"), 2, 8, 11));
//        _deadhearts.push_back(scene2::PolygonNode::allocWithTexture(
//            assets->get<Texture>("heart_dead")));
//        _livehearts[i]->setScale(0.45);
//        _livehearts[i]->setFrame(0);
//        _livehearts[i]->setAnchor(Vec2::ANCHOR_CENTER);
//        _livehearts[i]->setPosition(_scene->getCamera()->getPosition() +
//                                    Vec2(-130 * i - 1000, 500));
//        _livehearts[i]->setVisible(true);
//        _deadhearts[i]->setScale(0.45);
//        _deadhearts[i]->setAnchor(Vec2::ANCHOR_CENTER);
//        _deadhearts[i]->setPosition(_scene->getCamera()->getPosition() +
//                                    Vec2(-130 * i - 1000, 500));
//        _deadhearts[i]->setVisible(false);
//        //            _scene->addChild(_deadhearts[i]);
//        //            _scene->addChild(_livehearts[i]);
//    }

    _spriteSheets.push_back(assets->get<Texture>("kill_one"));
    _spriteSheets.push_back(assets->get<Texture>("kill_two"));
    _spriteSheets.push_back(assets->get<Texture>("kill_three"));
    _spriteSheets.push_back(assets->get<Texture>("kill_four"));
    _spriteSheets.push_back(assets->get<Texture>("kill_five"));
    _spriteSheets.push_back(assets->get<Texture>("kill_six"));

    for (int i = 0; i < _spriteSheets.size(); i++) {
        _spriteNodes.push_back(
            scene2::SpriteNode::allocWithSheet(_spriteSheets[i], 5, 2, 10));
        _spriteNodes[i]->setScale(1.7);
        _spriteNodes[i]->setFrame(0);
        _spriteNodes[i]->setAnchor(Vec2::ANCHOR_CENTER);
        //            _spriteNodes[i]->setPosition(Vec2(0, 0));
        _spriteNodes[i]->setVisible(false);
        //            _scene->addChild(_spriteNodes[i]);
    }

    _filterTexture = _assets->get<Texture>("filter");
    _filter = scene2::PolygonNode::allocWithTexture(_filterTexture);
    _filter->setPosition(_scene->getCamera()->getPosition());

    _filter->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _filter->setScale(Vec2(_dimen.width / 1280, _dimen.height / 720));

    _trappedbool = false;

    // Initialize the world
    _world = physics2::ObstacleWorld::alloc(
        Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, 0));

    _serializer = NetcodeSerializer::alloc();
    _deserializer = NetcodeDeserializer::alloc();

    initJoystick();
    Vec3 currPos = (_hunter->getPosition());
    if (_network) {
        _network->receive([this](const std::string source,
                                 const std::vector<std::byte>& data) {
            processData(source, data);
        });
        checkConnection();

        if (Vec2(currPos) != _lastpos) {
            std::vector<float> pos = std::vector<float>();
            pos.push_back(0);
            pos.push_back(currPos.x);
            pos.push_back(currPos.y);
            transmitPos(pos);
            _lastpos = Vec2(currPos);
        }
    }
}

#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void HGameController::reset() { CULog("reset"); }

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */

float HGameController::getZoom() {
    return std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
        ->getZoom();
}

void HGameController::update(float dt) {

    if (_gameStatus == 0) {
        _status = START;
        if (!_levelLoaded) {
            checkLevelLoaded();
        }

        //        for(auto obs:_obstacles){
        //            if(_hunter->getPosition()==){
        //
        //            }
        //        }
        sortNodes();

        AudioEngine::get()->play("theme", _theme, false, 0.5, false);

        _loseLabel->setText("You Lose!");
        _loseLabel->setPosition(_scene->getCamera()->getPosition() -
                                Vec2(200, 0));
        _loseLabel->setForeground(cugl::Color4f::RED);

//        if (_timer != 0) {
//            _timer = _timer - 1;
//        }

        _winLabel->setText("RUN to EXIT!");
        _winLabel->setPosition(_scene->getCamera()->getPosition() -
                               Vec2(200, 0));
        _winLabel->setForeground(cugl::Color4f::GREEN);

        _finalWinLabel->setText("You win!");
        _finalWinLabel->setPosition(_scene->getCamera()->getPosition() -
                                    Vec2(200, 0));
        _finalWinLabel->setForeground(cugl::Color4f::YELLOW);

        //        Vec2 minimapOffset = Vec2(_scene->getSize().width, 0) -
        //        (_miniMap == nullptr ? Vec2::ZERO : Vec2(1000, 0)) - Vec2(60,
        //        -30);
        //
        _miniMap->setPolygon(Rect(
            (_scene->getCamera()->getPosition().x * 0.4 + 230),
            (_scene->getCamera()->getPosition().y * 0.4 + 990), 1500, 1000));

        _miniMap->setPosition(_scene->getCamera()->getPosition() +
                              Vec2(1000, 370));

        _hunternow->setPosition(_scene->getCamera()->getPosition() +
                                Vec2(1000, 370));

        //        if(abs((_exitpos-_hunter->getPosition()).x)<740 &&
        //        abs((_exitpos-_hunter->getPosition()).y)<490){
        //            _huntertwo->setPosition(_hunternow->getPosition()-(_hunter->getPosition()-_exitpos)*0.4);
        //        }else{
        //            _huntertwo->setPosition(Vec2(-10000000,-1000000));
        //        }

        if (int(_timer / 60 / 60) == 0) {
            AudioEngine::get()->play("tension", _tension, false, 0.5, false);
        }
        CULog("didlose %d", _didLose);

        if ((int(_timer / 60 / 60) == 0 && (int(_timer / 60) % 60 == 0) &&
             !_didLose && !_didFinalwin) ||
            (_finalCount >= 216)) {

            //        _scene->addChild(_loseNode);
            //        _scene->addChild(_loseLabel);
//            _endScene =
//                std::make_shared<EndScene>(_scene, _assets, false, true);
//            _endScene->addChildTo(_scene);
            _didLose = true;
//            transmitSpiritWin();
            _gameStatus = 1;
        }

        if (_treasureCount >= 3 && !_didWin && !_didLose) {
            _scene->addChild(_exit);
//            _scene->addChild(_winLabel);
            _didWin = true;
        }

        if (!_didFinalwin && _didWin && !_didLose &&
            abs(_hunter->getPosition().x - _exitpos.x) < 200 &&
            abs(_hunter->getPosition().y - _exitpos.y) < 200) {
            //            _scene->removeChild(_winLabel);
            _scene->addChild(_finalWinLabel);
//            _endScene =
//                std::make_shared<EndScene>(_scene, _assets, false, false);
//            _endScene->addChildTo(_scene);
            _didFinalwin = true;
            transmitHunterWin();
            _gameStatus = 1;
        }

        for (auto& indicator : _indicators) {
            indicator->setVisible(false);
        }
        _filter->setPosition(_scene->getCamera()->getPosition());
        string seconds = std::to_string(int(_timer / 60) % 60).length() <= 1 ? "0" + std::to_string(int(_timer / 60) % 60) : std::to_string(int(_timer / 60) % 60);
        _timerLabel->setText(std::to_string(int(_timer / 60 / 60)) + ":" +
                             seconds);
        _timerLabel->setPosition(_scene->getCamera()->getPosition() -
                                 Vec2(50, 700));
        //        float vPos = _scene->getSize().height - 20 -
        //                     _timerLabel->getSize().height * getZoom() / 2;
        //        float hPos = _scene->getSize().width / 2 -
        //                     _timerLabel->getSize().width * getZoom() / 2;
        //        _timerLabel->setPosition(
        //            _scene->getCamera()->screenToWorldCoords(Vec2(hPos,
        //            vPos)));
        _timerLabel->setScale(2);
        _timerLabel->setForeground(cugl::Color4f::WHITE);

        _treasureLabel->setText(std::to_string(int(_treasureCount)) +
                                "/3 Treasure Collected");

        _treasureLabel->setFont(_assets->get<Font>("gamefont"));

        _treasureLabel->setPosition(_scene->getCamera()->getPosition() +
                                    Vec2(-200, 500));

        _treasureLabel->setForeground(cugl::Color4f::WHITE);

        // ea->setPosition(100*Vec2(_star->getBody()->GetTransform().p.x,_star->getBody()->GetTransform().p.y));
        //    _collision.init(_hunter.getHunterBody(), _trap.getTrapBody(),
        //    _treasure.getTreasureBody());
        _world->update(dt);
        _world->activateCollisionCallbacks(true);
        _world->onBeginContact = [this](b2Contact* contact) {
            CULog("COLLIDE!");
            //_hunter->move(0,0);
            _collision.beginContact(contact);
        };

        if (_animates) {
            _portraits->updatespecific(_indexfromspirit);
            if (_indexfromspirit == 0) {
                _indicators[_indicators.size() - 1]->setVisible(true);
            }
            if (_indexfromspirit == -1) {
                for(auto id:_indicators){
                    id->setVisible(false);
                }
            }
            if (_indexfromspirit != 0 && _indexfromspirit != -1) {
                _indicators[_indexfromspirit - 1]->setVisible(true);
            }
        }



        _inputController->update(dt);
        if (_inputController->didPressReset()) {
            reset();
        }
        //                if (inputController->didPress() &&
        //                inputController->getPosition().x>1700 ){
        //                    _killed = true;
        //                }
        // for hunter side kill testing

        for (int i = 0; i < _doorslocked.size(); i++) {
            if (_hunter->detectedDoor(
                    _doors.at(_doorslocked[i])->getModelPosition())) {
                _lockhunter->setVisible(true);
                _currdoor = _doorslocked[i];
                _currdoorindex = i;

                if (
                    abs(_inputController->getPosition().x -
                        _scene->worldToScreenCoords(_hunter->getPosition()).x) <
                        100 &&
                    abs(_inputController->getPosition().y -
                        _scene->worldToScreenCoords(_hunter->getPosition()).y) <
                        100) {
                    _triggered = true;
                }
                _lockhunter->setPosition(_hunter->getPosition());
            }

            else {
                _lockhunter->setVisible(false);
            }
        }

        if (_triggered or _inprogress) {
            if (_timerlock != 0) {
                _inprogress = true;
                _timerlock = _timerlock - 1;
                _timerLabellock->setVisible(true);
                _timerLabellock->setPosition(
                    _scene->getCamera()->getPosition() - Vec2(100, 0));
                _timerLabellock->setScale(8);

                _timerLabellock->setText(
                    std::to_string(int(_timerlock / 60) % 60 + 1));

                _timerLabellock->setForeground(cugl::Color4f::RED);

            } else {
                _inprogress = true;
                _timerLabellock->setVisible(false);
                _tick = 0;
                _frameNum = (_frameNum + 1) % _lockhunter->getSpan();
                _lockhunter->setFrame(_frameNum);
                if (_lockhunter->getSpan() - 1 == _frameNum) {
                    _inprogress = false;
                    _doortrigger = true;
                    _frameNum = 0;
                    _triggered = false;
                    _timerlock = 300;
                    _doorslocked.erase(_doorslocked.begin() + _currdoorindex);
                    _lockhunter->setFrame(0);
                    _lockhunter->setVisible(false);
                }
            }
            _tick++;
        }

        if (_doortrigger) {
            _doors.at(_currdoor)->setFrame(_frameNumDoor %
                                           (_doors.at(_currdoor)->getFrame()));
            _frameNumDoor = _frameNumDoor - 1;
            if (_frameNumDoor <= 0) {
                //            _frameNumDoor=12;
                _doortrigger = false;
                transmitUnlockDoor(_doorslocked[_currdoorindex]);
            }
        }

        int posx;
        int posxup;
        int posyup;
        int posy;

        int midx;
        int midy;

        Vec3 currPos = (_hunter->getPosition());

        Vec3 currPosAdj = (_hunter->getPosition());
        posx = (int)(currPosAdj.x) / _tileWidth;
        posy = (int)((currPosAdj.y)) / _tileHeight;

        posxup = (int)(currPosAdj.x + 40) / _tileWidth;
        posyup = (int)((currPosAdj.y + 40)) / _tileHeight;

        midx = (int)(currPosAdj.x + 20) / _tileWidth;
        midy = (int)((currPosAdj.y + 20)) / _tileHeight;

        float forward = _inputController->getForward();

        float rightward = _inputController->getRight();
        _threehearts->setPosition(_scene->getCamera()->getPosition() +
                                                Vec2(-150 - 850, 500));
        _twohearts->setPosition(_scene->getCamera()->getPosition() +
                                                Vec2(-150 - 850, 500));
        _oneheart->setPosition(_scene->getCamera()->getPosition() +
                                                Vec2(-150 - 850, 500));
//        for (int i = 0; i < 3; i++) {
//            _deadhearts[i]->setPosition(_scene->getCamera()->getPosition() +
//                                        Vec2(-180 * i - 850, 500));
//            _livehearts[i]->setPosition(_scene->getCamera()->getPosition() +
//                                        Vec2(-180 * i - 850, 500));
//        }

        _count++;
        if (_count == 6) {
            _hunter->setViewFrame(forward, rightward, _beingKilled);
            if (_heart_frame >= 11) {
                _heart_frame = 0;
            }
//            for (int i = 0; i < 3; i++) {
//                _livehearts[i]->setFrame(_heart_frame);
//            }
            _heart_frame += 1;
            _count = 0;
        }
        _countfortimer++;

        _beingKilled = false;
        if (_kill_ani_count < 84) {
            _kill_ani_count += 1;
            _beingKilled = true;
        }
        if (_finalKilled) {
            for (int i = 0; i < _spriteNodes.size(); i++) {
                _spriteNodes[i]->setPosition(
                    _scene->getCamera()->getPosition());
            }
            _spriteNodes[_finalCount / 40]->setVisible(true);
            _spriteNodes[_finalCount / 40]->setFrame((_finalCount % 40) / 4);

            //            _spriteNodes[0]->setVisible(true);
            //            _spriteNodes[0]->setFrame(0);
            _finalCount += 1;
            // move frame
        }

        if (_finalCount < 1 && !_beingKilled) {
            _move = true;
        } else {
            _move = false;
        }

        for (auto obsta : _obstaclePoly) {
            if (obsta.contains(_shadow->getPosition() + Vec2(40, 0) +
                               Vec2(rightward * _hunter->getVelocity().x,
                                    forward * _hunter->getVelocity().y)) or
                obsta.contains(_shadow->getPosition() - Vec2(40, 0) +
                               Vec2(rightward * _hunter->getVelocity().x,
                                    forward * _hunter->getVelocity().y)) or
                obsta.contains(_shadow->getPosition() +
                               Vec2(rightward * _hunter->getVelocity().x,
                                    forward * _hunter->getVelocity().y))) {
                _move = false;
            }
        }

        if (_doorslocked.size() != 0) {
            for (int i = 0; i < _doorslocked.size(); i++) {
                Vec2 position = _doors.at(_doorslocked[i])->getViewPosition();

                if (abs(_hunter->getPosition().x +
                        Vec2(rightward * _hunter->getVelocity().x,
                             forward * _hunter->getVelocity().y)
                            .x -
                        (position.x)) < 128 * 2 and
                    abs(_hunter->getPosition().y +
                        Vec2(rightward * _hunter->getVelocity().x,
                             forward * _hunter->getVelocity().y)
                            .y -
                        (position.y - 128)) < 30) {
                    _move = false;
                }
            }
        }

        if (_move) {
            if (_hunter->getTrapSize() > 0) {
                _ismovedonece = false;

                for (int i = 0; i < _hunter->getTrapSize(); i++) {
                    if (_hunter->getTraps()[i]->getTrigger()) {
                        _ismovedonece = true;
                    }
                }
                if (!_ismovedonece && _move) {
                    _hunter->move(forward, rightward);
                }

            } else {
                _hunter->move(forward, rightward);
            }
        }

        //        if (_didLose || _didFinalwin) {
        //            // Freeze movement after lose/win
        //            forward = 0;
        //            rightward = 0;
        //            _hunter->setViewFrame(forward, rightward, _beingKilled);
        //        }

        if (_trappedbool == false) {
            if (_hunter->getTrapSize() != 0) {
                for (int i = 0; i < _hunter->getTrapSize(); i++) {
                    if (abs(_hunter->getTraps()[i]->getPosition().x -
                            _hunter->getPosition().x) <= 300 &&
                        abs(_hunter->getTraps()[i]->getPosition().y -
                            _hunter->getPosition().y) <= 300) {
                        if (_neverPlayed) {
                            AudioEngine::get()->play("trapSound", _trapSound,
                                                     false, 0.8, true);
                            _neverPlayed = false;
                        }
                        _trappedbool = true;
                        _frameNumClam++;
                        _hunter->getTraps()[i]->setTrigger(true);
                        _hunter->getTrapViews()[i]->setVisible(true,
                                                               _frameNumClam);
                        _trapped = i;
                        if (!_timertriggered) {
                            _countfortimer = 0;
                            _timertriggered = true;
                        }
                    }
                }
            }
        } else {
            _frameNumClam++;
            _hunter->getTraps()[_trapped]->setTrigger(true);
            _hunter->getTrapViews()[_trapped]->setVisible(true, _frameNumClam);

            _countfortimer++;
            if (_hunter->getTraps()[_trapped]->getTrigger() &&
                _countfortimer >= 300) {
                _hunter->getTraps()[_trapped]->setTrigger(false);
                _hunter->getTrapViews()[_trapped]->setVisible(false,
                                                              _frameNumClam);
                _hunter->removeTrap(_trapped);
                _neverPlayed = true;
                _trappedbool = false;
                _timertriggered = false;
                transmitTrapTriggered(_hunter->getPosition());
            }
        }

        if (abs(_treasure.getPosition().x - _hunter->getPosition().x) <= 200 &&
            abs(_treasure.getPosition().y - _hunter->getPosition().y) <= 200 &&
            !_collision.didHitTreasure) {
            CULog("IN ");
            _collision.didHitTreasure = true;
            _treasure.getNode()->setVisible(false);
            AudioEngine::get()->play("treasureSound", _treasureSound, false,
                                     0.8, true);
            transmitTreasureStolen();
            _treasureCount++;
        }
        if (abs(_treasure2.getPosition().x - _hunter->getPosition().x) <= 200 &&
            abs(_treasure2.getPosition().y - _hunter->getPosition().y) <= 200 &&
            !_collision.didHitTreasure2) {
            CULog("IN 2");
            _collision.didHitTreasure2 = true;
            _treasure2.getNode()->setVisible(false);
            AudioEngine::get()->play("treasureSound", _treasureSound, false,
                                     0.8, true);
            transmitTreasureStolen();
            _treasureCount++;
        }
        if (abs(_treasure3.getPosition().x - _hunter->getPosition().x) <= 200 &&
            abs(_treasure3.getPosition().y - _hunter->getPosition().y) <= 200 &&
            !_collision.didHitTreasure3) {
            CULog("IN 3");
            _collision.didHitTreasure3 = true;
            _treasure3.getNode()->setVisible(false);
            AudioEngine::get()->play("treasureSound", _treasureSound, false,
                                     0.8, true);
            transmitTreasureStolen();
            _treasureCount++;
        }

        //        ea->setPosition(Vec2(a,b));
        _shadow->setPosition(_hunter->getPosition() - Vec2(130, 270));
        updateCamera(dt);

        cugl::Vec2 center = _inputController->getCenter();       
      

        if (center.x < 900) {
            if (_inputController->didPress()) {

                _innerJoystick->setVisible(true);
                _outerJoystick->setVisible(true);
            } else if (_inputController->didRelease()) {
                _innerJoystick->setVisible(false);
                _outerJoystick->setVisible(false);
                
            } else if (_inputController->isTouchDown()) {
                cugl::Vec2 center = _inputController->getCenter();
                cugl::Vec2 centerPos =
                    _scene->getCamera()->screenToWorldCoords(center);
                if (!(_didLose || _didFinalwin)) {
                    updateJoystick(forward, rightward, centerPos);
                }
            }
        }

        // TODO: update direction index for portraits on spirit control
        //    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int
        //    index#>)

        if (_network) {
            _network->receive([this](const std::string source,
                                     const std::vector<std::byte>& data) {
                processData(source, data);
            });
            checkConnection();

            if (Vec2(currPos) != _lastpos) {
                std::vector<float> pos = std::vector<float>();
                pos.push_back(0);
                pos.push_back(currPos.x);
                pos.push_back(currPos.y);
                transmitPos(pos);
                _lastpos = Vec2(currPos);
            }
            //            for (int i = 0; i < _spriteNodes.size(); i++) {
            //                _spriteNodes[i]->setPosition(
            //                _scene->getCamera()->getPosition());
            //
            //            }
            if (_killed) {
                AudioEngine::get()->play("damage", _damageSound, false, 0.5, false);
//                _livehearts[_killCount]->setVisible(false);
//                _deadhearts[_killCount]->setVisible(true);
                _killCount += 1;
                if(_killCount==1){
                    _threehearts->setVisible(false);
                    _twohearts->setVisible(true);
                }
                if(_killCount==2){
                    _twohearts->setVisible(false);
                    _oneheart->setVisible(true);
                }
                _killed = false;
                _kill_ani_count = 0;
                //            _move = false;
            }
            if (_killCount == 3) {
                AudioEngine::get()->play("kill", _killSound, false, 0.5, false);
                _finalKilled = true;
                for (int i = 0; i < _spriteNodes.size(); i++) {

                    _scene->addChild(_spriteNodes[i]);
                }
                _killCount += 1;
            }
        }
    } else if (_gameStatus == 1) {
        // Hunter lose or win
//        _endScene->update();
        _countEndAnim++;
        AudioEngine::get()->clear("tension", 1);
        AudioEngine::get()->clear("theme", 1);
        if (_countEndAnim > 100) {
            CULog("stuck here!!!!");
            _status = RESET;
//            _gameStatus = 0;
        }

    } else {
        // Hunter win!
        //        _endScene->update();
    }
}

/**
 * Draws all this scene to the given SpriteBatch.
 *
 * The default implementation of this method simply draws the scene graph
 * to the sprite batch.  By overriding it, you can do custom drawing
 * in its place.
 *
 * @param batch     The SpriteBatch to draw with.
 */
void HGameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}

void HGameController::initDoors() {
    std::vector<std::pair<Vec2, int>> doors = _level->getDoors();

    for (int i = 0; i < doors.size(); i++) {
        _doors.emplace_back(std::make_shared<DoorController>(
            _assets, doors[i].first, doors[i].second, 0));
        _doors.at(i)->addChildToVector(_doorNodes);
        _doors.at(i)->setFrame(0);
    }
}

void HGameController::animatelocks() {
    for (int i = 0; i < _doorslocked.size(); i++) {
        _doors.at(_doorslocked[i])->setFrame(11);
    }
}

void HGameController::addlocks(int index) {
    _doorslocked.push_back(index);
    _stopanim = false;
    while (!_stopanim) {
        _doors.at(index)->setFrame(_frameNumClose);
        _frameNumClose += 1;
        if (_frameNumClose == _doors.at(index)->getFrame()) {
            _stopanim = true;
            _frameNumClose = 0;
        }
    }
}

void HGameController::checkLevelLoaded() {
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

        // Initialize SpiritController
        _spirit = SpiritController();

        _tileHeight = _level->getTileWidth();
        _tileWidth = _level->getTileWidth();

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

        // TODO: implement direction and direction limits

        //        _map =
        //            scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("map"));
        //        _map->setPolygon(Rect(0, 0, 2304, 2304));
        //        //    _map = scene2::PolygonNode::allocWithPoly(Rect(0, 0,
        //        9216, 9216));
        //        //    _map ->setTexture(_assets->get<Texture>("map"));
        //        _scene->addChild(_map);

        _miniMap = scene2::PolygonNode::allocWithTexture(
            _assets->get<Texture>("minimaplarge"));
        _miniMap->setScale(0.4);

        _miniMap->shiftTexture(-_miniMap->getSize().height - 780,
                               -_miniMap->getSize().width - 500);

        for (int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits->addPortrait(_portraitNodes, i,
                                    _level->getPortaits()[i][0],
                                    _level->getPortaits()[i][1], Vec3(0, 0, -1),
                                    Vec2::ZERO, true, _level->getPortraitTypes()[i], _level->getBattery());
            if (i <= 0)
                continue;
            auto indicator = cugl::scene2::PolygonNode::allocWithTexture(
                _assets->get<Texture>("indicator" + to_string(i)));
            indicator->setPosition(_level->getPortaits()[i][2]);
            indicator->setScale(2);
            indicator->setVisible(false);
            _scene->addChild(indicator);
            _indicators.emplace_back(indicator);
        }

        // Draw hunter shadow
        for (int i = 0; i < 3; i++) {
            _shadowTexture = _assets->get<Texture>("shadow");
            std::shared_ptr<scene2::PolygonNode> shadownow =
                scene2::PolygonNode::allocWithTexture(_shadowTexture);

            shadownow->setPosition(_hunterSet[i]->getPosition() -
                                   Vec2(130, 270));

            shadownow->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

            _shadowSet[i] = shadownow;
        }

        _shadow = _shadowSet[0];
        //        _shadow->setScale(Vec2(_dimen.width/1280,_dimen.height/720));

        //        for (int i = 0; i < 3; i++){
        //            _scene->addChild(_shadowSet[i]);
        //        }
        _hunterNodes.emplace_back(_shadowSet[0]);

        // Draw hunter after shadow
        //        for (int i = 0; i < 3; i++){
        //            _hunterSet[i]->addChildTo(_scene);
        ////            _hunterSet[i]->setPosition(Vec2(0,0));
        //        }
        // _hunter->setPosition(Vec2(5,5));
        _hunterSet[0]->addChildToNode(_hunterNodes);

        for (int n = 0; n < _hunterNodes.size(); n++) {
            _obstacleNode->addChild(_hunterNodes.at(n));
        }

        _tilemap->addDoorTo(_scene);

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

        _scene->addChild(_filter);
        _scene->addChild(_oneheart);
        _scene->addChild(_twohearts);
        _scene->addChild(_threehearts);
        
//        for (int i = 0; i < 3; i++) {
//            _scene->addChild(_deadhearts[i]);
//            _scene->addChild(_livehearts[i]);
//        }

        _scene->addChild(_miniMap);

        _timerlock = 300;
        _timerLabellock = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "5", _assets->get<Font>("gamefont"));
        _scene->addChild(_timerLabellock);
        _timerLabellock->setVisible(false);

        _timer = 5270;
        _timerLabel = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "2:00", _assets->get<Font>("gamefont"));
        _scene->addChild(_timerLabel);

        _treasureCount = 0;
        _treasureLabel = cugl::scene2::Label::allocWithText(
            Vec2(300, 300), "0/3 Treasure Collected",
            _assets->get<Font>("gamefont"));
        _treasureLabel->setColor(cugl::Color4f::WHITE);
        _scene->addChild(_treasureLabel);

        // win-lose labels
        _loseLabel = cugl::scene2::Label::allocWithText(
            Vec2(800, 800), "You Lose!", _assets->get<Font>("gamefont"));
        _winLabel = cugl::scene2::Label::allocWithText(
            Vec2(800, 800), "RUN to EXIT!", _assets->get<Font>("gamefont"));
        _finalWinLabel = cugl::scene2::Label::allocWithText(
            Vec2(800, 800), "You Win!", _assets->get<Font>("gamefont"));

        _timertriggered = false;
        initLock();

        _removedvar = true;

        PolyFactory _pf = PolyFactory();
        _hunternow =
            scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0, 0), 60));
        _hunternow->setAnchor(cugl::Vec2::ANCHOR_CENTER);
        _hunternow->setScale(0.3f);
        _hunternow->setColor(Color4(Vec4(1, 0, 0, 1)));
        _scene->addChild(_hunternow);

        animatelocks();

        _counterbool = false;

        // sounds
        _theme = _assets->get<Sound>("theme");
        _tension = _assets->get<Sound>("tension");
        _trapSound = _assets->get<Sound>("trapSound");
        _treasureSound = _assets->get<Sound>("treasureSound");
        _killSound = _assets->get<Sound>("kill");
        _damageSound = _assets->get<Sound>("damage");

        addPolys();
        makePolyObstacle(_obstaclePoly);

        //        _world->addObstacle(_hunter->getModel());
        //        for (auto obj:_obstacleswall){
        //            _world->addObstacle(obj);
        //        }

        _scene->addChild(_worldnode);

        _hunterspun.emplace_back(Vec2(17*128, (40-1-25)*128));
        _hunterspun.emplace_back(Vec2(50*128, (40-1-5)*128));
        _hunterspun.emplace_back(Vec2(59*128, (40-1-29)*128));

        srand(time(NULL));

        int index = rand() % 3;

        // Manually add more treasurepos
//        _treasurepos.emplace_back(Vec2(3900, 5700));
//
//        _treasurepos.emplace_back(Vec2(2000, 5700));
//
//        _treasurepos.emplace_back(Vec2(1000, 5700));
        _treasurepos.emplace_back(Vec2(15*128, (40-1-23)*128));

        _treasurepos.emplace_back(Vec2(46*128, (40-1-7)*128));

        _treasurepos.emplace_back(Vec2(55*128, (40-1-27)*128));
        
        

        Vec2 posxx=randomHunterLocation();
        _hunter->setPosition(posxx);
        _exitpos = posxx;

        _exitTexture = _assets->get<Texture>("exit");
        _exit = scene2::PolygonNode::allocWithTexture(_exitTexture);
        _exit->setScale(1.3);
        _exit->setPosition(_exitpos);

        
        std:vector<Vec2> nowtrea=randomTreasureLocation();

        _treasure = TreasureController(_assets, _scene->getSize(), PLAYER_SIZE,
                                       nowtrea.at(0));
        _treasure2 = TreasureController(_assets, _scene->getSize(), PLAYER_SIZE,
                                        nowtrea.at(1));
        _treasure3 = TreasureController(_assets, _scene->getSize(), PLAYER_SIZE,
                                        nowtrea.at(2));

        _treasure.setPosition(nowtrea.at(0));
        _treasure.addChildTo(_scene);
        _treasure2.setPosition(nowtrea.at(1));
        _treasure2.addChildTo(_scene);
        _treasure3.setPosition(nowtrea.at(2));
        _treasure3.addChildTo(_scene);
        //_scene->addChild(_exit);

        _levelLoaded = true;
    }
}

/**
 *
 */
void HGameController::initCamera() {

    Vec3 curr = _scene->getCamera()->getPosition();
    //    if(_star!=nullptr){
    //        Vec3 next = _offset + (Vec3(_star->getPosition().x,
    //        _star->getPosition().y, 1));
    //    }
    //    _scene->getCamera()->translate(Vec3(0, 0, -0.8));
    //    Vec3 next =
    //        _offset + (Vec3(_hunter->getPosition().x,
    //        _hunter->getPosition().y, 1));
    _scene->getCamera()->setPosition(_hunter->getPosition());
    _scene->getCamera()->setFar(100000);
    _scene->getCamera()->setNear(0);
    _scene->getCamera()->update();
    //    std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
    //        ->setZoom(2.5);
}

Vec2 HGameController::randomHunterLocation(){

    std::vector<Vec2> boarder = _level->getBoarder();
    srand(time(NULL));
    int maxx= -1410065408;
    int maxy=-1410065408;
    int minx=1410065407;
    int miny=1410065407;
    for (auto i : boarder){
        if(i.y<1280){
            i.y=i.y+40;
        }
        if(i.y>1280){
            i.y=i.y-40;
        }
        if(i.x>1600){
            i.x=i.x-40;
        }
        if(i.x<1600){
            i.x=i.x+40;
        }
        if (i.x>maxx){
            maxx=i.x;
        }
        if (i.y>maxy){
            maxy=i.y;
        }
        if (i.x<minx){
            minx=i.x;
        }
        if (i.y<miny){
            miny=i.y;
        }
    }
    
    cugl::EarclipTriangulator ea=EarclipTriangulator ();
    ea.set(boarder);
    ea.calculate();
    
    cugl::Poly2 inside=ea.getPolygon();
    
    int huntery = rand() % (maxy-miny)+miny+1;
    int hunterx = rand() % (maxx-minx)+minx+1;

    
    for (auto obsta : _obstaclePoly) {
        while (obsta.contains(Vec2(hunterx,huntery) - Vec2(130, 270)) or
               obsta.contains(Vec2(hunterx,huntery) - Vec2(130, 270)+ Vec2(40, 0)) or
               obsta.contains(Vec2(hunterx,huntery) - Vec2(130, 270)- Vec2(55, 0))
               or !inside.contains(Vec2(hunterx,huntery)) or !inside.contains(Vec2(hunterx,huntery) - Vec2(130, 270)) or !inside.contains(Vec2(hunterx,huntery) - Vec2(130, 270)+ Vec2(55, 0)) or !inside.contains(Vec2(hunterx,huntery) - Vec2(130, 270)- Vec2(55, 0))){
            huntery = rand() % (maxy-miny)+miny+1;
            hunterx = rand() % (maxx-minx)+minx+1;
        }
        
    }
    
    return Vec2(hunterx,huntery);
}



std::vector<Vec2> HGameController::randomTreasureLocation(){
    std::vector<Vec2> treasurepo;
    std::vector<Vec2> boarder = _level->getBoarder();
    
    srand(time(NULL));
    int maxx= -1410065408;
    int maxy=-1410065408;
    int minx=1410065407;
    int miny=1410065407;
    for (auto i : boarder){
        if(i.y<1280){
            i.y=i.y+35;
        }
        if (i.x>maxx){
            maxx=i.x;
        }
        if (i.y>maxy){
            maxy=i.y;
        }
        if (i.x<minx){
            minx=i.x;
        }
        if (i.y<miny){
            miny=i.y;
        }
    }
    
    cugl::EarclipTriangulator ea=EarclipTriangulator ();
    ea.set(boarder);
    ea.calculate();
    
    cugl::Poly2 inside=ea.getPolygon();
    
    int huntery = rand() % (maxy-miny)+miny+1;
    int hunterx = rand() % (maxx-minx)+minx+1;

    
    for (auto obsta : _obstaclePoly) {
        while (obsta.contains(Vec2(hunterx,huntery)) or !_tilemap->isTileTraversable(Vec2(hunterx,huntery))  or !inside.contains(Vec2(hunterx,huntery)) or !inside.contains(Vec2(hunterx,huntery) - Vec2(130, 270)) or !inside.contains(Vec2(hunterx,huntery) - Vec2(130, 270)+ Vec2(40, 0)) or sqrt((hunterx-_hunter->getPosition().x)*(hunterx-_hunter->getPosition().x)+(huntery-_hunter->getPosition().y)*(huntery-_hunter->getPosition().y))<1000){
            huntery = rand() % (maxy-miny)+miny+1;
            hunterx = rand() % (maxx-minx)+minx+1;
        }
    }
    treasurepo.emplace_back(Vec2(hunterx,huntery));
    
    int huntery2 = rand() % (maxy-miny)+miny+1;
    int hunterx2 = rand() % (maxx-minx)+minx+1;
    
    for (auto obsta : _obstaclePoly) {
        while (obsta.contains(Vec2(hunterx2,huntery2))or !_tilemap->isTileTraversable(Vec2(hunterx2,huntery2)) or
                !inside.contains(Vec2(hunterx2,huntery2)) or !inside.contains(Vec2(hunterx2,huntery2) - Vec2(130, 270)) or !inside.contains(Vec2(hunterx2,huntery2) - Vec2(130, 270)+ Vec2(40, 0)) or sqrt((hunterx2-_hunter->getPosition().x)*(hunterx2-_hunter->getPosition().x)+(huntery2-_hunter->getPosition().y)*(huntery2-_hunter->getPosition().y))<1000 or huntery2==huntery or sqrt((hunterx2-hunterx)*(hunterx2-hunterx)+(huntery2-huntery)*(huntery2-huntery))<2000){
            huntery2 = rand() % (maxy-miny)+miny+1;
            hunterx2 = rand() % (maxx-minx)+minx+1;
        }
    }
    treasurepo.emplace_back(Vec2(hunterx2,huntery2));
    
    int huntery3 = rand() % (maxy-miny)+miny+1;
    int hunterx3 = rand() % (maxx-minx)+minx+1;
    
    for (auto obsta : _obstaclePoly) {
        while (obsta.contains(Vec2(hunterx3,huntery3)) or !_tilemap->isTileTraversable(Vec2(hunterx3,huntery3)) or
            !inside.contains(Vec2(hunterx3,huntery3)) or !inside.contains(Vec2(hunterx3,huntery3) - Vec2(130, 270)) or !inside.contains(Vec2(hunterx3,huntery3) - Vec2(130, 270)+ Vec2(40, 0)) or sqrt((hunterx3-_hunter->getPosition().x)*(hunterx3-_hunter->getPosition().x)+(huntery3-_hunter->getPosition().y)*(huntery3-_hunter->getPosition().y))<1000 or huntery3==huntery or huntery3==huntery2 or sqrt((hunterx3-hunterx)*(hunterx3-hunterx)+(huntery3-huntery)*(huntery3-huntery))<2000 or sqrt((hunterx3-hunterx2)*(hunterx3-hunterx2)+(huntery3-huntery2)*(huntery3-huntery2))<2000){
            huntery3 = rand() % (maxy-miny)+miny+1;
            hunterx3 = rand() % (maxx-minx)+minx+1;
        }
    }
    treasurepo.emplace_back(Vec2(hunterx3,huntery3));
    
    return treasurepo;
}


/**
 * Updates camera based on the position of the controlled player
 */
void HGameController::updateCamera(float timestep) {

    if (!_didWin || _shiftback) {
        Vec2 curr = _scene->getCamera()->getPosition();
        _filter->setPosition(_scene->getCamera()->getPosition());
        _filter->setAnchor(Vec2::ANCHOR_CENTER);
        Vec2 fixHunterOffset = Vec2(600, 0);
        Vec2 next = _hunter->getPosition();

        int timeFactor = (_shiftback) ? 5 : 2;

        // restrict camera focus within 300<x<1800, 300<y<1900 window
        int cameraXmin = 300;
        // _tilemap->getDimensions().width*_tileWidth = 8192
        int cameraXmax = 8000;
        int cameraYmin = 300;
        // _tilemap->getDimensions().height*_tileWidth = 6144
        int cameraYmax = 6000;
        if (_hunter->getPosition().x < cameraXmin) {
            next.x = cameraXmin;
        } else if (_hunter->getPosition().x > cameraXmax) {
            next.x = cameraXmax;
        }
        if (_hunter->getPosition().y < cameraYmin) {
            next.y = cameraYmin;
        } else if (_hunter->getPosition().y > cameraYmax) {
            next.y = cameraYmax;
        }
        _scene->getCamera()->translate((next - curr) * timeFactor * timestep);

        _timerLabel->setPosition(_scene->getCamera()->getPosition() -
                                 Vec2(50, 500));
    }

    if (_didWin) {

        Vec3 curr = _scene->getCamera()->getPosition();
        //        Vec3 exit = Vec3(350,350,0);
        // camera pans back to entry
        // camera pans back to hunter
        if (!_shiftback) {
            _scene->getCamera()->translate((_exitpos - curr) * timestep);
        }
        if (abs(_scene->getCamera()->getPosition().x - _exitpos.x) < 200 &&
            abs(_scene->getCamera()->getPosition().y - _exitpos.y) < 200) {
            _shiftback = true;
        }
    }

    _filter->setPosition(_scene->getCamera()->getPosition());
    _scene->getCamera()->update();
}

void HGameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
}

void HGameController::initHunter(int hunterId) {

    //  Vec2 gravity(0,DEFAULT_GRAVITY);

    //_exitpos=Vec2(2000,2000);
    _hunterSet[hunterId] = std::make_shared<HunterController>(
        _assets, _scene->getSize(), _scene, PLAYER_SIZE, hunterId, _scale);
}

void HGameController::initLock() {
    CULog("lock initialized");

    _frameNum = 0;
    _spriteSheet = _assets->get<Texture>("lock_animation_hunter");
    _lockhunter = scene2::SpriteNode::allocWithSheet(_spriteSheet, 2, 8, 16);
    _lockhunter->setScale(0.5);
    _lockhunter->setFrame(_frameNum);
    _lockhunter->setAnchor(Vec2::ANCHOR_CENTER);
    _lockhunter->setPosition(_hunter->getPosition());
    _lockhunter->setVisible(false);

    _scene->addChild(_lockhunter);

    // Reposition the joystick components
}

void HGameController::initJoystick() {
    PolyFactory _pf = PolyFactory();
    // mac
    //_outerJoystick =
    // scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0,0), 60)); ios
    _outerJoystick =
        scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0, 0), 120));
    _outerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _outerJoystick->setScale(1.0f);
    _outerJoystick->setColor(Color4(Vec4(1, 1, 1, 0.25)));
    _scene->addChild(_outerJoystick);

    // Create inner part of joystick view
    // mac
    //_innerJoystick =
    // scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0,0), 30)); ios
    _innerJoystick =
        scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0, 0), 60));
    _innerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _innerJoystick->setScale(1.0f);
    _innerJoystick->setColor(Color4(Vec4(1, 1, 1, 0.25)));
    _scene->addChild(_innerJoystick);
    _innerJoystick->setPosition(Vec2(-10000, -10000));
    _outerJoystick->setPosition(Vec2(-10000, -10000));
    _joystickon = true;
    _outerJoystick ->setVisible(false);
    _innerJoystick ->setVisible(false);

    

    // Reposition the joystick components
}

void HGameController::makePolyObstacle(std::vector<Poly2> input) {
    for (auto ob : input) {
        std::shared_ptr<cugl::physics2::PolygonObstacle> p =
            physics2::PolygonObstacle::alloc(Poly2(ob) / 100, Vec2::ZERO);
        p->setBodyType(b2_staticBody);
        p->setDebugScene(_worldnode);

        _obstacleswall.emplace_back(p);
    }
}

void HGameController::removeJoystick() {
    _scene->removeChild(_outerJoystick);
    _scene->removeChild(_innerJoystick);
    _joystickon = false;
}

void HGameController::processData(const std::string source,
                                  const std::vector<std::byte>& data) {
    if (source == _network->getHost()) {
        _deserializer->receive(data);
        std::vector<float> mes =
            std::get<std::vector<float>>(_deserializer->read());
        if (mes[0] == 1) {
            _hunter->addTrap(Vec2(mes[1], mes[2]));
        } else if (mes[0] == 3) {
            //            CULog("portrait received");
            int idx = static_cast<int>(mes[1]);
            _indexfromspirit = idx;
        } else if (mes[0] == 5) {
            int idx = static_cast<int>(mes[1]);
            //            CULog("door index: %d", idx);
            addlocks(idx);
        } else if (mes[0] == 9) {
            CULog("got killed");
            _killed = true;
        } else if (mes[0] == 10) {
            _didLose = true;
            _didFinalwin = false;
            _didWin = false;
        } else if (mes[0] == 8) {
            _didWin = true;
            _didFinalwin = true;
            _didLose = false;
        }else if (mes[0] == 11) {
            int idx = static_cast<int>(mes[1]);
            _timer=idx;
        }

        _deserializer->reset();
    }
}

/**
 * Checks that the network connection is still active.
 *
 * Even if you are not sending messages all that often, you need to be calling
 * this method regularly. This method is used to determine the current state
 * of the scene.
 *
 * @return true if the network connection is still active.
 */
bool HGameController::checkConnection() {
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

void HGameController::transmitPos(std::vector<float> position) {
    _serializer->writeFloatVector(position);
    _network->broadcast(_serializer->serialize());
    _serializer->reset();
}

void HGameController::transmitTreasureStolen() {
    std::vector<float> message = std::vector<float>();
    message.push_back(4);
    message.push_back(1);
    _serializer->writeFloatVector(message);
    _network->sendToHost(_serializer->serialize());
    _serializer->reset();
}

void HGameController::transmitUnlockDoor(int idx) {
    std::vector<float> message = std::vector<float>();
    message.push_back(6);
    message.push_back(idx);
    _serializer->writeFloatVector(message);
    _network->sendToHost(_serializer->serialize());
    _serializer->reset();
}

void HGameController::transmitTrapTriggered(Vec2 position) {
    std::vector<float> message = std::vector<float>();
    message.push_back(7);
    message.push_back(position.x);
    message.push_back(position.y);
    _serializer->writeFloatVector(message);
    _network->sendToHost(_serializer->serialize());
    _serializer->reset();
}

void HGameController::transmitHunterWin() {
    std::vector<float> message = std::vector<float>();
    message.push_back(8);
    _serializer->writeFloatVector(message);
    _network->sendToHost(_serializer->serialize());
    _serializer->reset();
}

void HGameController::transmitSpiritWin() {
    std::vector<float> message = std::vector<float>();
    message.push_back(10);
    _serializer->writeFloatVector(message);
    _network->sendToHost(_serializer->serialize());
    _serializer->reset();
}

void HGameController::updateJoystick(float forward, float rightward,
                                     cugl::Vec2 centerPos) {

    _outerJoystick->setPosition(centerPos);
    _innerJoystick->setPosition(centerPos + Vec2(rightward, forward) * 100);
}

void HGameController::addFloorTile(int type, int c, int r) {
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

// void HGameController::addWallTile(int type, int c, int r) {
//     if (type == 0) {
//         return;
//     }
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

// void HGameController::addWallUpper(int type, int c, int r) {
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
// void HGameController::addWallGrime(int type, int c, int r) {
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
// void HGameController::addWallLower(int type, int c, int r) {
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
// void HGameController::addFurnitures(int type, int c, int r) {
//     if (type == 0 || type - 129 == 0) {
//         if (type - 129 == 0) {
//             _treasurepos.emplace_back(Vec2(128 * c, 128 * r + 16 * 128));
//         }
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
// void HGameController::addCandles(int type, int c, int r) {
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

void HGameController::modifyTexture(std::shared_ptr<Texture>& texture,
                                    int index) {
    float x = 1.0 / 8;
    float y = 1.0 / 8;
    int c = index % 8;
    int r = index / 8;
    texture = texture->getSubTexture(c * y, (c + 1) * y, r * x, (r + 1) * x);
}

void HGameController::addPolys() {
    std::vector<Vec2> boarder = _level->getBoarder();
    cugl::SimpleExtruder extruder = SimpleExtruder();
    extruder.set(boarder, true);
    extruder.calculate(10, 10);
    _obstaclePoly.emplace_back(extruder.getPolygon());

    std::vector<std::vector<Vec2>> obs = _level->getCollision();
    cugl::Path2 line = Path2();
    for (int i = 0; i < obs.size(); i++) {
        line.set(obs.at(i));
        EarclipTriangulator et;
        et.set(line);
        et.calculate();
        cugl::Poly2 poly = et.getPolygon();
        _obstaclePoly.emplace_back(poly);
    }
}

void HGameController::sortNodes() {
    for (int n = 0; n < _hunterNodes.size(); n++) {
        _obstacleNode->removeChild(_hunterNodes.at(n));
        _obstacleNode->addChild(_hunterNodes.at(n));
    }

    for (int i = 0; i < _sortedObstacles.size(); i++) {
        float xDiff = abs(_hunter->getPosition().x -
                          _sortedObstacles[i][0]->getPosition().x);
        if (xDiff < 128 * 2) {
            for (int n = 0; n < _sortedObstacles.at(i).size(); n++) {
                if (_hunter->getPosition().y >
                        _sortedObstacles[i][n]->getYPos() &&
                    _sortedObstacles[i][n]->isObstacle()) {
                    _sortedObstacles[i][n]->removeChildFrom(_obstacleNode);
                    _sortedObstacles[i][n]->addChildTo(_obstacleNode);
                }
            }
        }
    }

    for (int i = 0; i < _doorNodes.size(); i++) {
        if (_hunter->getPosition().y > _doorNodes.at(i)->getPositionY() + 32) {
            _obstacleNode->removeChild(_doorNodes.at(i));
            _obstacleNode->addChild(_doorNodes.at(i));
        }
    }

    for (int i = 0; i < _portraitNodes.size(); i++) {
        if (_hunter->getPosition().y >
            _portraitNodes.at(i)->getPositionY() - 64) {
            _obstacleNode->removeChild(_portraitNodes.at(i));
            _obstacleNode->addChild(_portraitNodes.at(i));
        } else {
            return;
        }
    }
}

std::shared_ptr<Texture> HGameController::getTexture(int type) {
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

void HGameController::addDetails(int type, int c, int r) {
    std::shared_ptr<Texture> texture = getTexture(type);
    Vec2 pos(_level->getTileWidth() * c, _level->getTileWidth() * r);
    std::shared_ptr<TileController> tile = std::make_shared<TileController>(
        pos, _level->getTileSize(), Color4::WHITE, false, texture, pos.y);
    float yPos = getYPos(type, pos.y, tile, c, r);
    if (yPos != -FLT_MAX) {
        tile->setYPos(yPos);
    } else {
        tile->setObstacle(false);
    }
    _obstacles.emplace_back(tile);
    tile->addChildTo(_obstacleNode);
}

float HGameController::getYPos(int type, float pos,
                               std::shared_ptr<TileController>& tile, int c, int r) {
    float yPos = pos;
    int tileSize = _level->getTileWidth();
    int index = type;
    if (type < 65) {
        // wall
        _tilemap->setTileTraversable(c, r, false);
        yPos += 11;
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
        yPos += 8;
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
        yPos += 9;
        index -= 193;
    } else if (type < 321) {
        // placeholder
        return -FLT_MAX;
    } else if (type < 385) {
        // wall upper
        yPos += 10;
        index -= 321;
        if (index >= 16 && index <= 63) {
            yPos += tileSize;
        }
    } else if (type < 449) {
        // ao
        return -FLT_MAX;
    } else if (type < 513) {
        // dector2
        yPos += 8;
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
        yPos += 8;
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
        yPos += 8;
        index -= 577;
        if (index == 0 || index == 1 || index == 6 || index == 7 ||
            (index >= 16 && index <= 21)) {
            yPos -= tileSize;
        }
    }
    return yPos;
}
