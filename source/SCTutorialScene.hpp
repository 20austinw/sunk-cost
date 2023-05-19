//
//  SCTutorialScene.hpp
//  Sunk
//
//  Created by 任清扬 on 17/5/23.
//

#ifndef SCTutorialScene_hpp
#define SCTutorialScene_hpp
#include "InputController.h"
/**
 * This class presents the menu to the player.
 */
class TutorialScene : public cugl::Scene2 {
  public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        /** User has not yet made a choice */
        NONE,
        /** User wants to go back to loading scene */
        BACK

    };

  protected:
    /** The asset manager for this scene. */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** The menu button for hosting a game */
    std::shared_ptr<cugl::scene2::Button> _hostbutton;
    /** The menu button for joining a game */
    std::shared_ptr<cugl::scene2::Button> _clientbutton;
    /** The player menu choice */
    Choice _choice;
    std::shared_ptr<cugl::Scene2> _scene;
    std::shared_ptr<InputController> _inputController;
    Size _dimen;
    std::shared_ptr<scene2::PolygonNode> _frame0;
    std::shared_ptr<scene2::PolygonNode> _frame1;
    std::shared_ptr<scene2::PolygonNode> _frame2;
    std::shared_ptr<scene2::PolygonNode> _frame3;
    std::shared_ptr<scene2::PolygonNode> _frame4;
    std::shared_ptr<scene2::PolygonNode> _frame5;
    std::shared_ptr<scene2::PolygonNode> _frame6;
    std::vector<std::shared_ptr<scene2::PolygonNode>> _frames;
    int _page=0;

  public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new  menu scene with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    TutorialScene() : cugl::Scene2() {}

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~TutorialScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose() override;
    
    void update(float dt);
    
    /**
     * Renders the game elements using the`batch.
     *
     * @param batch The SpriteBatch used to render this scene
     */
    void render(std::shared_ptr<SpriteBatch>& batch);

    /**
     * Initializes the controller contents.
     *
     * In previous labs, this method "started" the scene.  But in this
     * case, we only use to initialize the scene user interface.  We
     * do not activate the user interface yet, as an active user
     * interface will still receive input EVEN WHEN IT IS HIDDEN.
     *
     * That is why we have the method {@link #setActive}.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    /**
     * Sets whether the scene is currently active
     *
     * This method should be used to toggle all the UI elements.  Buttons
     * should be activated when it is made active and deactivated when
     * it is not.
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /**
     * Returns the user's menu choice.
     *
     * This will return NONE if the user had no yet made a choice.
     *
     * @return the user's menu choice.
     */
    Choice getChoice() const { return _choice; }
};

#endif /* SCTutorialScene_hpp */

