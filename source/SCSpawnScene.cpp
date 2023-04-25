//
//  SCSpawnScene.cpp
//  Sunk
//
//  Created by Jiayi Cheng on 4/18/23.
//


#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "SCSpawnScene.hpp"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  720


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
bool SpawnScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
//    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    // Start up the input handler
    _assets = assets;
    _frameNum = 0;
    
    // Acquire the scene built by the asset loader and resize it the scene
    scene =  scene2::SpriteNode::allocWithSheet( _assets->get<Texture>("spawn"), 5, 5, 25);
//    scene->setScale(1);
    scene->setFrame(0);
    scene->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    scene->setPosition(Vec2(0, 0));
    scene->setVisible(true);
    scene->setScale(Vec2(dimen.width/1280,dimen.height/720));


    scene->doLayout(); // Repositions the HUD



    addChild(scene);
    setActive(false);

    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void SpawnScene::dispose() {
        removeAllChildren();

}

void SpawnScene::update(){
    if (_count%6 == 0){
        advanceFrame();
        

        
    }
    _count++;

}

void SpawnScene::advanceFrame() {
    if (_frameNum >= 25) {
        _frameNum = 0;
    }
        scene->setFrame(_frameNum);
        _frameNum++;

}
//void SpawnScene::setActive(bool value) {
//    if (isActive() != value) {
//        Scene2::setActive(value);
//      
//}

void SpawnScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);

    }
}



