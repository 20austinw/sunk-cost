//
//  HunterModel.h
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//

#ifndef _HUNTER_MODEL_H
#define _HUNTER_MODEL_H
/** The density of this player */
#define DEFAULT_DENSITY 1.0f
/** The friction of this player */
#define DEFAULT_FRICTION 0.4f
/** The restitution of this player */
#define DEFAULT_RESTITUTION 0.4f

#include <cugl/cugl.h>

using namespace cugl;

class HunterModel : public cugl::physics2::CapsuleObstacle {
#pragma mark State
private:
    /** Position of the hunter */
    cugl::Vec2 _position;
    /** Direction the hunter is facing */
    cugl::Vec2 _direction;
    /** Speed of the hunter */
    float _speed;
    /** Cooldown time for hiding */
    float _hideCool;
    
public:
    /** A public accessible, read-only version of the  hunter position */
    cugl::Vec2& position;
    /** A public accessible, read-only version of the  hunter direction  */
    cugl::Vec2& direction;
    /** A public accessible, read-only version of the  hunter speed */
    float& speed;
    /** A public accessible, read-only version of the hiding cooldown */
    float& hideCool;
    
#pragma mark Main Functions
public:
    /**
     * Constructor for the hunter model
     *
     * @param position the hunter's position
     * @param direction the hunter's direction
     * @param speed the hunter's movement speed
     */
    HunterModel() :
    position(_position),
    direction(_direction),
    speed(_speed),
    hideCool(_hideCool) {
        // Call the parent's initializer
        physics2::CapsuleObstacle::init(_position, Vec2(20,20));
        
        // Set physics properties for the body
        setBodyType(b2_dynamicBody);
        setDensity(DEFAULT_DENSITY);
        setFriction(DEFAULT_FRICTION);
        setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
        setDebugColor(Color4::RED);
        setPosition(_position);
        setDirection(_direction);
        setSpeed(0);
        setHideCooldown(0);
    }
    
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
    
    /**
     * Sets direction for this hunter
     *
     * @param direction hunter direction
     */
    void setDirection(cugl::Vec2 direction) {
        _direction = direction;
    }
    
    /**
     * Sets speed for this hunter
     *
     * @param speed hunter movement speed
     */
    void setSpeed(float speed) {
        _speed = speed;
    }
    
    /**
     * Sets cooldown time for hiding
     *
     * @param hideCool cooldown time for hiding
     */
    void setHideCooldown(float hideCool) {
        _hideCool = hideCool;
    }
    
};


#endif /* _HUNTER_MODEL_H__ */
