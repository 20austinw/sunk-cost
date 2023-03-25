#ifndef SGameController_hpp
#define SGameController_hpp
#include <climits>
#include <random>

#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
using namespace cugl;
#include "HunterController.h"
#include "InputController.h"
#include "LevelModel.h"
#include "PortraitSetController.h"
#include "SpiritController.h"
#include "TilemapController.h"
#include "EndScene.h"
#include "DoorController.hpp"

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
        ABORT
    };

#pragma mark Internal References
  private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

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
    std::shared_ptr<cugl::Texture> _background;
    /** The text with the current health */
    std::shared_ptr<cugl::TextLayout> _text;
    
    /** The network connection (as made by this scene) */
    std::shared_ptr<cugl::net::NetcodeConnection> _network;

    bool _levelLoaded;

    std::shared_ptr<scene2::PolygonNode> _map;
    std::shared_ptr<scene2::PolygonNode> _miniMap;

    Status _status;
    
    bool _ishost;
    
    /** Whether we quit the game */
    bool _quit;
    
    std::shared_ptr<cugl::net::NetcodeSerializer> _serializer;
    
    std::shared_ptr<cugl::net::NetcodeDeserializer> _deserializer;
    
    bool _hunterAdded;
    
    bool _gameStatus = 0;
    
    std::shared_ptr<EndScene> _endScene;
    
    std::vector<std::shared_ptr<DoorController>> _doors;

#pragma mark External References
  private:
    /** The tilemap to procedurally generate */
    std::shared_ptr<TilemapController> _tilemap;

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
    float getZoom();
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
    
    Status getStatus() {
        return _status;
    }
    
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
    void setHost(bool host)  { _ishost = host; }

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

    void displayBattery(float battery, bool state,
                        std::shared_ptr<cugl::SpriteBatch>& batch);

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

    void transmitTrap(std::vector<float> pos);
    
    void initDoors();
    
    void updateDoors();

};

#endif /* SGameController_hpp */
