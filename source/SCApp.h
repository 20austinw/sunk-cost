//
//  SCApp.h
//  Sunk Cost
//
//  This is the root class for your game.  The file main.cpp accesses this class
//  to run the application.  While you could put most of your game logic in
//  this class, we prefer to break the game up into player modes and have a
//  class for each mode.
//
//  Author of Referenced File: Walker White
//  Version: 2/22/23
//
#ifndef _SC_APP_H__
#define _SC_APP_H__
#include "HGameController.h"
#include "LoadingScene.h"
#include "SCMenuScene.h"
#include "SGameController.hpp"
#include <cugl/cugl.h>

/**
 * This class represents the application root for the ship demo.
 */
class SCApp : public cugl::Application {
protected:
    /**
     * The current active scene
     */
    enum State {
        /** The loading scene */
        LOAD,
        /** The main menu scene */
        MENU,
        /** The scene to host a game */
        HOST,
        /** The scene to join a game */
        CLIENT
    };
    
protected:
    /** The global sprite batch for drawing (only want one of these) */
    std::shared_ptr<cugl::SpriteBatch> _batch;
    /** The global asset manager */
    std::shared_ptr<cugl::AssetManager> _assets;
    // Player modes
    /** The primary controller for the hunter game world */
    HGameController _hunterGameplay;
    /** The primary controller for the spirit game world*/
    SGameController _spiritGameplay;
    /** The controller for the loading screen */
    LoadingScene _loading;
    
    MenuScene _menu;
    
    /** Whether or not we have finished loading all assets */
    bool _loaded;
    
    /** The current active scene */
    SCApp::State _scene;
    
    
public:
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    SCApp() : cugl::Application(), _loaded(false), _scene(State::LOAD) {}
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~SCApp() { }
#pragma mark Application State
    /**
     * The method called after OpenGL is initialized, but before running the
     * application.
     *
     * This is the method in which all user-defined program intialization should
     * take place.  You should not create a new init() method.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to FOREGROUND,
     * causing the application to run.
     */
    virtual void onStartup() override;
    
    /**
     * The method called when the application is ready to quit.
     *
     * This is the method to dispose of all resources allocated by this
     * application.  As a rule of thumb, everything created in onStartup()
     * should be deleted here.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to NONE,
     * causing the application to be deleted.
     */
    virtual void onShutdown() override;
    
    /**
     * The method called when the application is suspended and put in the
     * background.
     *
     * When this method is called, you should store any state that you do not
     * want to be lost.  There is no guarantee that an application will return
     * from the background; it may be terminated instead.
     *
     * If you are using audio, it is critical that you pause it on suspension.
     * Otherwise, the audio thread may persist while the application is in
     * the background.
     */
    virtual void onSuspend() override;
    
    /**
     * The method called when the application resumes and put in the foreground.
     *
     * If you saved any state before going into the background, now is the time
     * to restore it. This guarantees that the application looks the same as
     * when it was suspended.
     *
     * If you are using audio, you should use this method to resume any audio
     * paused before app suspension.
     */
    virtual void onResume() override;
    
#pragma mark Application Loop
    /**
     * The method called to update the application data.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should contain any code that is not an OpenGL call.
     * This is your core loop and should be replaced with your custom
     * implementation. This method should contain any code that is not an OpenGL
     * call.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    virtual void update(float timestep) override;
    
    /**
     * Inidividualized update method for the loading scene.
     *
     * This method keeps the primary {@link #update} from being a mess of switch
     * statements. It also handles the transition logic from the loading scene.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void updateLoadingScene(float timestep);
    
    /**
     * Inidividualized update method for the menu scene.
     *
     * This method keeps the primary {@link #update} from being a mess of switch
     * statements. It also handles the transition logic from the menu scene.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void updateMenuScene(float timestep);
    
    /**
     * Inidividualized update method for the host scene.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void updateSGameController(float timestep);
    
    /**
     * Inidividualized update method for the client scene.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void updateHGameController(float timestep);
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom
     * implementation. This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    virtual void draw() override;
    
};

#endif /* _SC_APP_H__ */
