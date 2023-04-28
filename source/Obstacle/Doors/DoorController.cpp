//
//  DoorController.cpp
//  Sunk
//
//  Created by Xiaoyue Jin on 3/24/23.
//

#include "DoorController.hpp"
#include "DoorModel.h"
#include "DoorView.h"

bool DoorController::update(bool started, bool released, Vec2 touchPos){
    //detect a drug action to enable the animation for an unlocked door, then update step for the frame number
    if (started && _model->getState() == 0){
        _model->setState(2);
        _model->setStep(abs(touchPos.distance(_model->getPosition())));
    }
    //detect if ending pos for the drug is on an unlocked door, change to locked
    else if(released && isInBound(touchPos) && _model->getState() == 2){
        _model->setState(1);
        setLockFrame();
        return true;
    } else if (released && _model->getState() == 2) {
        _model->setState(0);
        setFrame(0);
        return false;
    }
    //update the current frame number
    if(_model->getState() == 2){
        updateFrame(touchPos);
    }
    return false;
}



void DoorController::updateFrame(Vec2 pos){
    float dis = abs(pos.distance(_model->getPosition()));
    int frame;
    if(_model->getType() == 0){
        frame = FRAME_NUM_FRONT-1-(dis/_model->getStep());
    } else {
        frame = FRAME_NUM_SIDE-1-(dis/_model->getStep());
    }
    
    //ensure the frame number is in the range
    if ((frame >= 0) && ((_model->getType() == 0 && frame <= FRAME_NUM_FRONT -1)
        || (_model->getType() == 1 && frame <= FRAME_NUM_SIDE -1))){
        setFrame(frame);
    }
}

bool DoorController::isInBound(Vec2 pos){
    float dist = abs(pos.distance(getPosition()));
    if(dist <= _view->getSize().width/2 && dist<= _view->getSize().height/2){
        return true;
    }
    
    return false;
}

void DoorController::setLockFrame(){
    if(_model->getType() == 0){
        setFrame(FRAME_NUM_FRONT-1);
    }else {
        setFrame(FRAME_NUM_SIDE-1);
    }
}

void DoorController::resetToUnlock(){
    if(_model->getState() == 2){
        _model->setState(0);
        setFrame(0);
    }
}

void DoorController::resetHunterUnlock(){
    _model->setState(0);
    setFrame(0);
}

Vec2 DoorController::getModelPosition(){
    return _model->getPosition();
    
}

Vec2 DoorController::getViewPosition(){
    return _view->getPosition();
    
}

bool DoorController::isLocked(){
    return _model->getState() == 1;
}
