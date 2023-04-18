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
#include <cstdlib>
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>
#include <cugl/cugl.h>
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
    #define DEFAULT_WIDTH   2000.0f
    /** Height of the game world in Box2d units */
    #define DEFAULT_HEIGHT  1000.0f
    /** The default value of gravity (going down) */
    #define DEFAULT_GRAVITY 0.0f
    #define DEBUG_ON        0
    #define PLAYER_SIZE     Vec2(40,40)

    Rect rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT);
    Vec2 gravity(0,DEFAULT_GRAVITY);

    _count = 0;
   
    _triggered = false;
    _inprogress=false;
        
    _frameNumDoor=12;
        

    _doortrigger=false;
    _currdoor=0;
    _tick = 0;
    _frameNumClose=0;
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
    _unlockbutton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_host"));
    
    _unlockbutton->addListener(
        [=](const std::string& name, bool down) { this->_active = down;
        });
    _unlockbutton->setVisible(true);
    _unlockbutton->activate();


        
    auto inputController = InputController::getInstance();
    inputController->initListeners();
        
   
        
    _treasure = TreasureController(assets, displaySize, PLAYER_SIZE);

    
        
    // Initialize SpiritController
    _spirit = SpiritController();
    
    
    // Initialize the world
    _world = physics2::ObstacleWorld::alloc(rect,gravity);
   // _hunter.setAsObstacle(_world);
//    _trap.setAsObstacle(_world);
    _treasure.setAsObstacle(_world);
    
    // Add callbacks for entering/leaving collisions
 //
//    _collision.init(_hunter.getHunterBody(), _trap.getTrapBody(), _treasure.getTreasureBody());
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) { _collision.beginContact(contact); };
        
        
    
//    _world->onEndContact = [this](b2Contact* contact) { _collision.endContact(contact); };
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
    
    _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }

    // TODO: Replace with networking request to find other players ID
    for (int i = 0; i < 3; i++){
        initHunter(i);
    }

    _hunter = _hunterSet[0];

        
        
    initCamera();
        
    _serializer = NetcodeSerializer::alloc();
    _deserializer = NetcodeDeserializer::alloc();
    
//    initJoystick();
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
void HGameController::update(float dt) {
    if (!_levelLoaded) {
        checkLevelLoaded();
    }
     
    AudioEngine::get()->play("theme", _theme, true, _theme->getVolume(), false);
    
    _loseLabel->setText("You Lose!");
    _loseLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(200,0));
    _loseLabel->setForeground(cugl::Color4f::RED);
    
    if(_timer!=0){
        _timer=_timer-1;
    }
    
    _winLabel->setText("RUN to EXIT!");
    _winLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(200,0));
    _winLabel->setForeground(cugl::Color4f::GREEN);
    
    _finalWinLabel->setText("You win!");
    _finalWinLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(200,0));
    _finalWinLabel->setForeground(cugl::Color4f::YELLOW);
    

   
    if(int(_timer/6000)==0){
       AudioEngine::get()->play("tension", _tension, true, _theme->getVolume(), true);
   }

    if(int(_timer/6000)==0 && int(_timer/100) % 60 ==0 && !_didLose && !_didFinalwin){
//        _scene->addChild(_loseNode);
//        _scene->addChild(_loseLabel);
        _gameStatus = 1;
        _endScene = std::make_shared<EndScene>(_assets, true);
        _endScene->addChildTo(_scene);
        _didLose = true;
    }
    else if(_gameStatus == 1){
        // Hunter lose
        CULog("Hunter lose!");
        _endScene->update();
    }else{
        // Hunter win!
        
    }
    

    if(_treasureCount>=1 && !_didWin && !_didLose){
        _scene->addChild(_winLabel);
        _didWin = true;
    }
    
    if(!_didFinalwin && _didWin && !_didLose && _hunter->getPosition().x < 400 && _hunter->getPosition().y < 400){
        _scene->removeChild(_winLabel);
        _scene->addChild(_finalWinLabel);
        _didFinalwin = true;
    }
    
    _timerLabel->setText(std::to_string(int(_timer/6000))+":"+std::to_string(int(_timer/100) % 60 ));
    _timerLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(0,300));
    _timerLabel->setForeground(cugl::Color4f::WHITE);
    
    
    
    _treasureLabel->setText(std::to_string(int(_treasureCount)) + " Treasure Collected");
    _treasureLabel->setPosition(_scene->getCamera()->getPosition()+Vec2(350,350));
    _treasureLabel->setForeground(cugl::Color4f::YELLOW);
    
//    _collision.init(_hunter.getHunterBody(), _trap.getTrapBody(), _treasure.getTreasureBody());
    _world->update(dt);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) { _collision.beginContact(contact); };
    
    
    if(_animates){
             _portraits->updatespecific(_indexfromspirit);
         }

        

    auto inputController = InputController::getInstance();

    inputController->update(dt);
    if (inputController->didPressReset()) {
        reset();
    }
    
    for (int i=0; i<_doorslocked.size(); i++){
        if(_hunter->detectedDoor(_doors.at(_doorslocked[i])->getModelPosition())){
            _lockhunter->setVisible(true);
            _currdoor=_doorslocked[i];
            _currdoorindex=i;
            
            if(abs(inputController->getPosition().x-_scene->worldToScreenCoords(_hunter->getPosition()).x)<100&&abs(inputController->getPosition().y-_scene->worldToScreenCoords(_hunter->getPosition()).y)<100){
                _triggered=true;
            }
            _lockhunter->setPosition(_hunter->getPosition());
        }
        
        else{
            _lockhunter->setVisible(false);
        }
    }
        
    if(_triggered or _inprogress){
        if (_timerlock!=0) {
            _inprogress=true;
            _timerlock=_timerlock-1;
            _timerLabellock->setVisible(true);
            _timerLabellock->setPosition(_scene->getCamera()->getPosition()-Vec2(100,0));
            _timerLabellock->setScale(8);
        
            _timerLabellock->setText(std::to_string(int(_timerlock/60) % 60 +1));
           
            _timerLabellock->setForeground(cugl::Color4f::RED);
            
        }
        else {
            _inprogress=true;
            _timerLabellock->setVisible(false);
            _tick = 0;
            _frameNum = (_frameNum + 1) % _lockhunter->getSpan();
            _lockhunter->setFrame(_frameNum);
            if(_lockhunter->getSpan()-1==_frameNum){
                _inprogress=false;
                _doortrigger=true;
                _frameNum=0;
                _triggered=false;
                _timerlock=300;
                _doorslocked.erase(_doorslocked.begin()+_currdoorindex);
                _lockhunter->setFrame(6);
            }
        }
        _tick++;
    }
    
    if(_doortrigger){
        _doors.at(_currdoor)->setFrame(_frameNumDoor%12);
        _frameNumDoor=_frameNumDoor-1;
        if(_frameNumDoor<=0){
//            _frameNumDoor=12;
            _doortrigger=false;
            transmitUnlockDoor(_doorslocked[_currdoorindex]);
        }
    }

    std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
    int posx;
    int posxup;
    int posyup;
    int posy;

    int midx;
    int midy;

    Vec3 currPos = (_hunter->getPosition());
    posx = (int)(currPos.x) / _tileWidth;
    posy = (int)((currPos.y)) / _tileHeight;

    posxup = (int)(currPos.x + 40) / _tileWidth;
    posyup = (int)((currPos.y + 40)) / _tileHeight;

    midx = (int)(currPos.x + 20) / _tileWidth;
    midy = (int)((currPos.y + 20)) / _tileHeight;

    float forward = inputController->getForward();
    float rightward = inputController->getRight();

    _count++;
    if (_count == 6) {
        _hunter->setViewFrame(forward, rightward);
        _count = 0;
    }
    _countfortimer++;
    
    std::string left = (midy<18 && posx <18 && midy>-1 && posx >-1) ? tiles[midy][posx]: "black";
    std::string up = (posyup<18 && midx <18 && posyup>-1 && midx >-1) ? tiles[posyup][midx]: "black";
    std::string bottom = (posy<18 && midx <18 && posy>-1 && midx >-1) ? tiles[posy][midx]: "black";
    std::string right = (midy<18 && posxup <18 && midy>-1 && posxup >-1) ? tiles[midy][posxup]: "black";

    if (left == "black") {
        if (rightward < 0) {
            rightward = 0;
        }
    }
    if (right == "black") {
        if (rightward > 0) {
            rightward = 0;
        }
    }
    if (up == "black") {
        if (forward > 0) {
            forward = 0;
        }
    }
    if (bottom == "black") {
        if (forward < 0) {
            forward = 0;
        }
    }
    
    if(_doorslocked.size()!=0){
        for (int i=0; i<_doorslocked.size(); i++){
        Vec2 position = _doors.at(_doorslocked[i])->getModelPosition();
            if(abs(_hunter->getPosition().x-position.x)<150 && abs(_hunter->getPosition().y-position.y)<150){
                rightward = -1;
                forward = -1;
            }
        }
    }
    if(_didLose || _didFinalwin){
        //Freeze movement after lose/win
        forward=0;
        rightward=0;
        _hunter->setViewFrame(forward, rightward);
    }
    if (_hunter->getTrapSize()== 0 ){
        _hunter->move(forward,rightward);
            }
    else{
        _ismovedonece=false;
        for (int i=0;i<_hunter->getTraps().size();i++){
            if(_hunter->getTraps()[i]->getTrigger()){
                    _ismovedonece=true;
                }
            }
        if(!_ismovedonece){
            _hunter->move(forward,rightward);
        }
        }
    //trap collision
   
//    if(_collision.didHitTrap){
//        _trap.setTrigger(true);
//    }
    
    
    
    if(_hunter->getTrapSize()!=0){
        for (int i=0;i<_hunter->getTrapSize();i++){
            if(abs(_hunter->getTraps()[i]->getPosition().x-_hunter->getPosition().x)<= 50 && abs(_hunter->getTraps()[i]->getPosition().y-_hunter->getPosition().y)<= 50){
                if(_neverPlayed){
                    AudioEngine::get()->play("trapSound", _trapSound, false, _theme->getVolume(), true);
                    _neverPlayed = false;
                }
                    
                _hunter->getTraps()[i]->setTrigger(true);
                _hunter->getTrapViews()[i]->setVisible(true);
               
                    if(!_timertriggered){
                        _countfortimer=0;
                        _timertriggered=true;
                    }
                   
                }
                
                }
        for (int i=0;i<_hunter->getTraps().size();i++){
            if (_hunter->getTraps()[i]->getTrigger()&& _countfortimer >= 300){
                _hunter->getTraps()[i]->setTrigger(false);
                _hunter->getTrapViews()[i]->setVisible(false);
                _hunter->removeTrap(i);
                _neverPlayed = true;
                _timertriggered=false;
                transmitTrapTriggered(_hunter->getPosition());
//                if(_removedvar){
//                    _hunter->removeTrap(i);
//                    _removedvar=false;
//                    _timertriggered=false;
//                    transmitTrapTriggered(_hunter->getPosition());
//                }
            }
        }

        }
    
    if(abs(_treasure.getPosition().x-_hunter->getPosition().x)<= 100 && abs(_treasure.getPosition().y-_hunter->getPosition().y)<= 100 && !_collision.didHitTreasure ){
        _collision.didHitTreasure = true;
        _treasure.getNode() ->setVisible(false);
        AudioEngine::get()->play("treasureSound", _treasureSound, false, _theme->getVolume(), true);
        transmitTreasureStolen();
        _treasureCount++;
    }

    _shadow->setPosition(_hunter->getPosition()-Vec2(130,270));

    updateCamera(dt);
    if(!(_didLose || _didFinalwin)){
        updateJoystick(forward,rightward);
    }
    
    
    // TODO: update direction index for portraits on spirit control
    //    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int index#>)
    
    if (_network) {
        _network->receive([this](const std::string source,
                                 const std::vector<std::byte>& data) {
            processData(source,data);
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

void HGameController::initDoors(){
    std::vector<std::pair<Vec2, int>> doors = _level->getDoors();
    
    for (int i=0; i<doors.size(); i++){
        _doors.emplace_back(std::make_shared<DoorController>(_assets, doors[i].first, doors[i].second, 0));
        _doors.at(i)->addChildTo(_scene);
        _doors.at(i)->setFrame(0);
        
    }
    
}

void HGameController::animatelocks(){
    for (int i=0;i<_doorslocked.size();i++){
        _doors.at(_doorslocked[i])->setFrame(11);
    }
    
}

void HGameController::addlocks(int index){
    _doorslocked.push_back(index);
    _stopanim=false;
    while(!_stopanim){
        _doors.at(index)->setFrame(_frameNumClose);
        _frameNumClose+=1;
        if(_frameNumClose==_doors.at(index)->getFrame()){
            _stopanim=true;
            _frameNumClose=0;
        }
    }
}

void HGameController::checkLevelLoaded() {
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
        
        // Initialize SpiritController
        _spirit = SpiritController();

        _tileHeight = 128;
        _tileWidth = 128;

        // TODO: implement direction and direction limits
        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(_tileWidth, _tileHeight));

        _map =
            scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("map"));
        _map->setPolygon(Rect(0, 0, 2304, 2304));
        //    _map = scene2::PolygonNode::allocWithPoly(Rect(0, 0, 9216, 9216));
        //    _map ->setTexture(_assets->get<Texture>("map"));
        _scene->addChild(_map);
        _scene->addChild(_worldnode);
        _scene->addChild(_debugnode);
        
        for (int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits->addPortrait(i + 1, _level->getPortaits()[i].first,
                                    _level->getPortaits()[i].second,
                                    Vec3(0, 0, -1), Vec2::ZERO,
                                    _level->getBattery());
        }
        
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

        // Initialize HunterController
        
        
//        _hunter = HunterController(_assets, _scene->getSize(),_scene, PLAYER_SIZE);
        
        // Draw hunter shadow
        for (int i = 0; i < 3; i++){
            _shadowTexture = _assets->get<Texture>("shadow");
            std::shared_ptr<scene2::PolygonNode> shadownow = scene2::PolygonNode::allocWithTexture(_shadowTexture);
            
            shadownow->setPosition(_hunterSet[i]->getPosition()-Vec2(130,270));
            
            shadownow->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
            
            _shadowSet[i]=shadownow;
        }
        
        
        _shadow=_shadowSet[0];
        //        _shadow->setScale(Vec2(_dimen.width/1280,_dimen.height/720));
        
        for (int i = 0; i < 3; i++){
            _scene->addChild(_shadowSet[i]);
        }
        
        
        //Draw hunter after shadow
        for (int i = 0; i < 3; i++){
            _hunterSet[i]->addChildTo(_scene);
        }
        
        

        // _trap = TrapController(_assets, _scene->getSize(), PLAYER_SIZE);
        // _trap.addChildTo(_scene);
        _treasure = TreasureController(_assets, _scene->getSize(), PLAYER_SIZE);

        _treasure.addChildTo(_scene);
        
        _tilemap->addDoorTo(_scene);

        _filterTexture = _assets->get<Texture>("filter");
        _filter = scene2::PolygonNode::allocWithTexture(_filterTexture);
        _filter->setPosition(_scene->getCamera()->getPosition());
        
        _filter->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _filter->setScale(Vec2(_dimen.width/1280,_dimen.height/720));
        _scene->addChild(_filter);
        
        _timerlock = 300;
        _timerLabellock = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "5", _assets->get<Font>("pixel32"));
        _scene->addChild(_timerLabellock);
        _timerLabellock->setVisible(false);

        _timer = 12000;
        _timerLabel = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "2:00", _assets->get<Font>("pixel32"));
        _scene->addChild(_timerLabel);

        _treasureCount = 0;
        _treasureLabel= cugl::scene2::Label::allocWithText(Vec2(200,200), "0 Treasure Collected", _assets->get<Font>("pixel32"));
        _treasureLabel->setColor(cugl::Color4f::YELLOW);
        _scene->addChild(_treasureLabel);
        
        //win-lose labels
        _loseLabel = cugl::scene2::Label::allocWithText(Vec2(800,800), "You Lose!", _assets->get<Font>("pixel32"));
        _winLabel = cugl::scene2::Label::allocWithText(Vec2(800,800), "RUN to EXIT!", _assets->get<Font>("pixel32"));
        _finalWinLabel = cugl::scene2::Label::allocWithText(Vec2(800,800), "You Win!", _assets->get<Font>("pixel32"));


        _timertriggered=false;
        initLock();
        initDoors();
        _removedvar=true;
        
        //for testing only, delete when network added
//        _doorslocked.push_back(3);
        
        animatelocks();

        //sounds
        _theme = _assets->get<Sound>("theme");
        _tension = _assets->get<Sound>("tension");
        _trapSound = _assets->get<Sound>("trapSound");
        _treasureSound = _assets->get<Sound>("treasureSound");
        
        initJoystick();
        
        _levelLoaded = true;
        
    }
}

/**
 *
 */
void HGameController::initCamera() {

    
    Vec3 curr = _scene->getCamera()->getPosition();
    Vec3 next = _offset + (Vec3(_hunter->getPosition().x, _hunter->getPosition().y, 1));
    _scene->getCamera()->translate(next - curr);
    _scene->getCamera()->setFar(100000);
    _scene->getCamera()->setNear(0);
    _scene->getCamera()->update();
    
}

/**
 * Updates camera based on the position of the controlled player
 */
void HGameController::updateCamera(float timestep) {

    if(!_didWin || _shiftback){
        Vec2 curr = _scene->getCamera()->getPosition();
        _filter->setPosition(_scene->getCamera()->getPosition());
        _filter->setAnchor(Vec2::ANCHOR_CENTER);
        Vec2 next = _offset
        + ((Vec3(_hunter->getPosition().x, _hunter->getPosition().y, 1)));
        
        int timeFactor = (_shiftback)? 5 : 2;

        // restrict camera focus within 300<x<1800, 300<y<1900 window
        int cameraXmin = 300;
        int cameraXmax = 1800;
        int cameraYmin = 300;
        int cameraYmax = 1900;
        if(_hunter->getPosition().x < cameraXmin){
            next.x = cameraXmin;
        }
        else if(_hunter->getPosition().x > cameraXmax){
            next.x = cameraXmax;
        }
        if(_hunter->getPosition().y < cameraYmin){
            next.y = cameraYmin;
        }else if(_hunter->getPosition().y > cameraYmax){
            next.y = cameraYmax;
        }
        _scene->getCamera()->translate((next - curr) * timeFactor *timestep);
        
        _timerLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(0,300));
        
    }
    
    if(_didWin){
        
        Vec3 curr = _scene->getCamera()->getPosition();
        Vec3 exit = Vec3(350,350,0);
        // camera pans back to entry
        // camera pans back to hunter
        if(!_shiftback){
            _scene->getCamera()->translate((exit - curr) * timestep);
        }
        if(_scene->getCamera()->getPosition().x < 400){
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
   
    _hunterSet[hunterId]=std::make_shared<HunterController>(_assets, _scene->getSize(),_scene, PLAYER_SIZE, hunterId);
}

void HGameController::initLock(){
    CULog("lock initialized");
    
    _frameNum = 0;
    _spriteSheet = _assets->get<Texture>("lock_animation_hunter");
    _lockhunter =
        scene2::SpriteNode::allocWithSheet(_spriteSheet, 2, 8, 16);
    _lockhunter->setScale(0.5);
    _lockhunter->setFrame(_frameNum);
    _lockhunter->setAnchor(Vec2::ANCHOR_CENTER);
    _lockhunter->setPosition(_hunter->getPosition());
    _lockhunter->setVisible(false);
    
    
    _scene->addChild(_lockhunter);
    
    // Reposition the joystick components
    
    
}


void HGameController::initJoystick(){
    CULog("joystick initialized");
    // Create outer part of joystick
    _outerJoystick = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("joystick_bottom"));
    _outerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _outerJoystick->setScale(0.7f);
    //_outerJoystick->setPolygon(Rect(Vec2(0,0),Vec2(20,150)));
    _scene->addChild(_outerJoystick);
    _outerJoystick->setPosition(Vec2(20,150));

    // Create inner part of joystick view
    _innerJoystick = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("joystick_top"));
    _innerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _innerJoystick->setScale(0.7f);
   // _innerJoystick->setPolygon(Rect(Vec2::ZERO,Vec2(20,150)));
    _innerJoystick->setPosition(Vec2(20,150));
    _scene->addChild(_innerJoystick);
}

void HGameController::processData(const std::string source, const std::vector<std::byte> &data) {
    if (source == _network->getHost()) {
        _deserializer->receive(data);
        std::vector<float> mes = std::get<std::vector<float>>(_deserializer->read());
        if (mes[0] == 1) {
            _hunter->addTrap(Vec2(mes[1], mes[2]));
        } else if (mes[0] == 3) {
            CULog("portrait received");
            int idx = static_cast<int>(mes[1]);
            _indexfromspirit = idx;
        } else if (mes[0] == 5) {
            int idx = static_cast<int>(mes[1]);
            CULog("door index: %d", idx);
            addlocks(idx);
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

void HGameController::updateJoystick(float forward,float rightward){

    _outerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(680,350));
    _innerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(680,350)+Vec2(rightward,forward)*100);
}

