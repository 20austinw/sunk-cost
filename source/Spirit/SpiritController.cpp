//
//  SpiritController.cpp
//  Sunk Cost
//
//  This module provides the SpiritController class.
//
//  Author: Soon Jae Park
//  Version: 2/22/23.
//
#include "SpiritController.h"

#pragma mark Main Functions

SpiritController::SpiritController() {}
/**
 * TODO: Implement Me
 * The Constructor should set up the preset cooltimes as well as all other class
 * variables.
 */
SpiritController::SpiritController(
    const std::shared_ptr<cugl::AssetManager>& assets,
    std::shared_ptr<cugl::Scene2> scene,
    std::shared_ptr<PortraitSetController> portraits, Size screenSize) {
    _scene = scene;
    _model = std::make_shared<SpiritModel>(assets, scene, 3, 2, 30);
    _view = std::make_shared<SpiritView>(_model->doors, _model->traps, assets,  _scene);
    _portraits = portraits;
    _screenSize = screenSize;
    _cameraCool = CAMERA_COOL;
}

/**
 * TODO: Implement Me (Not for Gameplay Prototype)
 */
Vec2 SpiritController::closeDoors() { return Vec2::ZERO; }

/**
 * TODO: Implement Me (Not for Gameplay Prototype)
 */
Vec2 SpiritController::placeTraps() { return Vec2::ZERO; }

// Helper function
Rect SpiritController::screenToWorld(Rect rect) {
    Size size = rect.size;
    const float WIDTH = _screenSize.width;
    const float HEIGHT = _screenSize.height;
    return Rect(2 * rect.getMinX() / WIDTH, 2 * rect.getMinY() / HEIGHT,
                2 * size.width / WIDTH, 2 * size.height / HEIGHT);
}

/**
 * TODO: Implement Me
 * This function should
 * (1) get the view from portraitsetcontroller
 * (2) attach the viewport to the scene
 */
void SpiritController::render(std::shared_ptr<cugl::SpriteBatch>& batch,
                              Size size) {
}


bool SpiritController::placeTrap(const std::shared_ptr<TilemapController> _tilemap,
                              Vec2 pos) {
    if (_portraits->getCurState()) {
        if (!_tilemap->isTileTraversable(pos) ||
            _tilemap->mapPosToGridPos(pos).x < 0 ||
            _tilemap->mapPosToGridPos(pos).y < 0)
            return false;
        _model->addTrap(pos);
        _trapAdded = true;
        setLastTrapPos(pos);
    }
    return true;
}

/**
 * TODO: Implement Me
 * This function should (for gameplay prototype)
 * (1) detect camera change
 * (2) modify portraitsetcontroller to reflect the change
 */
bool SpiritController::update(){
    bool result = _model->update();
    _portraits->update();
    return result;
}


void SpiritController::updateLocksPos(const std::shared_ptr<cugl::Scene2>& scene){
    if (!_model->isOnLock){
        float zoom = std::dynamic_pointer_cast<OrthographicCamera>(scene->getCamera()) ->getZoom();
        Vec2 pos = scene->getCamera()->screenToWorldCoords(
                _scene->getSize() - _view->getLockSize() / 2 * zoom);
        _view->updateUnusedLocksPos(pos);
        _view->removeLocksFrom(scene);
        _view->addLocksTo(scene);
    }
}

void SpiritController::updateTrapBtnsPos(const std::shared_ptr<cugl::Scene2>& scene){
    if (!_model->isOnTrap){
        float zoom = std::dynamic_pointer_cast<OrthographicCamera>(scene->getCamera()) ->getZoom();
        Vec2 pos = scene->getCamera()->screenToWorldCoords(
                _scene->getSize() - _view->getTrapSize() / 2 * zoom) + Vec2(0, +_view->getLockSize().height);
        _view->updateUnusedTrapsPos(pos);
        _view->removeTrapsFrom(scene);
        _view->addTrapButtonsTo(scene);
    }
}

void SpiritController::removeLastLock(const std::shared_ptr<cugl::Scene2>& scene){
    if (_model->doors <= 0){
        return;
    }
    _model->setDoors(_model->doors-1);
    _view->removeLastLock(scene);
}

void SpiritController::removeLastTrapBtn(const std::shared_ptr<cugl::Scene2> &scene){
    if (_model->traps <= 0){
        return;
    }
    _model->setTraps(_model->traps-1);
    _view->removeLastTrapButton(scene);
}

bool SpiritController::touchInLockBound(Vec2 touchPos){
    if(_model->doors <= 0){
        return false;
    }
    float dist = _view->getLastLockPos().distance(touchPos);
    return abs(dist) <= _view->getLockSize().width/2 && abs(dist) <= _view->getLockSize().height/2;
}

bool SpiritController::touchInTrapBound(Vec2 touchPos){
    if (_model->traps <= 0){
        return false;
    }
    float dist = _view->getLastTrapBtnPos().distance(touchPos);
    return abs(dist) <= _view->getTrapSize().width/2 && abs(dist) <= _view->getTrapSize().height/2;
}

void SpiritController::updateMovingLock(Vec2 pos){
    _view->updateLockInProgress(pos);
}

void SpiritController::updateMovingTrap(Vec2 pos){
    _view->updateTrapInProgress(pos);
}

void SpiritController::addNewTrapBtn(const std::shared_ptr<Scene2>& scene){
    _model->setTraps(_model->traps+1);
    _view->addNewTrap(scene);
    updateTrapBtnsPos(scene);
}

void SpiritController::addNewLock(const std::shared_ptr<Scene2>& scene){
    _model->setDoors(_model->doors+1);
    _view->addNewLock(scene);
    updateLocksPos(scene);
}

