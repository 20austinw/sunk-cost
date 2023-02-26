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
    _tilemap = std::make_unique<TilemapController>();

    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }
}

#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void GameController::reset() {
    CULog("reset");
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
        
        // Initialize HunterController
        _hunter = HunterController();
        _hunter.updatePosition(_level->getPlayerPosition());
        
        // Initialize SpiritController
        _spirit = SpiritController();
        
        // Initialize PortraitSetController
        _portraits = PortraitSetController();
        
        // TODO: implement direction and direction limits
        for(int i = 0; i < _level->getPortaits().size(); i++) {
            _portraits.addPortrait(i + 1, _level->getPortaits()[i], Vec2::ZERO, Vec2::ZERO);
        }
        
        _levelLoaded = true;
    }
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
    
    _input.readInput();
    _input.update(dt);
    if (_input.didPressReset()) {
        reset();
    }
    
    // Will crash the program because the constructor doesn't set up the model/view yet (delete this comment later)
    _hunter.update();
    _spirit.update();
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



