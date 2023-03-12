//
//  TrapModel.hpp
//  Sunk
//
//  Created by Austin Wu on 3/11/23.
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
    
    bool triggered = false;
    
#pragma mark Main Functions
public:
    /**
     * Constructor for the hunter model
     *
     * @param position the hunter's position
     * @param direction the hunter's direction
     * @param speed the hunter's movement speed
     */
    TrapModel(Vec2 position) {
        setPosition(position);
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
     * Sets position for this hunter
     *
     * @param position hunter position
     */
    void setPosition(cugl::Vec2 position) {
        _position = position;
    }
    
};

#endif /* TrapModel_hpp */
