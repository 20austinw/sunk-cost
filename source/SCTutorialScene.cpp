//
//  SCTutorialScene.cpp
//  Sunk
//
//  Created by 任清扬 on 17/5/23.
//


#include <cugl/cugl.h>
#include <iostream>
#include <sstream>
#include "SCTutorialScene.hpp"
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
bool TutorialScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    _dimen = Application::get()->getDisplaySize();
    _inputController = InputController::getInstance();
    _inputController->initListeners();
//    _dimen *= SCENE_HEIGHT / _dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(_dimen)) {
        return false;
    }

    // Start up the input handler
    _assets = assets;
    _choice = Choice::NONE;
//    // Acquire the scene built by the asset loader and resize it the scene
//    std::shared_ptr<scene2::SceneNode> scene =
//        _assets->get<scene2::SceneNode>("menu");
//    scene->setContentSize(_dimen);
//    scene->doLayout(); // Repositions the HUD
//    _choice = Choice::NONE;
//    _hostbutton = std::dynamic_pointer_cast<scene2::Button>(
//        _assets->get<scene2::SceneNode>("menu_host"));
//    _clientbutton = std::dynamic_pointer_cast<scene2::Button>(
//        _assets->get<scene2::SceneNode>("menu_join"));
//
//    // Program the buttons
//    _hostbutton->addListener([this](const std::string& name, bool down) {
//        if (down) {
//            _choice = Choice::HOST;
//        }
//    });
//    _clientbutton->addListener([this](const std::string& name, bool down) {
//        if (down) {
//            _choice = Choice::CLIENT;
//        }
//    });

    // Acquire the scene built by the asset loader and resize it the scene
    _scene = cugl::Scene2::alloc(_dimen);
    _frame0 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial0"));
    _frame0->setScale(0.7);
    _frame0->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame0->setPosition(Vec2(200,0));
    _frame0->setVisible(true);
    _scene->addChild(_frame0);
    
    _page = 0;
    
    _frame1 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial1"));
    _frame1->setScale(0.7);
    _frame1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame1->setPosition(Vec2(200,0));
    _frame1->setVisible(false);
    _scene->addChild(_frame1);
    
    _frame2 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial2"));
    _frame2->setScale(0.7);
    _frame2->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame2->setPosition(Vec2(200,0));
    _frame2->setVisible(false);
    _scene->addChild(_frame2);
    
    _frame3 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial3"));
    _frame3->setScale(0.7);
    _frame3->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame3->setPosition(Vec2(200,0));
    _frame3->setVisible(false);
    _scene->addChild(_frame3);
    
    _frame4 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial4"));
    _frame4->setScale(0.7);
    _frame4->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame4->setPosition(Vec2(200,0));
    _frame4->setVisible(false);
    _scene->addChild(_frame4);
    
    _frame5 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial5"));
    _frame5->setScale(0.7);
    _frame5->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame5->setPosition(Vec2(200,0));
    _frame5->setVisible(false);
    _scene->addChild(_frame5);
    
    _frame6 = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("tutorial6"));
    _frame6->setScale(0.7);
    _frame6->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _frame6->setPosition(Vec2(200,0));
    _frame6->setVisible(false);
    _scene->addChild(_frame6);
    
    _frames.push_back(_frame0);
    _frames.push_back(_frame1);
    _frames.push_back(_frame2);
    _frames.push_back(_frame3);
    _frames.push_back(_frame4);
    _frames.push_back(_frame5);
    _frames.push_back(_frame6);

    
    std::shared_ptr<scene2::PolygonNode> back = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("back_button"));
    back->setAnchor(Vec2::ANCHOR_CENTER);
    back->setScale(0.7);
    back->setPosition(200,_dimen.height-100);
    _scene->addChild(back);
    
    std::shared_ptr<scene2::PolygonNode> previous = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("left"));
    previous->setAnchor(Vec2::ANCHOR_CENTER);
    previous->setScale(0.8);
    previous->setPosition(100,_dimen.height/2);
    _scene->addChild(previous);
    
    std::shared_ptr<scene2::PolygonNode> next = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("right"));
    next->setAnchor(Vec2::ANCHOR_CENTER);
    next->setScale(1);
    next->setPosition(_dimen.width-100,_dimen.height/2);
    _scene->addChild(next);
    
    setActive(false);
    return true;
}

void TutorialScene::update(float dt){
    _inputController->update(dt);
    
    if(_inputController->didPress() && abs(_inputController->getPosition().x - 100) <100 &&
       abs(_inputController->getPosition().y -(_dimen.height/2)) <
       100){
        CULog("PREVIOUS PAGE");
        if(_page>0){
            _frames[_page]->setVisible(false);
            _page = _page - 1;
        }
    }
    else if(_inputController->didPress() && abs(_inputController->getPosition().x - (_dimen.width-100)) <100 &&
            abs(_inputController->getPosition().y -_dimen.height/2) <100){
        CULog("NEXT PAGE");
        if(_page<6){
            _frames[_page]->setVisible(false);
            _page = _page + 1;
        }
    }
    else if(_inputController->didPress() && abs(_inputController->getPosition().x - 200) <100 &&
            abs(_inputController->getPosition().y -100) <100){
        CULog("QUITTTT ");
        _choice = Choice::BACK;
    }
    _frames[_page]->setVisible(true);
}
/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void TutorialScene::dispose() {
    if (_active) {
        _scene->removeAllChildren();
        _active = false;
    }
}

void TutorialScene::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
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
void TutorialScene::setActive(bool value) {
//    if (isActive() != value) {
//        Scene2::setActive(value);
//        if (value) {
//            _choice = NONE;
//            _hostbutton->activate();
//            _clientbutton->activate();
//        } else {
//            _hostbutton->deactivate();
//            _clientbutton->deactivate();
//            // If any were pressed, reset them
//            _hostbutton->setDown(false);
//            _clientbutton->setDown(false);
//        }
//    }
}
