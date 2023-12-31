#ifndef SGameController_hpp
#define SGameController_hpp
#include <climits>
#include <random>

#include "Button.h"
#include "DoorController.hpp"
#include "HunterController.h"
#include "InputController.h"
#include "LevelModel.h"
#include "Minimap.h"
#include "PortraitSetController.h"
#include "SpiritController.h"
#include "TileController.h"
#include "TilemapController.h"
#include "TrapController.hpp"
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>

using namespace cugl;

/**
 * The primary controller for the game logic.
 *
 * In class, we have typically broken up our game logic into scenes,
 * with each such class being a subclass of Scene2. However, as this
 * class shows, this is not necessary. You could have the scene as
 * an attribute of a more general class.
 */
class SGameController {
  public:
    /**
     * The configuration status
     *
     * This is how the application knows to switch to the next scene.
     */
    enum Status {
        /** Host is waiting on a connection */
        WAIT,
        /** Host is waiting on all players to join */
        IDLE,
        /** Time to start the game */
        START,
        /** Game was aborted; back to main menu */
        ABORT,
        RESET
    };

#pragma mark Internal References
  private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    // background and map floor(tilemap)
    std::shared_ptr<cugl::scene2::PolygonNode> _firstLayer;

    // furnitures, walls, doors, traps
    std::shared_ptr<cugl::scene2::PolygonNode> _secondLayer;

    // block screen
    std::shared_ptr<cugl::scene2::PolygonNode> _thirdLayer;

    // Shadow
    std::shared_ptr<cugl::scene2::PolygonNode> _fourthLayer;

    // lock, trap, eyeball, kill buttons
    std::shared_ptr<cugl::scene2::PolygonNode> _fifthLayer;

    // Battery, timer
    std::shared_ptr<cugl::scene2::PolygonNode> _sixthLayer;
    
    std::shared_ptr<cugl::scene2::SpriteNode> _spawnNode;

    // CONTROLLERS are attached directly to the scene (no pointers)
    /** The controller to manage the ship */
    InputController _input;

    HunterController _hunter;

    SpiritController _spirit;

    std::shared_ptr<PortraitSetController> _portraits;

    // MODELS should be shared pointers or a data structure of shared pointers

    /** The level model */
    std::shared_ptr<LevelModel> _level;

    // VIEW items are going to be individual variables
    // In the future, we will replace this with the scene graph
    /** The backgrounnd image */
    std::shared_ptr<cugl::scene2::PolygonNode> _background;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;

    /** The network connection (as made by this scene) */
    std::shared_ptr<cugl::net::NetcodeConnection> _network;

    bool _levelLoaded = false;

    std::shared_ptr<scene2::PolygonNode> _map;

    Status _status;

    bool _ishost;

    /** Whether we quit the game */
    bool _quit;
    bool _blocked;

    bool _selection;

    std::shared_ptr<cugl::net::NetcodeSerializer> _serializer;

    std::shared_ptr<cugl::net::NetcodeDeserializer> _deserializer;

    bool _hunterAdded;

    int _gameStatus = 0;
    
    int _win = 0;


    std::vector<std::shared_ptr<DoorController>> _doors;
    std::shared_ptr<Font> _font;
    float _textHeight = 100;
    float _timerScale;
    std::shared_ptr<cugl::scene2::Label> _timerLabel;
    int _timeLeft = 120 * 60;

    /** If hunter trigger the trap */
    bool _trapTriggered;
    /** If hunter unlock a door */
    bool _doorUnlocked;

    bool _treasureStolen;

    // place holder for treasure alert
    std::shared_ptr<cugl::scene2::Label> _alertLabel;

    int _alertTimer;

    int _doorToUnlock;

    Vec2 _trapPos;

    float _hunterYPos;

    float _hunterXPos;

    bool _spawn;

    int _ticks;

    std::shared_ptr<Button> _viewButton;

    float _buttonHeight;

    bool _selectionPhase;

    /** The theme sound */
    std::shared_ptr<cugl::Sound> _theme;
    /** The sound of tension when time left is less than 1 min */
    std::shared_ptr<cugl::Sound> _tension;
    std::shared_ptr<cugl::Sound> _trapSound;
    std::shared_ptr<cugl::Sound> _treasureSound;
    std::shared_ptr<cugl::Sound> _killSound;
    std::shared_ptr<cugl::Sound> _damageSound;
    bool _neverPlayed = true;
    std::vector<std::shared_ptr<cugl::scene2::PolygonNode>> _indicators;
    std::vector<std::shared_ptr<cugl::scene2::PolygonNode>> _shadows;
    std::vector<std::shared_ptr<cugl::scene2::PolygonNode>> _grayshadows;

#pragma mark External References
  private:
    /** The tilemap to procedurally generate */
    std::shared_ptr<TilemapController> _tilemap;
    std::vector<std::shared_ptr<TileController>> _obstacles;
    std::vector<std::vector<std::shared_ptr<TileController>>> _sortedObstacles;
    std::vector<std::shared_ptr<TileController>> _detections;
    std::shared_ptr<scene2::PolygonNode> _obstacleNode;
    std::vector<std::shared_ptr<TileController>> _holes;
    std::vector<std::shared_ptr<TileController>> _carpets;

    std::vector<std::shared_ptr<scene2::SpriteNode>> _candleNodes;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _hunterNodes;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _doorNodes;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _portraitNodes;

#pragma mark Main Methods
  public:
    SGameController(){};
    /**
     * Creates the game controller.
     *
     * This constructor will procedurally generate a tilemap immediately
     * on creation.
     *
     * @param displaySize   The display size of the game window
     * @param randoms        Reference to the random number generator
     */
    SGameController(const Size displaySize,
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

    Status getStatus() { return _status; }

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
    void setConnection(
        const std::shared_ptr<cugl::net::NetcodeConnection>& network) {
        _network = network;
    }

    /**
     * Returns true if the player is host.
     *
     * We may need to have gameplay specific code for host.
     *
     * @return true if the player is host.
     */
    bool isHost() const { return _ishost; }

    /**
     * Sets whether the player is host.
     *
     * We may need to have gameplay specific code for host.
     *
     * @param host  Whether the player is host.
     */
    void setHost(bool host) { _ishost = host; }

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
    
    bool getWin(){
        if (_gameStatus == 1){
            return _gameStatus;
        }else{
            return 0;
        }
    }

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

    void transmitTrap(std::vector<float> pos);

    void transmitActiveCamIndex(int i);

    void transmitKill();

    void transmitSpiritWin();

    void transmitHunterWin();

    void initDoors();

    void updateDoors();

    void transmitLockedDoor(int i);
    
    void transmitTimer(int i);

    void addFloorTile(int type, int c, int r);

    void addWallTile(int type, int c, int r);

    void addWallUpper(int type, int c, int r);

    void addWallGrime(int type, int c, int r);

    void addWallLower(int type, int c, int r);

    void addFurnitures(int type, int c, int r);

    void addCandles(int type, int c, int r);

    void sortNodes();

    std::shared_ptr<Texture> getTexture(int type);

    void addDetails(int type, int c, int r);

    void modifyTexture(std::shared_ptr<Texture>& texture, int index);

    float getYPos(int type, float pos, std::shared_ptr<TileController>& tile, int c, int r);
    
    void beginDetectTrap();
    
    void updateDetectTrap();
    
    void endDetectTrap();
    
    void updateSpawn();
};

#endif /* SGameController_hpp */
