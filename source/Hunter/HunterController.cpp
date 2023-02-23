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
    _pos = _model->position;
    _ang  = 0;
    _dang = 0;
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


/**
 * Moves the hunter by the specified amount.
 *
 * Forward is the amount to move forward, while turn is the angle to turn the ship
 * (used for the "banking" animation. This method performs no collision detection.
 * Collisions are resolved afterwards.
 *
 * @param forward    Amount to move forward
 * @param rightward        Amount to move the hunter rightward
 */
void HunterController::move(float forward, float rightward) {
    // Process the hunter thrust.
    if (forward != 0.0f) {
        // Thrust key pressed; increase the hunters velocity.
        float rads = M_PI*_ang/180.0f+M_PI_2;
        Vec2 dir(cosf(rads),sinf(rads));
        _vel += dir * forward;
    }
    if (_vel.length() > 10.f) {
        _vel.normalize();
        _vel = 10.0f*_vel;
    }

    // Move the hunter, updating it.
    // Adjust the angle by the change in angle
    setAngle(_ang+_dang);
    
    // INVARIANT: 0 <= ang < 360
    if (_ang > 360)
        _ang -= 360;
    if (_ang < 0)
        _ang += 360;
    
    // Move the hunter position by the hunter velocity
    _pos += _vel;

}





