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

    bool _levelLoaded;

    bool _prevState;

    std::shared_ptr<scene2::PolygonNode> _map;

    Status _status;
    
    bool _ishost;

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
    
    void setHost(bool b) {
        _ishost = b;
    }

  private:
    void checkLevelLoaded();

    void generateLevel();

    void displayBattery(float battery, bool state,
                        std::shared_ptr<cugl::SpriteBatch>& batch);
};

#endif /* SGameController_hpp */
