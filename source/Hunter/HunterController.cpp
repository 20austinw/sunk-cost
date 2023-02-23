//
//  HunterController.cpp
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//

#include "HunterController.h"

#pragma mark Main Functions
/**
 * TODO: Implement Me
 * The constructor should set up the model, view, and camera controller
 */
HunterController::HunterController() {
    _model = std::make_unique<HunterModel>();
    _view = std::make_unique<HunterView>();
    _camera = std::make_unique<CameraController>();
    _hideCool = 0.0;
}

/**
 * TODO: Implement Me
 * This function should
 * (1) get the view from CameraController
 * (2) attach the viewport to the scene
 */
void HunterController::render() {}

/**
 * TODO: Implement Me
 * This function should (for gameplay prototype)
 * (1) detect camera change
 * (2) modify camera to reflect the change
 */
void HunterController::update() {
    _input.readInput();
    
    
}


