//
//  InputController.cpp
//  Sunk Cost
//
//  This module represents the input handlers. Note that this application
//  does not use the mouse at all.  This code is here to simply show you
//  how you might want to organize your handler.
//
//  Author of Referenced file: Gonzalo Gonzalez
//  Version: 2/21/23.
//
// This is in the same folder so it is okay
#include "InputController.h"
/**
 * Creates the handler, initializing the input devices.
 *
 * This method will create the mouse listeners. In the case of failure
 * an assertion will crash the program.
 *
 * Note, the constructor should not be called directly. Retrieve a
 * singleton with {@link #getInstance} instead.
 */
InputController::InputController():
_model(std::make_unique<InputModel>()),
_forward(0),
_rightward(0),
_active(false),
_ts(false)
{}

bool InputController::initListeners() {
    Touchscreen* ts = Input::get<Touchscreen>();
    _keyboard = Input::get<Keyboard>();
    _mouse = Input::get<Mouse>();
    if(ts){
        _ts = true;
        _touchKey = ts->acquireKey();
        ts->addBeginListener(_touchKey, [=](const cugl::TouchEvent& event, bool focus){
            this->touchDownCB(event, ts->touchPressed(event.touch));
        });
        ts->addEndListener(_touchKey,[=](const cugl::TouchEvent& event, bool focus) {
            this->touchUpCB(event, ts->touchReleased(event.touch));
        });
        ts->addMotionListener(_touchKey,[=](const cugl::TouchEvent& event, Vec2 previous,bool focus) {
            this->touchMotionCB(event, focus);
            
        });
        _active = true;
    }
    if (_keyboard) {
        CULog("boolean of KEYY %d", _keyboard);
        _kkey = _keyboard->acquireKey();
        bool addedKey = _keyboard->addKeyDownListener(_kkey,
                                                      [this](const KeyEvent& event, bool focus) {
            _current.emplace(event.keycode);
        });
        CUAssertLog(addedKey, "failed adding key listener");
        _active = true;
    }
    if (_mouse) {
        _mkey = _mouse->acquireKey();
        _mouse->setPointerAwareness(Mouse::PointerAwareness::DRAG);
        bool addedPress = _mouse->addPressListener(_mkey, [this](const MouseEvent &event, Uint8 clicks, bool focus) { buttonDownCB(event, clicks, focus);
        });
        CUAssertLog(addedPress, "failed adding press listener");
        
        bool addedDrag = _mouse->addDragListener(_mkey, [this](const MouseEvent& event, const Vec2 previous, bool focus) {
            buttonHeldCB(event, previous, focus);
        });
        CUAssertLog(addedDrag, "failed adding drag listener");
        
        bool addedRelease = _mouse->addReleaseListener(_mkey, [this](const MouseEvent &event, Uint8 clicks, bool focus) {
            buttonUpCB(event, clicks, focus);
        });
        CUAssertLog(addedRelease, "failed adding release listener");
        _active = true;
    }
    CULog("tsts in init %f", _active);
    return _active;
}

/** Returns a singleton instance of InputController. */
std::shared_ptr<InputController> InputController::getInstance() {
    static std::shared_ptr<InputController> inputController;
    if (inputController == nullptr) {
        inputController = std::make_shared<InputController>();
        inputController->initListeners();
    }
    return inputController;
}

/**
 * Reads the input for this player and converts the result into game logic.
 *
 * This is an example of polling input.  Instead of registering a listener,
 * we ask the controller about its current state.  When the game is running,
 * it is typically best to poll input instead of using listeners.  Listeners
 * are more appropriate for menus and buttons (like the loading screen).
 */
void InputController::readInput() {
  // This makes it easier to change the keys later
  KeyCode up    = KeyCode::ARROW_UP;
  KeyCode down  = KeyCode::ARROW_DOWN;
  KeyCode left  = KeyCode::ARROW_LEFT;
  KeyCode right = KeyCode::ARROW_RIGHT;
  KeyCode reset = KeyCode::R;

  // Convert keyboard state into game commands
  _forward = _rightward = 0;
  _didReset = false;

    
  // Movement forward/backward
  Keyboard* keys = Input::get<Keyboard>();
  if(keys){
      if (keys->keyDown(up) && !keys->keyDown(down)) {
        _forward = 1;
      } else if (keys->keyDown(down) && !keys->keyDown(up)) {
        _forward = -1;
      }

      // Movement left/right
      if (keys->keyDown(left) && !keys->keyDown(right)) {
        _rightward = -1;
      } else if (keys->keyDown(right) && !keys->keyDown(left)) {
        _rightward = 1;
      }

      // Reset the game
      if (keys->keyDown(reset)) {
        _didReset = true;
      }
    }
  }
  

void InputController::dispose() {
    Touchscreen* ts = Input::get<Touchscreen>();
    if (_active && ts) {
        ts->removeBeginListener(_touchID);
        ts->removeEndListener(_touchID);
        ts->removeMotionListener(_touchID);
        _active = false;
    }
        
}

/**
 * Aligns inputs detected through callbacks with frame updates.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void InputController::update(float dt) {
    
    if(_ts){
        _prevDown = _currDown;
        _currDown = _touchDown;
        _prevPos = _currPos;
        _currPos = _touchPos;
    }
    

    // Swap the keyboard buffers, removing old keys
    _previous.clear();
    _previous = _current;
    _current.clear();
    
    _model->isMouseClicked = _model->didClickMouse;
    // Only need to detect the first click
    _model->didClickMouse = false;
    _model->isMouseHeld = _model->didHoldMouse;

    
}


#pragma mark -
#pragma mark Mouse Callbacks
/**
 * Callback for the beginning of a mouse press event.
 *
 * This function will record a press only if the left button is pressed.
 *
 * @param event     The event with the mouse information
 * @param clicks    The number of clicks (for double clicking)
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::buttonDownCB(const cugl::MouseEvent &event, Uint8 clicks, bool focus) {
    if (!_model->didHoldMouse && event.buttons.hasLeft()) {
        _model->didClickMouse = true;
        _model->didHoldMouse = true;
        _model->lastMousePos = event.position;
    }
}

/**
 * Callback for when the mouse is pressed down.
 *
 * This function will record whenever the left mouse is held down after the initial press.
 *
 * @param event     The event with the mouse information
 * @param previous    The previous position of the mouse
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::buttonHeldCB(const MouseEvent& event, const Vec2 previous, bool focus) {
    if (_model->didHoldMouse && event.buttons.hasLeft()) {
        _model->lastMousePos = event.position;
    }
}

/**
 * Callback for the end of a mouse press event.
 *
 * This function will record a release for the left mouse button.
 *
 * @param event     The event with the mouse information
 * @param clicks    The number of clicks (for double clicking)
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::buttonUpCB(const cugl::MouseEvent &event, Uint8 clicks, bool focus) {
    if (_model->didHoldMouse && event.buttons.hasLeft()) {
        _model->didHoldMouse = false;
        _model->lastMousePos = event.position;
    }
}

#pragma mark Touch Callbacks
//record one touchID when touchDown, get touchPos
//if multiplr touchID, ignore others until the first touID is touchReleased
//record next touchID

/**
 * Call back to execute when a mouse button is first pressed.
 *
 * This function will record a press only if a touch is pressed.
 *
 * @param event     The event with the touch information
 * @param focus     Whether this touch has been pressed
 */
void InputController::touchDownCB(const cugl::TouchEvent& event, bool focus) {
    if (!_touchDown && event.touch && focus) {
        _touchDown = true;
        _touchID = event.touch;
        _touchPos = event.position;
    }
}

/**
 * Call back to execute when a mouse button is released.
 *
 * This function will record a release for the same touch which has been recorded by touchDownCB.
 *
 * @param event     The event with the touch information
 * @param focus     Whether this touch has been released
 */
void InputController::touchUpCB(const cugl::TouchEvent& event, bool focus) {
    if (_touchDown && event.touch==_touchID && focus) {
        _touchDown = false;
    }
}

                             
/**
 * Call back to execute when the touch moves.
 *
 * @param event     The event with the touch information
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::touchMotionCB(const cugl::TouchEvent& event, bool focus) {
    if (_touchDown && event.touch==_touchID) {
        _touchPos = event.position;
    }
}
                    
