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
#include "DoorController.hpp"
#include "EndScene.h"
#include "TilemapController.h"
#include "TreasureController.hpp"

/**
 * The primary controller for the game logic.
 *
 * In class, we have typically broken up our game logic into scenes,
 * with each such class being a subclass of Scene2. However, as this
 * class shows, this is not necessary. You could have the scene as
 * an attribute of a more general class.
 */
class HGameController {

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
        ABORT,
        /** Game was ended; back to reset screen */
        RESET
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

    bool _inprogress;

    bool _animates;

    int _indexfromspirit;

    int _count;

    int _countfortimer;

    int _currplayerid;

    bool _timertriggered;

    bool _removedvar;

    int _countEndAnim = 0;

    int _currdoor;

    int _currdoorindex;

    bool _doortrigger;

    bool _stopanim;

    int _frameNum;

    std::vector<int> _doorslocked;

    int _frameNumDoor;

    int _frameNumClose;

    int _tick;

    int _lockcount;

    float _scale;

    bool _counterbool;

    bool _triggered;

    std::shared_ptr<scene2::PolygonNode> _miniMap;
    /** The scale between the physics world and the screen (SCREEN UNITS / BOX2D
     * WORLD UNITS) */

    /** camera need to pan back from exit to hunter */
    bool _shiftback = false;
    /** inidicate camera shift back has finished */
    bool _finishShiftback = false;
    /** The scale between the physics world and the screen (SCREEN UNITS / BOX2D
     * WORLD UNITS) */
    float _scale2;

    int _tileWidth;

    std::shared_ptr<cugl::scene2::PolygonNode> ea;

    int _tileHeight;

    std::shared_ptr<cugl::physics2::PolygonObstacle> _star;

    std::vector<std::shared_ptr<cugl::physics2::PolygonObstacle>>
        _obstacleswall;

    cugl::Vec2 old_place;

    // CONTROLLERS are attached directly to the scene (no pointers)
    /** The controller to manage the ship */
    InputController _input;

    std::shared_ptr<HunterController> _hunter;

    std::unordered_map<int, std::shared_ptr<HunterController>> _hunterSet;

    bool _neverPlayed = true;

    SpiritController _spirit;

    int _frameNumClam;

    //    TrapController _trap;

    TreasureController _treasure;
    std::shared_ptr<scene2::PolygonNode> _outerJoystick;
    std::shared_ptr<scene2::PolygonNode> _innerJoystick;

    std::shared_ptr<scene2::PolygonNode> _hunternow;

    std::shared_ptr<scene2::PolygonNode> _hunterone;

    std::shared_ptr<scene2::PolygonNode> _huntertwo;

    std::shared_ptr<scene2::SpriteNode> _lockhunter;

    std::shared_ptr<scene2::PolygonNode> _filter;
    std::shared_ptr<scene2::PolygonNode> _shadow;
    std::unordered_map<int, std::shared_ptr<scene2::PolygonNode>> _shadowSet;
    std::shared_ptr<scene2::PolygonNode> _map;

    std::shared_ptr<cugl::Texture> _spriteSheet;
    std::shared_ptr<cugl::scene2::SpriteNode> _spriteNode;
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

    float _timerlock;
    std::shared_ptr<cugl::scene2::Label> _timerLabellock;
    int _treasureCount;
    std::shared_ptr<cugl::scene2::Label> _treasureLabel;
    std::shared_ptr<cugl::scene2::Label> _loseLabel;
    bool _didLose = false;

    std::shared_ptr<cugl::scene2::Label> _winLabel;
    bool _didWin;
    std::shared_ptr<cugl::scene2::Label> _finalWinLabel;
    bool _didFinalwin = false;

    /** The theme sound */
    std::shared_ptr<cugl::Sound> _theme;
    /** The sound of tension when time left is less than 1 min */
    std::shared_ptr<cugl::Sound> _tension;
    std::shared_ptr<cugl::Sound> _trapSound;
    std::shared_ptr<cugl::Sound> _treasureSound;

    // MODELS should be shared pointers or a data structure of shared pointers

    /** The level model */
    std::shared_ptr<LevelModel> _level;

    // VIEW items are going to be individual variables
    // In the future, we will replace this with the scene graph
    /** The backgrounnd image */
    std::shared_ptr<cugl::Texture> _background;

    std::shared_ptr<cugl::Texture> _filterTexture;
    std::shared_ptr<cugl::Texture> _shadowTexture;
    std::shared_ptr<cugl::Texture> _exitTexture;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;
    std::vector<std::shared_ptr<DoorController>> _doors;
    std::shared_ptr<cugl::scene2::Button> _unlockbutton;

    Vec2 _exitpos;

    std::shared_ptr<scene2::PolygonNode> _exit;

    bool _levelLoaded = false;

    bool _ishost;

    bool _active;

    int _trapped;

    bool _trappedbool;

    bool _quit;

    bool _move;

    bool _joystickon;

    std::vector<Vec2> _treasurepos;

    std::vector<Vec2> _hunterspun;

    Status _status;

    Vec2 _lastpos;

    std::shared_ptr<cugl::net::NetcodeConnection> _network;

    std::shared_ptr<cugl::net::NetcodeSerializer> _serializer;

    std::shared_ptr<cugl::net::NetcodeDeserializer> _deserializer;

    bool _gameStatus = 0;

    std::shared_ptr<EndScene> _endScene;

#pragma mark External References
  private:
    /** The tilemap to procedurally generate */
    std::unique_ptr<TilemapController> _tilemap;
    std::vector<std::shared_ptr<TileController>> _obstacles;
    std::vector<std::vector<std::shared_ptr<TileController>>> _sortedObstacles;
    std::shared_ptr<scene2::PolygonNode> _obstacleNode;
    std::vector<std::shared_ptr<scene2::SpriteNode>> _candleNodes;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _hunterNodes;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _textureNodes;
    //    std::vector<std::vector<std::shared_ptr<scene2::PolygonNode>>>
    //    _sortedTextures;

    std::vector<Poly2> _obstaclePoly;

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
     * @param randoms        Reference to the random number generator
     */
    HGameController(const Size displaySize,
                    const std::shared_ptr<AssetManager>& assets);

#pragma mark Gameplay Handling
    /**
     * Resets the status of the game so that we can play again.
     */
    void reset();

    float getZoom();
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

    void makePolyObstacle(std::vector<Poly2> input);

    void initCamera();
    void initJoystick();
    void removeJoystick();
    void initLock();
    void updateJoystick(float forward, float rightward, cugl::Vec2 center);
    void updateCamera(float timestep);

    Status getStatus() { return _status; }

    void setHost(bool b) { _ishost = b; }
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

    void initDoors();

    void animatelocks();

    void addlocks(int index);

    /**
     * Returns the network connection (as made by this scene)
     *
     * This value will be reset every time the scene is made active.
     *
     * @return the network connection (as made by this scene)
     */
    void setConnection(
        const std::shared_ptr<cugl::net::NetcodeConnection>& network) {
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
     * Technically, this method does not actually disconnect the network
     * controller. Since the network controller is a smart pointer, it is only
     * fully disconnected when ALL scenes have been disconnected.
     */
    void disconnect() { _network = nullptr; }

    Vec2 getLastPos() { return _lastpos; }

    void setLastPos(Vec2 pos) { _lastpos = pos; }

  private:
    void checkLevelLoaded();

    void generateLevel();

    /**
     * Processes data sent over the network.
     *
     * Once connection is established, all data sent over the network consistes
     * of byte vectors. This function is a call back function to process that
     * data. Note that this function may be called *multiple times* per
     * animation frame, as the messages can come from several sources.
     *
     * Typically this is where players would communicate their names after being
     * connected. In this lab, we only need it to do one thing: communicate that
     * the host has started the game.
     *
     * @param source    The UUID of the sender
     * @param data      The data received
     */
    void processData(const std::string source,
                     const std::vector<std::byte>& data);

    /**
     * Checks that the network connection is still active.
     *
     * Even if you are not sending messages all that often, you need to be
     * calling this method regularly. This method is used to determine the
     * current state of the scene.
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

    void transmitTreasureStolen();

    bool _ismovedonece;

    void transmitUnlockDoor(int idx);

    void transmitTrapTriggered(Vec2 position);

    void initHunter(int hunterId);

    void addFloorTile(int type, int c, int r);

    void addWallTile(int type, int c, int r);

    void addWallUpper(int type, int c, int r);

    void addWallGrime(int type, int c, int r);

    void addWallLower(int type, int c, int r);

    void addFurnitures(int type, int c, int r);

    void addCandles(int type, int c, int r);

    void addPolys();

    void sortNodes();

    void modifyTexture(std::shared_ptr<Texture>& texture, int index, int row,
                       int col);
};

#endif /* __HGAME_CONTROLLER_H__ */
