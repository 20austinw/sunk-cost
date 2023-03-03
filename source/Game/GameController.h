//
//  GameController.h
//  Sunk Cost
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors of Referenced File: Walker White and Gonzalo Gonzalez
//  Version: 2/22/23
//
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__
#include <random>
#include <climits>

#include <cugl/cugl.h>
#include <vector>
#include <unordered_set>
using namespace cugl;
#include "TilemapController.h"
#include "InputController.h"
#include "LevelModel.h"
#include "HunterController.h"
#include "SpiritController.h"
#include "PortraitSetController.h"


/**
 * The primary controller for the game logic.
 *
 * In class, we have typically broken up our game logic into scenes,
 * with each such class being a subclass of Scene2. However, as this
 * class shows, this is not necessary. You could have the scene as
 * an attribute of a more general class.
 */
class GameController{
    
#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** The amount to move the world node by to center it in the scene */
    cugl::Vec3 _offset;
    
    /** The locked size of the display. */
    cugl::Size _dimen;
    
    /** The scale between the physics world and the screen (SCREEN UNITS / BOX2D WORLD UNITS) */
    float _scale;
    
    // CONTROLLERS are attached directly to the scene (no pointers)
    /** The controller to manage the ship */
    InputController _input;
    
    HunterController _hunter;
    
    SpiritController _spirit;
    
    PortraitSetController _portraits;
    
    // MODELS should be shared pointers or a data structure of shared pointers
    

    /** The level model */
    std::shared_ptr<LevelModel> _level;
    
    // VIEW items are going to be individual variables
    // In the future, we will replace this with the scene graph
    /** The backgrounnd image */
    std::shared_ptr<cugl::Texture> _background;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;
    
    bool _levelLoaded;
    
#pragma mark External References
private:
    /** The tilemap to procedurally generate */
    std::unique_ptr<TilemapController> _tilemap;
    
#pragma mark Main Methods
public:
    
    GameController();

    /**
     * Creates the game controller.
     *
     * This constructor will procedurally generate a tilemap immediately
     * on creation.
     *
     * @param displaySize   The display size of the game window
     * @param randoms		Reference to the random number generator
     */
    GameController(const Size displaySize, const std::shared_ptr<AssetManager>& assets);
    
#pragma mark Gameplay Handling
    /**
     * Resets the status of the game so that we can play again.
     */
    void reset();
    
    /**
     * Responds to the keyboard commands.
     *
     * This method allows us to regenerate the procedurally generated tilemap
     * upon command.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);
    
    /**
     * Renders the game elements using the`batch.
     *
     * @param batch The SpriteBatch used to render this scene
     */
    void render(std::shared_ptr<SpriteBatch>& batch);
    
    void initCamera();
    void updateCamera(float timestep);
    
//    void updateCamera();
private:
    void checkLevelLoaded();
    
    void generateLevel();

    
};

#endif /* __GAME_CONTROLLER_H__ */
