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
        _model->setStep(touchPos.distance(_model->getPosition()));
    }
    //detect if ending pos for the drug is on an unlocked door, change to locked
    else if(released && isInBound() && _model->getState() == 2){
        _model->setState(1);
        return true;
    } else if (released && _model->getState() == 2) {
        _model->setState(0);
        setFrame(0);
    }
    //update the current frame number
    if(_model->getState() == 2){
        updateFrame(touchPos);
    }
    return false;
}

void DoorController::updateFrame(Vec2 pos){
    float dis = pos.distance(_model->getPosition());
    int frame = dis/_model->getStep();
    
    //ensure the frame number is in the range
    if ((frame >= 0) && ((_model->getType() == 0 && frame <= FRAME_NUM_FRONT -1)
        || (_model->getType() == 1 && frame <= FRAME_NUM_SIDE -1))){
        setFrame(frame);
    }
}

bool DoorController::isInBound(){
    if (_model->getType() == 0){
        return _model->getFrame() == FRAME_NUM_FRONT -1;
    } else{
        return _model->getFrame() == FRAME_NUM_SIDE -1;
    }
}
