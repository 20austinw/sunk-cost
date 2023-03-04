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
#include "GameController.h"
#include "LevelConstants.h"

#pragma mark Main Methods
GameController::GameController(){
    _hunter = HunterController();
    
    // Initialize SpiritController
    _spirit = SpiritController();
    
    // Initialize PortraitSetController
    _portraits = PortraitSetController();
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
GameController::GameController(const Size displaySize, const std::shared_ptr<cugl::AssetManager>& assets):
_scene(cugl::Scene2::alloc(displaySize)),
_assets(assets){
    /// Initialize the tilemap and add it to the scene
//    SCENE_WIDTH = 1024;
//    SCENE_HEIGHT = 576;
    _dimen = Application::get()->getDisplaySize();
//    _offset = Vec3((_dimen.width)/2.0f,(_dimen.height)/2.0f,50);
    _offset = Vec3(0,0,50);
    _tilemap = std::make_unique<TilemapController>();

    _tilemap->addChildTo(_scene);

    _hunter = HunterController(assets);
    
    // Initialize SpiritController
    _spirit = SpiritController();
    
    // Initialize PortraitSetController
    _portraits = PortraitSetController();
    
    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }
    
    initCamera();
}

#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void GameController::reset() {
    CULog("reset");
}

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void GameController::update(float dt) {
    if (!_levelLoaded) {
        checkLevelLoaded();
    }
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    if (inputController->didPressReset()) {
        reset();
    }
    if(inputController->isKeyPressed(KeyCode::NUM_1)) {
        CULog("NUM_1");
    }
    if(inputController->isKeyPressed(KeyCode::NUM_2)) {
        CULog("NUM_2");
    }
    if(inputController->isKeyPressed(KeyCode::NUM_3)) {
        CULog("NUM_3");
    }
    if(inputController->isKeyPressed(KeyCode::NUM_4)) {
        CULog("NUM_4");
    }
    
    // Will crash the program because the constructor doesn't set up the model/view yet (delete this comment later)

//    _hunter.update();
//    _spirit.update();
    
  std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
    int posx;
    int posy;
    Vec3 currPos = (_hunter.getPosition());
    posx =(int) (currPos.x)/45;
    posy=(int)((currPos.y))/45;

    
//    if((posx<17 && posx>0) && (posy<12 && posy>0)){
//        _hunter.update();
//    }
  //  _hunter.update();
    
    
    
    if (tiles[posy][posx]!= "black"){
        _hunter.update();
    }
    else{
        _hunter.updatePosition(Vec2(400,300));
    }

    
//    if(_tilemap->isTileTraversable(_hunter.getPosition())){
//            _hunter.updatePosition(_level->getPlayerPosition());
//        _hunter.update();
//    }
    updateCamera(dt);

    // TODO: update direction index for portraits on spirit control
//    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int index#>)
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
void GameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}


void GameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }


    // Check to see if new level loaded yet
    if (!_levelLoaded && _assets->complete()) {
        _level = nullptr;
        
        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
        _level->setAssets(_assets);
        

        // Initialize SpiritController
        _spirit = SpiritController();
        
        // Initialize PortraitSetController
        _portraits = PortraitSetController();
        
        // TODO: implement direction and direction limits
        for(int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits.addPortrait(i + 1, _level->getPortaits()[i], Vec3(0, 0, -1), Vec2::ZERO);
        }
        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(45, 45));
        
        _filterTexture = _assets->get<Texture>("filter");
        _filter = scene2::PolygonNode::allocWithTexture(_filterTexture);
        _filter->setPosition(_scene->getCamera()->getPosition());
      
        _filter->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _filter->setPolygon(Rect( Vec2::ZERO, Vec2(1280,720)));
        _scene->addChild(_filter);
        
        for (int i = 0; i < tiles.size() * tiles[0].size(); ++i){
            int c = i%tiles[0].size();
            int r = i/tiles[0].size();
            
            if (tiles[r][c] == "red") {
                _tilemap->addTile(c, r, Color4::RED, true, _assets->get<Texture>("red"));
                
            } else if (tiles[r][c] == "black") {
                _tilemap->addTile(c, r, Color4::BLACK, false, _assets->get<Texture>("black"));
            } else if (tiles[r][c] == "green") {
                _tilemap->addTile(c, r, Color4::GREEN, true, _assets->get<Texture>("green"));
            } else if (tiles[r][c] == "blue") {
                _tilemap->addTile(c, r, Color4::BLUE, true, _assets->get<Texture>("blue"));
            }
        }
        
        // Initialize HunterController
        _hunter = HunterController(_assets);
        _hunter.addChildTo(_scene);

        
        
        
        _levelLoaded = true;
    }
}

/**
 *
 */
void GameController::initCamera() {
    
    Vec3 curr = _scene->getCamera()->getPosition();
    Vec3 next = _offset
    + (Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1));
    _scene->getCamera()->translate(next - curr);
    
    
//    _scene->getCamera()->lookAt(Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1));
    _scene->getCamera()->update();
    
}

/**
 * Updates camera based on the position of the controlled player
 */
void GameController::updateCamera(float timestep) {
    
    Vec2 curr = _scene->getCamera()->getPosition();
    _filter->setPosition(_scene->getCamera()->getPosition());
    _filter->setAnchor(Vec2::ANCHOR_CENTER);
    Vec2 next = _offset
        + ((Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1)));
    _scene->getCamera()->translate((next - curr) * timestep);
    _scene->getCamera()->update();
   
}

void GameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
    
}

