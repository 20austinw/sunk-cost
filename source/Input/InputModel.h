//
//  InputModel.h
//  Sunk Cost
//
//  This module represents the current input state. Note that this application
//  does not use the mouse at all. This code is here to simply show you
//  how you might want to organize your state.
//
//  Author of Referenced file: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef __INPUT_MODEL_H__
#define __INPUT_MODEL_H__
#include <cugl/cugl.h>

using namespace cugl;

/**
 * A class representing the current input state
 */
class InputModel {

#pragma mark State
  public:
    /** Whether the mouse was first clicked during a callback */
    bool didClickMouse;
    /** Whether the mouse was first clicked during this frame */
    bool isMouseClicked;
    /** Whether the mouse was held down during a callback */
    bool didHoldMouse;
    /** Whether the mouse was held down during this frame */
    bool isMouseHeld;
    /** The last position that the mouse was recorded in any mouse callback */
    Vec2 lastMousePos;
    

#pragma mark Main Functions
  public:
    /** Creates the model state */
    InputModel()
        : didClickMouse(false), isMouseClicked(false), didHoldMouse(false),
          isMouseHeld(false), lastMousePos(Vec2(0, 0)) {}
};

#endif /* InputModel_h */
