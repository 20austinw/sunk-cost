//
//  HunterController.h
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/2023
//

#ifndef _HUNTER_CONTROLLER_H
#define _HUNTER_CONTROLLER_H

#include <cugl/cugl.h>
#include "HunterModel.h"
#include "HunterView.h"
#include "../Camera/CameraController.h"
#include "../Input/InputController.h"

using namespace cugl;

class HunterController {
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<HunterModel> _model;
    /** View reference */
    std::unique_ptr<HunterView> _view;
    
#pragma mark External References
private:
    /** Camera reference */
    std::unique_ptr<CameraController> _camera;
    /** The controller to manage the ship */
    InputController _input;
    
#pragma mark Constants
private:
    /** The preset cooltime for hiding */
    float _hideCool;
    
#pragma mark Main Functions
public:
    /**
     * TODO: Implement Me
     * The constructor should set up the model, view, and camera controller
     */
    HunterController();
    
    /**
     * Gets the viewpoint for the hunter's camera
     *
     * @return rectangle that should be drawn
     */
    const Rect getCameraView() {
        return _camera->getViewPort();
    }
    
    /**
     * TODO: Implement Me
     * This function should
     * (1) get the view from CameraController
     * (2) attach the viewport to the scene
     */
    void render();
    
    /**
     * TODO: Implement Me
     * This function should (for gameplay prototype)
     * (1) detect camera change
     * (2) modify camera to reflect the change
     */
    void update();
    
#pragma mark Setters
public:
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    void updatePosition(cugl::Vec2 position) {
        _model->setPosition(position);
        _view->setPosition(position);
    }
    
    /**
     * Updates remaining hide cooldown time for this hunter
     *
     * @param hideCool the new remaining cooltime
     */
    void updateHideCooldown(float hideCool) {
        _model->setHideCooldown(hideCool);
    }
};
#endif /* _HUNTER_CONTROLLER_H__ */
