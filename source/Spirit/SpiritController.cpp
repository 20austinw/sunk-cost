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
    std::shared_ptr<cugl::Scene2>& scene,
    std::shared_ptr<PortraitSetController> portraits, Size screenSize) {
    _scene = scene;
    _model = std::make_shared<SpiritModel>(assets, scene, 3, 2, 30);
    _view = std::make_shared<SpiritView>(_model->doors, _model->traps, assets,
                                         _scene);
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
                              Size size) {}

bool SpiritController::placeTrap(
    const std::shared_ptr<TilemapController> _tilemap, Vec2 pos,
    std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    if (_portraits->getCurState()) {
        if (!_tilemap->isTileTraversable(pos) ||
            _tilemap->mapPosToGridPos(pos).x < 0 ||
            _tilemap->mapPosToGridPos(pos).y < 0)
            return false;
        _model->addTrap(pos, node);
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
int SpiritController::update(bool trap, Vec2 pos,
                             std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    int result = _model->update(trap, pos, node);
    _portraits->update();
    return result;
}

void SpiritController::updateLocksPos() {
    if (!_model->isOnLock) {
        _view->updateUnusedLocksPos();
    }
}

void SpiritController::updateTrapBtnsPos() {
    if (!_model->isOnTrap) {
        _view->updateUnusedTrapsPos();
    }
}

void SpiritController::removeLastLock(
    std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    if (_model->doors <= 0) {
        return;
    }
    _model->setDoors(_model->doors - 1);
    _view->removeLastLock(node);
}

void SpiritController::removeLastTrapBtn(
    std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    if (_model->traps <= 0) {
        return;
    }
    _model->setTraps(_model->traps - 1);
    _view->removeLastTrapButton(node);
}

bool SpiritController::touchInLockBound(Vec2 touchPos) {
    if (_model->doors <= 0) {
        return false;
    }
    float dist = _view->getLastLockPos().distance(touchPos);
    return abs(dist) <= _view->getLockSize().width / 2 &&
           abs(dist) <= _view->getLockSize().height / 2;
}

bool SpiritController::touchInTrapBound(Vec2 touchPos) {
    if (_model->traps <= 0) {
        return false;
    }
    float dist = _view->getLastTrapBtnPos().distance(touchPos);
    return abs(dist) <= _view->getTrapSize().width / 2 &&
           abs(dist) <= _view->getTrapSize().height / 2;
}

void SpiritController::updateMovingLock(Vec2 pos) {
    _view->updateLockInProgress(pos);
}

void SpiritController::updateMovingTrap(Vec2 pos) {
    _view->updateTrapInProgress(pos);
}

void SpiritController::addNewTrapBtn(
    std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    _model->setTraps(_model->traps + 1);
    _view->addNewTrap(node);
    updateTrapBtnsPos();
}

void SpiritController::addNewLock(
    std::shared_ptr<cugl::scene2::PolygonNode>& node) {
    _model->setDoors(_model->doors + 1);
    _view->addNewLock(node);
    updateLocksPos();
}

void SpiritController::updateKillFrame() {
    _model->setKillCooldown(_model->killCool-1);
    if (_model->isKillable()){
        _view->setKillFrame(12);
        return;
    }
    float step = 300/11;
    int frame = _model->killCool /step;
    _view->setKillFrame(12 - frame);
}

void SpiritController::updateKillBtnsPos(){
    if(!_model->isOnKill) {
        _view->updateUnusedKillPos();
    }
}

bool SpiritController::touchInKillBound(Vec2 touchPos) {
    float dist = _view->getKillBtnPos().distance(touchPos);
    return abs(dist) <= _view->getKillSize().width/2 && abs(dist) <= _view->getKillSize().height/2;
}


bool SpiritController::hunterInBound(Vec2 pos) {
    float dis = abs(_model->getHunterPos().distance(pos));
    return dis <= _view->getKillSize().width;
}
