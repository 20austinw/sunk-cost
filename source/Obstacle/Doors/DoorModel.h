//
//  DoorModel.h
//  Sunk
//
//  Created by Xiaoyue Jin on 3/24/23.
//

#ifndef DoorModel_h
#define DoorModel_h

#include <cugl/cugl.h>

using namespace cugl;

class DoorModel {
#define FRAME_NUM_FRONT 0
#define FRAME_NUM_SIDE 0
#pragma mark States
private:
    /** door type (0 for front, 1 for side) */
    int _type;
    /** door type (0 for unlocked; 1 for locked;  2 for animation in progress) */
    int _state;
    
    Vec2 _position;
    
    float _step;
    
    int _frame;

#pragma mark Main Functions
public:
    DoorModel(Vec2 pos, int type){
        setType(type);
        setPosition(pos);
        setStep(0);
        setState(0);
        setFrame(0);
    }
    

#pragma mark Getters
    Vec2 getPosition(){
        return _position;
    }
    
    int getState(){
        return _state;
    }
    
    int getFrame(){
        return _frame;
    }
    
    int getType(){
        return _type;
    }
    
    float getStep(){
        return _step;
    }

#pragma mark Setters
    void setStep(float distance){
        if(_type == 0){
            _step = distance/(FRAME_NUM_FRONT-1);
        }else{
            _step = distance/(FRAME_NUM_SIDE-1);
        }
    }
    
    void setPosition(Vec2 position){
        _position = position;
    }
    
    void setType(int type){
        _type = type;
    }
    
    void setState(int state){
        _state = state;
    }
    
    void setFrame(int frame){
        _frame = frame;
    }
    
};


#endif /* DoorModel_h */
