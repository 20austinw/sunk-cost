//
//  TreasureModel.hpp
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#ifndef TreasureModel_hpp
#define TreasureModel_hpp

#include <stdio.h>

/** The density of this player */
#define DEFAULT_DENSITY 1.0f
/** The friction of this player */
#define DEFAULT_FRICTION 0.4f
/** The restitution of this player */
#define DEFAULT_RESTITUTION 0.4f

#include <cugl/cugl.h>

using namespace cugl;

class TreasureModel : public cugl::physics2::WheelObstacle {
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

    TreasureModel(){};

    TreasureModel(Vec2 position, int maxAge) {
        _maxAge = maxAge;

        // Call the parent's initializer
        physics2::WheelObstacle::init(position, 20);

        // Set physics properties for the body
        setBodyType(b2_staticBody);
        setDensity(DEFAULT_DENSITY);
        setFriction(DEFAULT_FRICTION);
        setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
        setDebugColor(Color4::RED);
        //        setPosition(position);
        _position = position;
    };

#pragma mark Getters
  public:
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    Vec2 getPosition() { return _position; }

#pragma mark Setters
  public:
    /**
     * Sets position for this trap
     *
     * @param position trap position
     */
    void setPosition(cugl::Vec2 position) { _position = position; }

    /**
     * Sets trigger for this trap
     *
     * @param position trap triggered
     */
    void setTrigger(bool isTriggered) { _triggered = isTriggered; }

    bool getTrigger() { return _triggered; }

    void updateTrigger() {
        if (_triggered) {
            _trigger_time++;
            if (_trigger_time > 150) {
                _triggered = false;
                _age = _maxAge;
            }
        }
    }

    bool update() {
        updateTrigger();
        _age++;
        if (_age > _maxAge) {
            return true;
        }
        return false;
    }
};

#endif /* TreasureModel_hpp */
