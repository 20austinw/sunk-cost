//
//  SCResetScene.cpp
//  Sunk
//
//  Created by 任清扬 on 18/4/23.
//

#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "SCResetScene.hpp"

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
bool ResetScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
        _assets->get<scene2::SceneNode>("resett");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    _choice = Choice::NONE;
    _leavebutton = std::dynamic_pointer_cast<scene2::Button>(
        _assets->get<scene2::SceneNode>("resett_leave"));
    _replaybutton = std::dynamic_pointer_cast<scene2::Button>(
        _assets->get<scene2::SceneNode>("resett_replay"));

    // Program the buttons
    _leavebutton->addListener([this](const std::string& name, bool down) {
        if (down) {
            CULog("leave!");
            _choice = Choice::LEAVE;
        }
    });
    _replaybutton->addListener([this](const std::string& name, bool down) {
        CULog("down is %d", down == true);
        if (down) {
            CULog("replay!");
            _choice = Choice::REPLAY;
        }
    });

    addChild(scene);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void ResetScene::dispose() {
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
void ResetScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            CULog("none?????");
            _choice = NONE;
            _leavebutton->activate();
            _replaybutton->activate();
        } else {
            CULog("else?????");
            _leavebutton->deactivate();
            _replaybutton->deactivate();
            // If any were pressed, reset them
            _leavebutton->setDown(false);
            _replaybutton->setDown(false);
        }
    }
}
