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
    /** Track the keys pressed this animation frame */
    std::unordered_set<KeyCode> _current;
    /** Track the keys pressed the previous animation frame */
    std::unordered_set<KeyCode> _previous;
    /** The unique key for the keyboard listeners */
    Uint32 _kkey;
    /** The unique key for the mouse listeners */
    Uint32 _mkey;
    

#pragma mark External References
private:
    /** Keyboard reference */
    Keyboard *_keyboard;
    /** Mouse reference */
    Mouse *_mouse;

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
    float getForward() const {
        return _forward;
    }

    /**
     * Returns the amount to turn the hunter.
     *
     * -1 = clockwise, 1 = counter-clockwise, 0 = still
     *
     * @return amount to turn the hunter.
     */
    float getRight() const {
        return _rightward;
    }


    /**
     * Returns whether the exit button was pressed.
     *
     * @return whether the exit button was pressed.
     */
    bool didPressExit() const {
      return _didExit;
    }

    /**
     * Returns whether the reset button was pressed.
     *
     * @return whether the reset button was pressed.
     */
    bool didPressReset() const {
      return _didReset;
    }

    /**
     * Reads the input for this player and converts the result into game logic.
     *
     * This is an example of polling input.  Instead of registering a listener,
     * we ask the controller about its current state.  When the game is running,
     * it is typically best to poll input instead of using listeners.  Listeners
     * are more appropriate for menus and buttons (like the loading screen).
     */
    void readInput();

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
    void buttonDownCB(const MouseEvent &event, Uint8 clicks, bool focus);

    /**
     * Callback for when the mouse is pressed down.
     *
     * This function will record whenever the left mouse is held down after the initial press.
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
    void buttonUpCB(const MouseEvent &event, Uint8 clicks, bool focus);

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
};

#endif /* __INPUT_CONTROLLER_H__ */
