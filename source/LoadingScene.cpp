//
//  GLLoadingScene.cpp
//  Ship Lab
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify. In
//  fact, this loading screen uses the new modular JSON format for defining
//  scenes.  See the file "loading.json" for how to change this scene.
//
//  Author: Walker White
//  Version: 1/20/22
//
#include "LoadingScene.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE 1024

#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LoadingScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    // Lock the scene to a reasonable resolution
    if (dimen.width > dimen.height) {
        dimen *= SCENE_SIZE / dimen.width;
    } else {
        dimen *= SCENE_SIZE / dimen.height;
    }
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/loading.json");
    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    _bar = std::dynamic_pointer_cast<scene2::ProgressBar>(
        assets->get<scene2::SceneNode>("load_bar"));
    _button = std::dynamic_pointer_cast<scene2::Button>(
        assets->get<scene2::SceneNode>("load_play"));
    _button->addListener(
        [=](const std::string& name, bool down) {
            this->_active = down;
            if (down) {
                _choice = Choice::PLAY;
            }
        });
    
    _settingsButton = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("load_row1_settings"));
    _settingsButton->addListener([=](const std::string& name, bool down) {
        this->_active = down;
        if (down) {
            _choice = Choice::SETTINGS;
        }
    });
    
    _creditsButton = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("load_row1_credits"));
    _creditsButton->addListener([=](const std::string& name, bool down) {
        this->_active = down;
        if (down) {
            _choice = Choice::CREDITS;
        }
    });
    
    _tutorialButton = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("load_row1_tutorial"));
    _tutorialButton->addListener([=](const std::string& name, bool down) {
        this->_active = down;
        if (down) {
            _choice = Choice::TUTORIAL;
        }
    });

    _background = std::dynamic_pointer_cast<scene2::SpriteNode>(
        assets->get<scene2::SceneNode>("load_logo"));
    _background->setScale(Application::get()->getDisplayHeight() /
                          (_background->getHeight() * 1.5));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);

    _choice = Choice::NONE;
    _frameNum = 0;
    _ticks = 0;
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadingScene::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button->deactivate();
    }
    _button = nullptr;
    _creditsButton = nullptr;
    _settingsButton = nullptr;
    _tutorialButton = nullptr;
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
}

#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingScene::update(float progress) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _bar->setVisible(false);
            _button->setVisible(true);
            _creditsButton->setVisible(true);
            _settingsButton->setVisible(true);
            _tutorialButton->setVisible(true);
            _button->activate();
            _creditsButton->activate();
            _settingsButton->activate();
            _tutorialButton->activate();
        }
        _bar->setProgress(_progress);
    }

    if (_ticks == 6) {
        if (_frameNum >= 39) {
            _frameNum = 0;
        } else {
            _frameNum++;
        }
        _ticks = 0;
    }

    _ticks++;
    _background->setFrame(_frameNum);
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadingScene::isPending() const {
    return _button != nullptr && _button->isVisible();
}
