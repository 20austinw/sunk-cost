//
//  InputController.h
//  Sunk Cost
//
//  This module represents the input handlers. Note that this application
//  does not use the mouse at all.  This code is here to simply show you
//  how you might want to organize your handler.
//
//  Author of Referenced file: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__
#include <unordered_set>

// This is in the same folder
#include "InputModel.h"

/**
 * A class representing a unified input handler.
 *
 * This handler processes both keyboard and mouse input. Notice that it
 * caches the result of callbacks, and never polls, even with keyboard
 * events. That is because PCG can slow the game down to 6 FPS (no, there
 * is not a missing 0 there), causing us to miss key events. This
 * architecture allows us to buffer keypresses even when FPS is slow.
 */
class InputController {

#pragma mark Internal References
  private:
    /** Model reference*/
    std::unique_ptr<InputModel> _model;
    /** Whether the input device was successfully initialized */
    bool _active;
    /** Track the keys pressed this animation frame */
    std::unordered_set<KeyCode> _current;
    /** Track the keys pressed the previous animation frame */
    std::unordered_set<KeyCode> _previous;
    /** The unique key for the keyboard listeners */
    Uint32 _kkey;
    /** The unique key for the mouse listeners */
    Uint32 _mkey;
    bool _joyStickPressed;

#pragma mark External References
  private:
    /** Keyboard reference */
    Keyboard* _keyboard;
    /** Mouse reference */
    Mouse* _mouse;
    
    /** Touchscreen reference */
    Touchscreen* _ts;

    /** Touch screen **/
    /** The ID for the touch */
    cugl::TouchID _touchID;
    /** The key for the touch */
    Uint32 _touchKey;
    /** The touch position  */
    cugl::Vec2 _touchPos;
    cugl::Vec2 _center;
    
    /** Whether the finger touch is down */
    bool _touchDown;
    bool _touchUp;

    /** The current touch/mouse position */
    cugl::Vec2 _currPos;
    /** The previous touch/mouse position */
    cugl::Vec2 _prevPos;
    /** Whether there is an active button/touch press */
    bool _currDown;
    /** Whether there was an active button/touch press last frame*/
    bool _prevDown;

#pragma mark State
  private:
    /** How much forward are we going? */
    float _forward;
    /** How much are we turning? */
    float _rightward;

    /** Did we press the exit button? */
    bool _didExit;

    /** Did we press the reset button? */
    bool _didReset;

#pragma mark Main Functions
  public:
    /**
     * Creates the handler, initializing the input devices.
     *
     * This method will create the mouse listeners. In the case of failure
     * an assertion will crash the program.
     *
     * Note, the constructor should not be called directly. Retrieve a
     * singleton with {@link #getInstance} instead.
     */
    InputController();

    bool initListeners();

    /** Returns a singleton instance of InputController. */
    static std::shared_ptr<InputController> getInstance();

    /**
     * Disposes this input controller, deactivating all listeners.
     *
     * As the listeners are deactived, the user will not be able to
     * monitor input until the controller is reinitialized with the
     * {@link #init} method.
     */
    void dispose();

    /**
     * Aligns inputs detected through callbacks with frame updates.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);

    /**
     * Returns the amount of forward movement.
     *
     * -1 = backward, 1 = forward, 0 = still
     *
     * @return amount of forward movement.
     */
    float getForward() const { return _forward; }

    /**
     * Returns the amount to turn the hunter.
     *
     * -1 = clockwise, 1 = counter-clockwise, 0 = still
     *
     * @return amount to turn the hunter.
     */
    float getRight() const { return _rightward; }
    bool getTouchDown() const { return _touchDown; }
    cugl::Vec2 getCenter() const {return _center;}

    /**
     * Returns whether the exit button was pressed.
     *
     * @return whether the exit button was pressed.
     */
    bool didPressExit() const { return _didExit; }

    /**
     * Returns whether the reset button was pressed.
     *
     * @return whether the reset button was pressed.
     */
    bool didPressReset() const { return _didReset; }

    /**
     * Reads the input for this player and converts the result into game logic.
     *
     * This is an example of polling input.  Instead of registering a listener,
     * we ask the controller about its current state.  When the game is running,
     * it is typically best to poll input instead of using listeners.  Listeners
     * are more appropriate for menus and buttons (like the loading screen).
     */
    void readInput();

#pragma mark Attributes
    /**
     * Returns true if this control is active.
     *
     * An active control is one where all of the listeners are attached
     * and it is actively monitoring input. An input controller is only
     * active if {@link #init} is called, and if {@link #dispose} is not.
     *
     * @return true if this control is active.
     */
    bool isActive() const { return _active; }

    /**
     * Returns the current mouse/touch position
     *
     * @return the current mouse/touch position
     */
    const cugl::Vec2& getPosition() const { return _currPos; }

    /**
     * Returns the previous mouse/touch position
     *
     * @return the previous mouse/touch position
     */
    const cugl::Vec2& getPrevious() const { return _prevPos; }

    /**
     * Return true if the user initiated a press this frame.
     *
     * A press means that the user is pressing (button/finger) this
     * animation frame, but was not pressing during the last frame.
     *
     * @return true if the user initiated a press this frame.
     */
    bool didPress() const { return !_prevDown && _currDown; }

    /**
     * Return true if the user initiated a release this frame.
     *
     * A release means that the user was pressing (button/finger) last
     * animation frame, but is not pressing during this frame.
     *
     * @return true if the user initiated a release this frame.
     */
    bool didRelease() const { return !_currDown && _prevDown; }

    /**
     * Return true if the user is actively pressing this frame.
     *
     * This method only checks that a press is active or ongoing.
     * It does not care when the press was initiated.
     *
     * @return true if the user is actively pressing this frame.
     */
    bool isDown() const { return _currDown; }

#pragma mark Mouse Callbacks
  private:
    /**
     * Callback for the beginning of a mouse press event.
     *
     * This function will record a press only if the left button is pressed.
     *
     * @param event     The event with the mouse information
     * @param clicks    The number of clicks (for double clicking)
     * @param focus     Whether this device has focus (UNUSED)
     */
    void buttonDownCB(const MouseEvent& event, Uint8 clicks, bool focus);

    /**
     * Callback for when the mouse is pressed down.
     *
     * This function will record whenever the left mouse is held down after the
     * initial press.
     *
     * @param event     The event with the mouse information
     * @param previous    The previous position of the mouse
     * @param focus     Whether this device has focus (UNUSED)
     */
    void buttonHeldCB(const MouseEvent& event, const Vec2 previous, bool focus);

    /**
     * Callback for the end of a mouse press event.
     *
     * This function will record a release for the left mouse button.
     *
     * @param event     The event with the mouse information
     * @param clicks    The number of clicks (for double clicking)
     * @param focus     Whether this device has focus (UNUSED)
     */
    void buttonUpCB(const MouseEvent& event, Uint8 clicks, bool focus);

    /**
     * Call back to execute when a touch screen is touched.
     *
     * This function will record a press only if ta touch is pressed.
     *
     * @param event     The event with the touch information
     * @param focus     Whether this touch has been pressed
     */
    void touchDownCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Call back to execute when a touch is released.
     *
     *  This function will record a release for the same touch which has been
     * recorded by touchDownCB.
     *
     * @param event     The event with the touch information
     * @param focus    Whether this touch has been released
     */
    void touchUpCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Call back to execute when the touch moves.
     *
     * @param event     The event with the touch information
     * @param focus     Whether this device has focus (UNUSED)
     */
    void touchMotionCB(const cugl::TouchEvent& event, bool focus);

#pragma mark Input State Getters
  public:
    /**
     * Returns whether `key` is held down.
     *
     * @param key   The keyboard key
     */
    bool isKeyPressed(KeyCode key) {
        // Previous was the result before the start of this frame
        return _previous.find(key) != _previous.end();
    }

    /** Returns whether the mouse was first clicked during this frame. */
    bool isMouseClicked() { return _model->isMouseClicked; }

    /** Returns whether the mouse was held down during this frame. */
    bool isMouseHeld() { return _model->isMouseHeld; }

    /** Returns the mouse's last recorded position. */
    Vec2 getLastMousePos() { return _model->lastMousePos; }
    
    /** Returns the touchscreen's last recorded position. */
    Vec2 getTouchPos() {return _touchPos; }
    
    bool isTouchDown() {return _touchDown;}
};

#endif /* __INPUT_CONTROLLER_H__ */
