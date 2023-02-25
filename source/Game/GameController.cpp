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

bool GameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        return false;
    }
    
    // Check to see if new level loaded yet
    if (_assets->complete()) {
        _level = nullptr;
        
        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
        _level->setAssets(_assets);
        return true;
    }
    // Level is not loaded yet; refuse input
    return false;
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
    if (checkLevelLoaded()) {
        _hunter = std::shared_ptr<HunterController>();
        _spirit = std::shared_ptr<SpiritController>();
    }
    
    _input.readInput();
    _input.update(dt);
    if (_input.didPressReset()) {
        reset();
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
void GameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}



