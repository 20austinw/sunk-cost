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
#ifndef __HGAME_CONTROLLER_H__
#define __HGAME_CONTROLLER_H__
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
#include "TrapController.hpp"


/**
 * The primary controller for the game logic.
 *
 * In class, we have typically broken up our game logic into scenes,
 * with each such class being a subclass of Scene2. However, as this
 * class shows, this is not necessary. You could have the scene as
 * an attribute of a more general class.
 */
class HGameController{
    
public:
    /**
     * The configuration status
     *
     * This is how the application knows to switch to the next scene.
     */
    enum Status {
        /** Client has not yet entered a room */
        IDLE,
        /** Client is connecting to the host */
        JOIN,
        /** Client is waiting on host to start game */
        WAIT,
        /** Time to start the game */
        START,
        /** Game was aborted; back to main menu */
        ABORT
    };
    
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
    
    int _count;
    /** The scale between the physics world and the screen (SCREEN UNITS / BOX2D WORLD UNITS) */
    float _scale;
    
    int _tileWidth; 
    
    int _tileHeight;
    
    cugl::Vec2 old_place;
    
    // CONTROLLERS are attached directly to the scene (no pointers)
    /** The controller to manage the ship */
    InputController _input;
    
    HunterController _hunter;
    
    SpiritController _spirit;
    
    TrapController _trap;
    
    std::shared_ptr<scene2::PolygonNode> _filter;
    std::shared_ptr<scene2::PolygonNode> _map;
    
    float _timer;
    std::shared_ptr<cugl::scene2::Label> _timerLabel;
    
    
    // MODELS should be shared pointers or a data structure of shared pointers
    

    /** The level model */
    std::shared_ptr<LevelModel> _level;
    
    // VIEW items are going to be individual variables
    // In the future, we will replace this with the scene graph
    /** The backgrounnd image */
    std::shared_ptr<cugl::Texture> _background;
    
    
    std::shared_ptr<cugl::Texture> _filterTexture;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;
    
    bool _levelLoaded;
    
    bool _ishost;
    
    Status _status;
    
#pragma mark External References
private:
    /** The tilemap to procedurally generate */
    std::unique_ptr<TilemapController> _tilemap;
    
#pragma mark Main Methods
public:
    
    HGameController();

    /**
     * Creates the game controller.
     *
     * This constructor will procedurally generate a tilemap immediately
     * on creation.
     *
     * @param displaySize   The display size of the game window
     * @param randoms		Reference to the random number generator
     */
    HGameController(const Size displaySize, const std::shared_ptr<AssetManager>& assets);
    
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
    
    Status getStatus() {
        return _status;
    }
    
    void setHost(bool b) {
        _ishost = b;
    }
//    void updateCamera();
private:
    void checkLevelLoaded();
    
    void generateLevel();

    
};

#endif /* __HGAME_CONTROLLER_H__ */
