//
//  TrapModel.hpp
//  Sunk
//
//  Created by 任清扬 on 14/3/23.
//

#ifndef TrapModel_hpp
#define TrapModel_hpp



#include <cugl/cugl.h>

using namespace cugl;

class TrapModel {
#pragma mark State
private:
    /** Position of the hunter */
    cugl::Vec2 _position;
    
    bool _triggered = false;
    int _age = 0;
    int _maxAge;
    int _trigger_time = 0;
    
#pragma mark Main Functions
public:
    /**
     * Constructor for the hunter model
     *
     * @param position the hunter's position
     * @param direction the hunter's direction
     * @param speed the hunter's movement speed
     */
    
    TrapModel(){};
    
    TrapModel(Vec2 position, int maxAge) {
        setPosition(position);
        _maxAge = maxAge;
    };
    
#pragma mark Getters
public:
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    Vec2 getPosition() {
        return _position;
    }

#pragma mark Setters
public:
    /**
     * Sets position for this trap
     *
     * @param position trap position
     */
    void setPosition(cugl::Vec2 position) {
        _position = position;
    }
    
    /**
     * Sets trigger for this trap
     *
     * @param position trap triggered
     */
    void setTrigger(bool isTriggered) {
        _triggered = isTriggered;
    }
    
    bool getTrigger() {
        return _triggered;
    }
    

    
    void updateTrigger(){
        if(_triggered){
            _trigger_time++;
            if(_trigger_time>150){
                _triggered = false;
                _age = _maxAge;
            }
        }

    }
    
    bool update() {
        updateTrigger();
        _age++;
        if(_age > _maxAge) {
            return true;
        }
        return false;
        
        
    }
};


#endif /* TrapModel_hpp */
