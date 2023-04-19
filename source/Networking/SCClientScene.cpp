//
//  SCClientScene.cpp
//  sunk-mac
//
//  Created by chloe on 3/17/23.
//

#include "SCClientScene.hpp"

#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

using namespace cugl;
using namespace cugl::net;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  720

/**
 * Converts a decimal string to a hexadecimal string
 *
 * This function assumes that the string is a decimal number less
 * than 65535, and therefore converts to a hexadecimal string of four
 * characters or less (as is the case with the lobby server). We
 * pad the hexadecimal string with leading 0s to bring it to four
 * characters exactly.
 *
 * @param dec the decimal string to convert
 *
 * @return the hexadecimal equivalent to dec
 */
static std::string dec2hex(const std::string dec) {
    Uint32 value = strtool::stou32(dec);
    if (value >= 655366) {
        value = 0;
    }
    return strtool::to_hexstring(value,4);
}


#pragma mark -
#pragma mark Provided Methods
/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool ClientScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    // Start up the input handler
    _assets = assets;
    
    // Acquire the scene built by the asset loader and resize it the scene
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("client");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    
//    _startgame = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("client_center_start"));
    _backout = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("client_back"));
    _status = Status::IDLE;
    
    _backout->addListener([this](const std::string& name, bool down) {
        if (down) {
            disconnect();
            _status = Status::ABORT;
        }
    });

//    _startgame->addListener([=](const std::string& name, bool down) {
//        if (down) {
//            // This will call the _gameid listener
////            _gameid->releaseFocus();
//        }
//    });
    
//    _gameid->addExitListener([this](const std::string& name, const std::string& value) {
//        connect(value);
//    });

    // Create the server configuration
    auto json = _assets->get<JsonValue>("server");
    _config.set(json);
    
    addChild(scene);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void ClientScene::dispose() {
    if (_active) {
        removeAllChildren();
        _network = nullptr;
        _active = false;
    }
}

/**
 * Sets whether the scene is currently active
 *
 * This method should be used to toggle all the UI elements.  Buttons
 * should be activated when it is made active and deactivated when
 * it is not.
 *
 * @param value whether the scene is currently active
 */
void ClientScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            _status = IDLE;
//            _gameid->activate();
            _backout->activate();
            _network = nullptr;
//            _player->setText("1");
//            configureStartButton();
            // Don't reset the room id
        } else {
//            _gameid->deactivate();
//            _startgame->deactivate();
            _backout->deactivate();
            // If any were pressed, reset them
//            _startgame->setDown(false);
            _backout->setDown(false);
        }
    }
}

/**
 * Checks that the network connection is still active.
 *
 * Even if you are not sending messages all that often, you need to be calling
 * this method regularly. This method is used to determine the current state
 * of the scene.
 *
 * @return true if the network connection is still active.
 */
void ClientScene::updateText(const std::shared_ptr<scene2::Button>& button, const std::string text) {
    auto label = std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"));
    label->setText(text);

}

#pragma mark -
#pragma mark Student Methods
/**
 * The method called to update the scene.
 *
 * We need to update this method to constantly talk to the server
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void ClientScene::update(float timestep) {
    // IMPLEMENT ME
    if (_network) {
        _network->receive([this](const std::string source,
                                 const std::vector<std::byte>& data) {
            processData(source,data);
        });
        checkConnection();
        // Do this last for button safety
        configureStartButton();
    }
}


/**
 * Connects to the game server as specified in the assets file
 *
 * The {@link #init} method set the configuration data. This method simply uses
 * this to create a new {@Link NetworkConnection}. It also immediately calls
 * {@link #checkConnection} to determine the scene state.
 *
 * @param room  The room ID to use
 *
 * @return true if the connection was successful
 */
bool ClientScene::connect(const std::string room) {
    // THIS IS WRONG. FIX ME
    _network = NetcodeConnection::alloc(_config, dec2hex(room));
    _network->open();
    checkConnection();
    return true;
}

/**
 * Processes data sent over the network.
 *
 * Once connection is established, all data sent over the network consistes of
 * byte vectors. This function is a call back function to process that data.
 * Note that this function may be called *multiple times* per animation frame,
 * as the messages can come from several sources.
 *
 * Typically this is where players would communicate their names after being
 * connected. In this lab, we only need it to do one thing: communicate that
 * the host has started the game.
 *
 * @param source    The UUID of the sender
 * @param data      The data received
 */
void ClientScene::processData(const std::string source,
                              const std::vector<std::byte>& data) {
    // IMPLEMENT ME
    if (source != "" && source == _network->getHost()) {
        _status = Status::START;
    }
}

/**
 * Checks that the network connection is still active.
 *
 * Even if you are not sending messages all that often, you need to be calling
 * this method regularly. This method is used to determine the current state
 * of the scene.
 *
 * @return true if the network connection is still active.
 */
bool ClientScene::checkConnection() {
    // IMPLEMENT ME
    NetcodeConnection::State network_status = _network->getState();
    switch (network_status) {
        case NetcodeConnection::State::NEGOTIATING:
            _status = Status::JOIN;
            break;
        case NetcodeConnection::State::CONNECTED:
            if (_status != Status::START) {
                _status = Status::WAIT;
            }
            _player->setText(std::to_string(_network->getNumPlayers()));
            break;
        case NetcodeConnection::State::DENIED:
        case NetcodeConnection::State::MISMATCHED:
        case NetcodeConnection::State::INVALID:
        case NetcodeConnection::State::FAILED:
        case NetcodeConnection::State::DISCONNECTED:
            disconnect();
            _status = Status::IDLE;
            return false;
    }
    return true;
}

/**
 * Reconfigures the start button for this scene
 *
 * This is necessary because what the buttons do depends on the state of the
 * networking.
 */
void ClientScene::configureStartButton() {
    // THIS IS WRONG. FIX ME
    if (_status == Status::IDLE) {
        _startgame->activate();
        updateText(_startgame, "Start Game");
    } else if (_status == Status::JOIN) {
        _startgame->deactivate();
        updateText(_startgame, "Connecting");
    } else if (_status == Status::WAIT) {
        _startgame->deactivate();
        updateText(_startgame, "Waiting");
    }
        
}
