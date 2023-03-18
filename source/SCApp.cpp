//
//  SCApp.cpp
//  Sunk Cost
//
//  This is the root class for the game.  The file main.cpp accesses this class
//  to run the application.
//  While we could put most of your game logic in
//  this class, we prefer to break the game up into player modes and have a
//  class for each mode.
//
//  Author of Referenced File: Walker White
//  Version: 2/22/23
//
#include "SCApp.h"
#include "LevelConstants.h"
#include "LevelModel.h"

using namespace cugl;

#pragma mark -
#pragma mark Gameplay Control

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
void SCApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch  = SpriteBatch::alloc();
#ifdef CU_TOUCH_SCREEN
    // Start-up basic input for loading screen (MOBILE ONLY)
    Input::activate<Touchscreen>();
#else
//    // Start-up basic input for loading screen (DESKTOP ONLY)
    Input::activate<Mouse>();
    Input::activate<Keyboard>();
#endif
   
    
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<WidgetValue>(WidgetLoader::alloc()->getHook());
    _assets->attach<JsonValue>(JsonLoader::alloc()->getHook());
    _assets->attach<scene2::SceneNode>(Scene2Loader::alloc()->getHook()); // Needed for loading screen
    _assets->attach<LevelModel>(GenericLoader<LevelModel>::alloc()->getHook());

    // Create a "loading" screen
    _scene = State::LOAD;
    _loaded = false;
    _loading.init(_assets);
    
    // Queue up the other assets
    _assets->loadDirectoryAsync("json/assets.json", nullptr);
    _assets->loadAsync<LevelModel>(LEVEL_TWO_KEY, LEVEL_TWO_FILE, nullptr);
    
    net::NetworkLayer::start(net::NetworkLayer::Log::INFO);
    AudioEngine::start();
    Application::onStartup(); // YOU MUST END with call to parent
}

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
void SCApp::onShutdown() {
    _loading.dispose();
    _assets = nullptr;
    _batch = nullptr;

    // Shutdown input
#ifdef CU_MOBILE
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Keyboard>();
    Input::deactivate<Mouse>();
#endif

    net::NetworkLayer::stop();
    AudioEngine::stop();
    Application::onShutdown();  // YOU MUST END with call to parent

}

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
void SCApp::onSuspend() { AudioEngine::get()->pause(); }

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
void SCApp::onResume() { AudioEngine::get()->resume(); }

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom
 * implementation. This method should contain any code that is not an OpenGL
 * call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void SCApp::update(float timestep) {
    switch (_scene) {
        case LOAD:
            updateLoadingScene(timestep);
            break;
        case MENU:
            updateMenuScene(timestep);
            break;
        case HOST:
            updateHostScene(timestep);
            break;
        case CLIENT:
            updateClientScene(timestep);
        case HOSTGAME:
            updateSGameController(timestep);
            break;
        case CLIENTGAME:
            updateHGameController(timestep);
            break;
    }
}


void SCApp::updateMenuScene(float timestep) {
    _menu.update(timestep);
    switch (_menu.getChoice()) {
        case MenuScene::Choice::HOST:
            _menu.setActive(false);
            _scene = State::HOST;
            break;
        case MenuScene::Choice::CLIENT:
            _menu.setActive(false);
            _scene = State::CLIENT;
            break;
        case MenuScene::Choice::NONE:
            // DO NOTHING
            break;
    }
}

void SCApp::updateLoadingScene(float timestep) {
    if (!_loaded && _loading.isActive()) {
        _loading.update(0.01f);
    } else {
        _loading.dispose();
        _menu.init(_assets);
        _hostgame.init(_assets);
        _spiritGameplay = SGameController(getDisplaySize(), _assets);
        _hunterGameplay = HGameController(getDisplaySize(), _assets);
        _menu.setActive(true);
        _scene = State::MENU;
    }
}

void SCApp::updateHostScene(float timestep) {
    _hostgame.update(timestep);
    switch (_hostgame.getStatus()) {
        case HostScene::Status::ABORT:
            _hostgame.setActive(false);
            _menu.setActive(true);
            _scene = State::MENU;
            break;
        case HostScene::Status::START:
            _hostgame.setActive(false);
            _scene = State::HOSTGAME;
            // Transfer connection ownership
//            _gameplay.setConnection(_hostgame.getConnection());
            _hostgame.disconnect();
//            _gameplay.setHost(true);
            break;
        case HostScene::Status::WAIT:
        case HostScene::Status::IDLE:
            // DO NOTHING
            break;
    }
}

void SCApp::updateClientScene(float timestep) {
//    _joingame.update(timestep);
//    switch (_joingame.getStatus()) {
//        case ClientScene::Status::ABORT:
//            _joingame.setActive(false);
//            _mainmenu.setActive(true);
//            _scene = State::MENU;
//            break;
//        case ClientScene::Status::START:
//            _joingame.setActive(false);
//            _gameplay.setActive(true);
//            _scene = State::GAME;
//            // Transfer connection ownership
//            _gameplay.setConnection(_joingame.getConnection());
//            _joingame.disconnect();
//            _gameplay.setHost(false);
//            break;
//        case ClientScene::Status::WAIT:
//        case ClientScene::Status::IDLE:
//        case ClientScene::Status::JOIN:
//            // DO NOTHING
//            break;
//    }
}

void SCApp::updateHGameController(float timestep) {
    _hunterGameplay.update(timestep);
    switch (_hunterGameplay.getStatus()) {
        case HGameController::Status::ABORT:
            _scene = State::MENU;
            break;
        case HGameController::Status::START:
            _scene = State::CLIENTGAME;
            // Transfer connection ownership
            _hunterGameplay.setHost(false);
            break;
        case HGameController::Status::WAIT:
        case HGameController::Status::IDLE:
        case HGameController::Status::JOIN:
            // DO NOTHING
            break;
    }
}

void SCApp::updateSGameController(float timestep) {
    _spiritGameplay.update(timestep);
    switch (_spiritGameplay.getStatus()) {
        case SGameController::Status::ABORT:
            _menu.setActive(true);
            _scene = State::MENU;
            break;
        case SGameController::Status::START:
            _scene = State::HOSTGAME;
            _spiritGameplay.setHost(true);
            break;
        case SGameController::Status::WAIT:
        case SGameController::Status::IDLE:
            // DO NOTHING
            break;
    }
}
/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom
 * implementation. This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void SCApp::draw() {
    switch (_scene) {
        case LOAD:
            _loading.render(_batch);
            break;
        case MENU:
            _menu.render(_batch);
            break;
        case HOST:
            _hostgame.render(_batch);
            break;
        case HOSTGAME:
            _spiritGameplay.render(_batch);
            break;
        case CLIENTGAME:
            _hunterGameplay.render(_batch);
            break;
    }
}
