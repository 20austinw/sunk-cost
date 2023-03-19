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
#include <climits>
#include <random>

#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
using namespace cugl;
#include "HunterController.h"
#include "InputController.h"
#include "LevelModel.h"
#include "SpiritController.h"
//#include "TrapController.hpp"
#include "CollisionController.hpp"
#include "TreasureController.hpp"
#include "TilemapController.h"

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
    
    std::shared_ptr<PortraitSetController> _portraits;

    
    int _count;
    /** The scale between the physics world and the screen (SCREEN UNITS / BOX2D
     * WORLD UNITS) */
    float _scale;
    
    int _tileWidth;
    
    int _tileHeight;
    
    cugl::Vec2 old_place;
    
    // CONTROLLERS are attached directly to the scene (no pointers)
    /** The controller to manage the ship */
    InputController _input;
    
    HunterController _hunter;
    
    SpiritController _spirit;
    
//    TrapController _trap;
    
    TreasureController _treasure;
    std::shared_ptr<scene2::PolygonNode>_outerJoystick;
    std::shared_ptr<scene2::PolygonNode>_innerJoystick;
    
    std::shared_ptr<scene2::PolygonNode> _filter;
    std::shared_ptr<scene2::PolygonNode> _shadow;
    std::shared_ptr<scene2::PolygonNode> _map;
    /** The Box2D world */
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    /** The Collision Controller instance */
    CollisionController _collision;
    /** Reference to the physics node of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _worldnode;
    /** Reference to the debug node of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _debugnode;
    float _timer;
    std::shared_ptr<cugl::scene2::Label> _timerLabel;
    int _treasureCount;
    std::shared_ptr<cugl::scene2::Label> _treasureLabel;
    std::shared_ptr<cugl::scene2::Label> _loseLabel;
    bool _didLose;
    
    // MODELS should be shared pointers or a data structure of shared pointers
    
    /** The level model */
    std::shared_ptr<LevelModel> _level;
    
    // VIEW items are going to be individual variables
    // In the future, we will replace this with the scene graph
    /** The backgrounnd image */
    std::shared_ptr<cugl::Texture> _background;
    
    std::shared_ptr<cugl::Texture> _filterTexture;
    std::shared_ptr<cugl::Texture> _shadowTexture;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;
    
    bool _levelLoaded;
    
    bool _ishost;
    
    bool _quit;
    
    Status _status;
    
    std::shared_ptr<cugl::net::NetcodeConnection> _network;
    
    std::shared_ptr<cugl::net::NetcodeSerializer> _serializer;
    
    std::shared_ptr<cugl::net::NetcodeDeserializer> _deserializer;
    
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
    HGameController(const Size displaySize,
                    const std::shared_ptr<AssetManager>& assets);
    
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
    void initJoystick();
    void updateJoystick();
    void updateCamera(float timestep);
    
    Status getStatus() {
        return _status;
    }
    
    void setHost(bool b) {
        _ishost = b;
    }
    //    void updateCamera();
    
    /**
     * Returns the network connection (as made by this scene)
     *
     * This value will be reset every time the scene is made active.
     *
     * @return the network connection (as made by this scene)
     */
    std::shared_ptr<cugl::net::NetcodeConnection> getConnection() const {
        return _network;
    }
    
    /**
     * Returns the network connection (as made by this scene)
     *
     * This value will be reset every time the scene is made active.
     *
     * @return the network connection (as made by this scene)
     */
    void setConnection(const std::shared_ptr<cugl::net::NetcodeConnection>& network) {
        _network = network;
    }
    
    
    /**
     * Returns true if the player quits the game.
     *
     * @return true if the player quits the game.
     */
    bool didQuit() const { return _quit; }
 
    /**
     * Disconnects this scene from the network controller.
     *
     * Technically, this method does not actually disconnect the network controller.
     * Since the network controller is a smart pointer, it is only fully disconnected
     * when ALL scenes have been disconnected.
     */
    void disconnect() { _network = nullptr; }
    
private:
    void checkLevelLoaded();
    
    void generateLevel();
    
    /**
     * Processes data sent over the network.
     *
     * Once connection is established, all data sent over the network consistes of
     * byte vectors. This function is a call back function to process that data.
     * Note that this function may be called *multiple times* per animation frame,
     * as the messages can come from several sources.
     *
     * Typically this is where players would communicate their names after being
     * connected. In this lab, we only need it to do one thing: communicate that
     * the host has started the game.
     *
     * @param source    The UUID of the sender
     * @param data      The data received
     */
    void processData(const std::string source, const std::vector<std::byte>& data);

    /**
     * Checks that the network connection is still active.
     *
     * Even if you are not sending messages all that often, you need to be calling
     * this method regularly. This method is used to determine the current state
     * of the scene.
     *
     * @return true if the network connection is still active.
     */
    bool checkConnection();

    /**
     * Transmits a hunter position change to all other devices.
     *
     * @param position The hunter's new position
     */
    void transmitPos(std::vector<float> position);
};

#endif /* __HGAME_CONTROLLER_H__ */
