//
//  TreasureController.cpp
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#include "TreasureController.hpp"
#include "TreasureModel.hpp"
#include "TreasureView.h"

#pragma mark Main Functions
/**
 * TODO: Implement Me
 * The constructor should set up the model, view, and camera controller
 */

TreasureController::TreasureController(const std::shared_ptr<cugl::AssetManager>& assets, Size screenSize) {
    _model = std::make_shared<TreasureModel>(Vec2(800,500),1000);
    _model->setPosition(Vec2(800,500));
    _view = std::make_unique<TreasureView>(assets, Vec2(800,500), Vec2(1000,1000));
    _screenSize = screenSize;

   
}



//void TreasureController::setViewFrame(){
//    _view->advanceFrame();
//}

void TreasureController::setAsObstacle(std::shared_ptr<cugl::physics2::ObstacleWorld> world){
    world->addObstacle(_model);
}
#pragma mark Getters
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    Vec2 TreasureController::getPosition() {
        return _model->getPosition();
        
    }

    b2Body* TreasureController::getTreasureBody(){
        return _model->getBody();
    }

    std::shared_ptr<scene2::PolygonNode> TreasureController::getNode(){
        return _view->getNode();
    }

#pragma mark Setters

    /**
     * Sets position for this trap
     *
     * @param position trap position
     */
    void TreasureController::setPosition(cugl::Vec2 position) {
        _model->setPosition(position);
    }
    
    /**
     * Sets trigger for this trap
     *
     * @param position trap triggered
     */
    void TreasureController::setTrigger(bool isTriggered) {
        _model->setTrigger(isTriggered);
    }
    
    bool TreasureController::getTrigger() {
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
    void TreasureController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_view->getNode());
    }

/**
 * Removes the TilemapView child from the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to remove the view from
 */
    void TreasureController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_view->getNode());
    }


    bool TreasureController::update() {
        return _model->update();

    }

