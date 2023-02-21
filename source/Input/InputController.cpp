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
InputController::InputController(): _model(std::make_unique<InputModel>()), _keyboard(Input::get<Keyboard>()),
    _mouse(Input::get<Mouse>()) {
    _kkey = _keyboard->acquireKey();
    bool addedKey = _keyboard->addKeyDownListener(_kkey,
        [this](const KeyEvent& event, bool focus) {
            _current.emplace(event.keycode);
        });
    CUAssertLog(addedKey, "failed adding key listener");

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
}

/** Returns a singleton instance of InputController. */
std::shared_ptr<InputController> InputController::getInstance() {
    static std::shared_ptr<InputController> inputController;
    if (inputController == nullptr) {
        inputController = std::make_shared<InputController>();
    }
    return inputController;
}

/**
 * Aligns inputs detected through callbacks with frame updates.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void InputController::update(float dt) {
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
