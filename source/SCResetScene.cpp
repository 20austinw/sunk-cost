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
#include "InputController.h"

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
    _dimen = Application::get()->getDisplaySize();
    _scale = (_dimen.width / 2556 >= _dimen.height / 1194) ? _dimen.width / 2556
                                  : _dimen.height / 1194;
    _inputController = InputController::getInstance();
    _inputController->initListeners();
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(_dimen)) {
        return false;
    }
    
    
    // Start up the input handler
    _assets = assets;

    // Acquire the scene built by the asset loader and resize it the scene
    _scene = cugl::Scene2::alloc(_dimen);
    
    _sheets.push_back(assets->get<Texture>("hunter_lose"));
    _sheets.push_back(assets->get<Texture>("hunter_win"));
    _sheets.push_back(assets->get<Texture>("spirit_lose"));
    _sheets.push_back(assets->get<Texture>("spirit_win"));
    
    for (int i = 0; i<4;i++){
        _frames.push_back(scene2::PolygonNode::allocWithTexture(_sheets[i]));
        _frames[i]->setScale(_scale*1.05);
        _frames[i]->setAnchor(Vec2::ANCHOR_CENTER);
        _frames[i]->setPosition(_dimen/2);
        _frames[i]->setVisible(false);
        _scene->addChild(_frames[i]);
    }
    
    _leave =scene2::PolygonNode::allocWithTexture(assets->get<Texture>("leave_ship"));
    _leave->setAnchor(Vec2::ANCHOR_CENTER);
    _leave->setPosition(Vec2((_dimen.width/2-400),(_dimen.height/2-400)));
    _replay =scene2::PolygonNode::allocWithTexture(assets->get<Texture>("play_again"));
    _replay->setAnchor(Vec2::ANCHOR_CENTER);
    _replay->setPosition(Vec2((_dimen.width/2+400),(_dimen.height/2-400)));
    _scene->addChild(_leave);
    _scene->addChild(_replay);

    
//    std::shared_ptr<scene2::SceneNode> scene =
//        _assets->get<scene2::SceneNode>("resett");
//    scene->setContentSize(dimen);
//    scene->doLayout(); // Repositions the HUD
//    _choice = Choice::NONE;
//    _leavebutton = std::dynamic_pointer_cast<scene2::Button>(
//        _assets->get<scene2::SceneNode>("resett_leave"));
//    _replaybutton = std::dynamic_pointer_cast<scene2::Button>(
//        _assets->get<scene2::SceneNode>("resett_replay"));
//
//    // Program the buttons
//    _leavebutton->addListener([this](const std::string& name, bool down) {
//        if (down) {
//            CULog("leave!");
//            _choice = Choice::LEAVE;
//        }
//    });
//    _replaybutton->addListener([this](const std::string& name, bool down) {
//        CULog("down is %d", down == true);
//        if (down) {
//            CULog("replay!");
//            _choice = Choice::REPLAY;
//        }
//    });
//
//    addChild(scene);
//    setActive(false);

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


void ResetScene::update(float dt){
    _inputController->update(dt);
 
    if(_inputController->didPress() && abs(_inputController->getPosition().x - (_dimen.width/2-400)) <300 &&
       abs(_inputController->getPosition().y -(_dimen.height/2+400)) <
       100){
        CULog("Leave");
        _choice = Choice::LEAVE;
    }
    else if(_inputController->didPress() && abs(_inputController->getPosition().x - (_dimen.width/2+400)) <300 &&
            abs(_inputController->getPosition().y -(_dimen.height/2+400)) <100){
        CULog("replay");
        _choice = Choice::LEAVE;
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
//    if (isActive() != value) {
//        Scene2::setActive(value);
//        if (value) {
//            CULog("none?????");
//            _choice = NONE;
//            _leavebutton->activate();
//            _replaybutton->activate();
//        } else {
//            CULog("else?????");
//            _leavebutton->deactivate();
//            _replaybutton->deactivate();
//            // If any were pressed, reset them
//            _leavebutton->setDown(false);
//            _replaybutton->setDown(false);
//        }
//    }
}

void ResetScene::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}

void ResetScene::setScene(int value) {
    CULog("setScene");
    _frames[value]->setVisible(true);
//    if (isActive() != value) {
//        Scene2::setActive(value);
//        if (value) {
//            CULog("none?????");
//            _choice = NONE;
//            _leavebutton->activate();
//            _replaybutton->activate();
//        } else {
//            CULog("else?????");
//            _leavebutton->deactivate();
//            _replaybutton->deactivate();
//            // If any were pressed, reset them
//            _leavebutton->setDown(false);
//            _replaybutton->setDown(false);
//        }
//    }
}
