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
    _hunter = HunterController();
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

    Rect rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT);
    Vec2 gravity(0,DEFAULT_GRAVITY);

    _count = 0;
    _didLose = false;
    _dimen = Application::get()->getDisplaySize();
    //    _offset = Vec3((_dimen.width)/2.0f,(_dimen.height)/2.0f,50);
    _offset = Vec3(0, 0, 50);
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
        
    auto inputController = InputController::getInstance();
    inputController->initListeners();

    CULog("%f, %f", displaySize.width, displaySize.height);
    _hunter = HunterController(assets, displaySize, _scene);
    //    _trap = TrapController(assets, displaySize);
    _treasure = TreasureController(assets, displaySize);
        
    // Initialize SpiritController
    _spirit = SpiritController();
    
    
    // Initialize the world
    _world = physics2::ObstacleWorld::alloc(rect,gravity);
    _hunter.setAsObstacle(_world);
//    _trap.setAsObstacle(_world);
    _treasure.setAsObstacle(_world);
    
    // Add callbacks for entering/leaving collisions
    _collision.init(_hunter.getHunterBody(), _treasure.getTreasureBody());
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
    
    _loseLabel->setText("You Lose!");
    _loseLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(100,0));
    _loseLabel->setForeground(cugl::Color4f::RED);
    
    _timer=_timer-1;
    if(int(_timer/6000)==0 && int(_timer/100) % 60 ==0 && !_didLose){
        _scene->addChild(_loseLabel);
        _didLose = true;
    }
    _timerLabel->setText(std::to_string(int(_timer/6000))+":"+std::to_string(int(_timer/100) % 60 ));
    _timerLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(0,300));
    _timerLabel->setForeground(cugl::Color4f::WHITE);
    
    _treasureLabel->setText(std::to_string(int(_treasureCount)) + " Treasure Collected");
    _treasureLabel->setPosition(_scene->getCamera()->getPosition()+Vec2(100,290));
    _treasureLabel->setForeground(cugl::Color4f::YELLOW);
    
//    _collision.init(_hunter.getHunterBody(), _trap.getTrapBody(), _treasure.getTreasureBody());
    _world->update(dt);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) { _collision.beginContact(contact); };

    auto inputController = InputController::getInstance();
//    inputController->readInput();
    inputController->update(dt);
    if (inputController->didPressReset()) {
        reset();
    }

//    if(inputController->isKeyPressed(KeyCode::NUM_1)) {
//        _hunter.addTrap();
//    }
//    if(inputController->isKeyPressed(KeyCode::NUM_2)) {
//        CULog("NUM_2");
//    }
//    if(inputController->isKeyPressed(KeyCode::NUM_3)) {
//        CULog("NUM_3");
//    }
//    if(inputController->isKeyPressed(KeyCode::NUM_4)) {
//        CULog("NUM_4");
//    }
    
    // Will crash the program because the constructor doesn't set up the model/view yet (delete this comment later)

    // Will crash the program because the constructor doesn't set up the
    // model/view yet (delete this comment later)

    //    _hunter.update();
    //    _spirit.update();

    std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
    int posx;
    int posxup;
    int posyup;
    int posy;

    int midx;
    int midy;

    Vec3 currPos = (_hunter.getPosition());
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
        _hunter.setViewFrame(forward, rightward);
        _count = 0;
    }

    std::string left = tiles[midy][posx];
    std::string up = tiles[posyup][midx];
    std::string bottom = tiles[posy][midx];
    std::string right = tiles[midy][posxup];

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

//    bool age = _trap.update(); //false means trap active
//    if (!_trap.getTrigger()){
        _hunter.move(forward,rightward);
//    }
    //trap collision
   
//    if(_collision.didHitTrap){
//        _trap.setTrigger(true);
//    }
    
//    if(abs(_trap.getPosition().x-_hunter.getPosition().x)<= 80 && abs(_trap.getPosition().y-_hunter.getPosition().y)<= 80 && !age){
//        _trap.setTrigger(true);
//    }
//    if (_trap.getTrigger()&& _count == 5){
//        _trap.setViewFrame();
//    }
    
    if(abs(_treasure.getPosition().x-_hunter.getPosition().x)<= 100 && abs(_treasure.getPosition().y-_hunter.getPosition().y)<= 100 && !_collision.didHitTreasure ){
        _collision.didHitTreasure = true;
        _treasure.getNode() ->setVisible(false);
        _treasureCount++;
    }

    _shadow->setPosition(_hunter.getPosition()-Vec2(210,370));

    updateCamera(dt);
    updateJoystick(forward,rightward);
    
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

        _tileHeight = 256;
        _tileWidth = 256;

        // TODO: implement direction and direction limits
        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(_tileWidth, _tileHeight));

        _map =
            scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("map"));
        _map->setPolygon(Rect(0, 0, 4608, 4608));
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
        _hunter = HunterController(_assets, _scene->getSize(), _scene);
        
        // Draw hunter shadow
        _shadowTexture = _assets->get<Texture>("shadow");
        _shadow = scene2::PolygonNode::allocWithTexture(_shadowTexture);
        _shadow->setPosition(_hunter.getPosition()-Vec2(210,370));
        _shadow->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        //        _shadow->setScale(Vec2(_dimen.width/1280,_dimen.height/720));
        _scene->addChild(_shadow);
        
        //Draw hunter after shadow
        _hunter.addChildTo(_scene);
        
        //        _trap = TrapController(_assets, _scene->getSize());
        //        _trap.addChildTo(_scene);
        _treasure = TreasureController(_assets, _scene->getSize());
        _treasure.addChildTo(_scene);
        
        _tilemap->addDoorTo(_scene);

        _filterTexture = _assets->get<Texture>("filter");
        _filter = scene2::PolygonNode::allocWithTexture(_filterTexture);
        _filter->setPosition(_scene->getCamera()->getPosition());
        
        _filter->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _filter->setScale(Vec2(_dimen.width/1280,_dimen.height/720));
        _scene->addChild(_filter);

        _timer = 12000;
        _timerLabel = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "2:00", _assets->get<Font>("pixel32"));
        _scene->addChild(_timerLabel);

        _treasureCount = 0;
        _treasureLabel= cugl::scene2::Label::allocWithText(Vec2(200,200), "0 Treasure Collected", _assets->get<Font>("pixel32"));
        _treasureLabel->setColor(cugl::Color4f::YELLOW);
        _scene->addChild(_treasureLabel);
        
        _loseLabel = cugl::scene2::Label::allocWithText(Vec2(800,800), "You Lose!", _assets->get<Font>("pixel32"));
        
        initJoystick();

        _levelLoaded = true;
    }
}

/**
 *
 */
void HGameController::initCamera() {

    Vec3 curr = _scene->getCamera()->getPosition();
    Vec3 next =
        _offset + (Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1));
    _scene->getCamera()->translate(next - curr);
    
    _scene->getCamera()->update();
}

/**
 * Updates camera based on the position of the controlled player
 */
void HGameController::updateCamera(float timestep) {

    Vec2 curr = _scene->getCamera()->getPosition();
    _filter->setPosition(_scene->getCamera()->getPosition());
    _filter->setAnchor(Vec2::ANCHOR_CENTER);
    Vec2 next = _offset
        + ((Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1)));
//    _scene->getCamera()->translate((next - curr) * timestep);
    _scene->getCamera()->translate((next - curr) * 5*timestep);
    
    _timerLabel->setPosition(_scene->getCamera()->getPosition()-Vec2(0,300));
    
    _filter->setPosition(_scene->getCamera()->getPosition());
    _scene->getCamera()->update();
}


void HGameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
}

void HGameController::initJoystick(){
    CULog("joystick initialized");
    // Create outer part of joystick
    PolyFactory _pf = PolyFactory();
    _outerJoystick = scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0,0), 60));
    _outerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _outerJoystick->setScale(1.0f);
    _outerJoystick->setColor(Color4(Vec4(1, 1, 1, 0.25)));
    _scene->addChild(_outerJoystick);
    
    // Create inner part of joystick view
    _innerJoystick = scene2::PolygonNode::allocWithPoly(_pf.makeCircle(Vec2(0,0), 30));
    _innerJoystick->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    _innerJoystick->setScale(1.0f);
    _innerJoystick->setColor(Color4(Vec4(1, 1, 1,0.25)));
    _scene->addChild(_innerJoystick);
    
    // Reposition the joystick components
    _outerJoystick->setPosition(Vec2(20,150));
    _innerJoystick->setPosition(Vec2(20,150));
}

void HGameController::updateJoystick(){
    _outerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(380,250));
    _innerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(380,250));
}

void HGameController::processData(const std::string source, const std::vector<std::byte> &data) {
    if (source == _network->getHost()) {
        _deserializer->receive(data);
        std::vector<float> mes = std::get<std::vector<float>>(_deserializer->read());
        _hunter.addTrap(Vec2(mes[1], mes[2]));
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

void HGameController::updateJoystick(float forward,float rightward){

    _outerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(680,350));
    _innerJoystick->setPosition(_scene->getCamera()->getPosition()-Vec2(680,350)+Vec2(rightward,forward)*100);
}
