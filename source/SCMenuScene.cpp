//
//  SCMenuScene.cpp
//  sunk-mac
//
//  Created by chloe on 3/16/23.
//

#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "SCMenuScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT 720

#pragma mark -
#pragma mark Constructors
/**
 * Initializes the controller contents, and starts the game
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
bool MenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT / dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    // Start up the input handler
    _assets = assets;

    // Acquire the scene built by the asset loader and resize it the scene
    std::shared_ptr<scene2::SceneNode> scene =
        _assets->get<scene2::SceneNode>("menu");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    _choice = Choice::NONE;
    _hostbutton = std::dynamic_pointer_cast<scene2::Button>(
        _assets->get<scene2::SceneNode>("menu_host"));
    _clientbutton = std::dynamic_pointer_cast<scene2::Button>(
        _assets->get<scene2::SceneNode>("menu_join"));

    // Program the buttons
    _hostbutton->addListener([this](const std::string& name, bool down) {
        if (down) {
            _choice = Choice::HOST;
        }
    });
    _clientbutton->addListener([this](const std::string& name, bool down) {
        if (down) {
            _choice = Choice::CLIENT;
        }
    });
    
    _backButton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_back"));
    
    _backButton->addListener([=](const std::string& name, bool down) {
        if (down) {
            _choice = Choice::LOAD;
        }
    });

    addChild(scene);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MenuScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}

/**
 * Sets whether the scene is currently active
 *
 * This method should be used to toggle all the UI elements.  Buttons
 * should be activated when it is made active and deactivated when
 * it is not.
 *
 * @param value whether the scene is currently active
 */
void MenuScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            _choice = NONE;
            _hostbutton->activate();
            _clientbutton->activate();
            _backButton->activate();
        } else {
            _hostbutton->deactivate();
            _clientbutton->deactivate();
            _backButton->deactivate();
            // If any were pressed, reset them
            _hostbutton->setDown(false);
            _clientbutton->setDown(false);
            _backButton->setDown(false);
        }
    }
}
