//
//  TrapController.cpp
//  Sunk
//
//  Created by 任清扬 on 14/3/23.
//

#include "TrapController.hpp"
#include "TrapModel.hpp"
#include "TrapViewH.h"

#pragma mark Main Functions
/**
 * TODO: Implement Me
 * The constructor should set up the model, view, and camera controller
 */

TrapController::TrapController(const std::shared_ptr<cugl::AssetManager>& assets, Size screenSize) {
    CULog("Called!");
    _model = std::make_unique<TrapModel>(Vec2(500,600),1000);
    _view = std::make_unique<TrapViewH>(assets, Vec2(500,600), Vec2(40,40));
    _screenSize = screenSize;

   
}



void TrapController::setViewFrame(){
    _view->advanceFrame();
}

#pragma mark Getters
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    Vec2 TrapController::getPosition() {
        return _model->getPosition();
        
    }

#pragma mark Setters

    /**
     * Sets position for this trap
     *
     * @param position trap position
     */
    void TrapController::setPosition(cugl::Vec2 position) {
        _model->setPosition(position);
    }
    
    /**
     * Sets trigger for this trap
     *
     * @param position trap triggered
     */
    void TrapController::setTrigger(bool isTriggered) {
        _model->setTrigger(isTriggered);
    }
    
    bool TrapController::getTrigger() {
        return _model->getTrigger();
    }


#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to add the view to
 */
    void TrapController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_view->getNode());
    }

/**
 * Removes the TilemapView child from the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to remove the view from
 */
    void TrapController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_view->getNode());
    }


    bool TrapController::update() {
        return _model->update();

    }
