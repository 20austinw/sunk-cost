//
//  SCCreditScene.cpp
//  sunk-ios
//
//  Created by chloe on 5/19/23.
//

#include "SCCreditScene.hpp"

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

#define SCENE_HEIGHT 720

bool CreditScene::init(const std::shared_ptr<cugl::AssetManager> &assets) {
    
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT / dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    // Start up the input handler
    _assets = assets;
    
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("credits");
    scene->setContentSize(dimen);
    scene->doLayout();
    
    _backButton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("credits_back"));
    
    _backButton->addListener([=](const std::string& name, bool down) {
        if (down) {
            _choice = Choice::LOAD;
        }
    });
    
    _choice = Choice::NONE;
    
    addChild(scene);
    setActive(false);
    return true;
}

void CreditScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}

void CreditScene::setActive(bool value) {
    if (!isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            _choice = Choice::NONE;
            _backButton->activate();
        } else {
            _backButton->deactivate();
            _backButton->setDown(false);
        }
    }
}
