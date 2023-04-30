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

#include "TrapModel.hpp"
#include "TrapView.h"
#include <cugl/cugl.h>

using namespace cugl;

class HunterModel : public physics2::WheelObstacle {
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

    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<cugl::Scene2> _scene;

    std::vector<std::shared_ptr<TrapModel>> _trapModels;
    std::vector<std::shared_ptr<TrapView>> _trapViews;

    int _size1;

  public:
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
    HunterModel(const std::shared_ptr<cugl::AssetManager>& assets,
                std::shared_ptr<cugl::Scene2> scene, float scale)
        : position(_position), direction(_direction), speed(_speed),
          hideCool(_hideCool) {

        // Call the parent's initializer
        physics2::WheelObstacle::init(Vec2(42, 24), 1);
        // Set physics properties for the body
        setBodyType(b2_dynamicBody);
        setDensity(DEFAULT_DENSITY);
        // setFriction(DEFAULT_FRICTION);
        // setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
        //        setDebugColor(Color4::RED);
        //        setDirection(_direction);
        // setSpeed(_speed);
        _assets = assets;
        _scene = scene;
        _size1 = 0;
    }

  public:
    /**
     * Get position of the hunter
     */
    Vec2 getPosition() { return _position; }
    /**
     * Get trap models
     */
    std::vector<std::shared_ptr<TrapModel>> getTraps() { return _trapModels; }
    /**
     * Get trap views
     */
    std::vector<std::shared_ptr<TrapView>> getTrapViews() { return _trapViews; }

    int getTrapSize() { return _size1; }

    /**
     * Sets direction for this hunter
     *
     * @param direction hunter direction
     */
    // void setDirection(cugl::Vec2 direction) { _direction = direction; }
    void setPosition(Vec2 position) {
        //        if(getBody()!=nullptr){
        //            getBody()->SetTransform(b2Vec2(position.x, position.y),
        //            0);
        //        }
        _position = position;
    }
    /**
     * Sets speed for this hunter
     *
     * @param speed hunter movement speed
     */
    void setSpeed(float speed) { _speed = speed; }

    /**
     * Sets cooldown time for hiding
     *
     * @param hideCool cooldown time for hiding
     */
    void setHideCooldown(float hideCool) { _hideCool = hideCool; }

    void addTrap(Vec2 position) {
        if (_trapModels.size() >= 3)
            return;
        _trapModels.emplace_back(std::make_shared<TrapModel>(position, 300));
        auto trap = std::make_shared<TrapView>(_assets, position, 20);
        _trapViews.emplace_back(trap);
        trap->addChildTo(_scene);
        trap->setVisible(false, 0);
        _size1++;
    }

    void removeTrap(int index) {
        _trapModels.erase(_trapModels.begin() + index);
        _trapViews.erase(_trapViews.begin() + index);
        _size1--;
    }

    void applyForce(cugl::Vec2 force) {
        // Push the player in the direction they want to go
        b2Vec2 b2force(0.05 * force.x, 0.05 * force.y);

        // If the player has reached max speed
        if (getLinearVelocity() >= Vec2(20, 20)) {

            _body->SetLinearVelocity(b2force);
            // _body->ApplyForceToCenter(b2force, true);
        } else {
            CULog("ELSE ");
            _body->SetLinearVelocity(b2force);
            //  _body->ApplyForceToCenter(b2force, true);
        }
        float a = _body->GetPosition().x;
        float b = _body->GetPosition().y;
        _position = 100 * Vec2(a, b);
        // CULog("position xxxxxxx%f",_position.x);
        // CULog("position yyyyyyyy%f",_position.y);
    }

    void update() {
        std::vector<std::shared_ptr<TrapModel>> pendingTrapModels;
        std::vector<std::shared_ptr<TrapView>> pendingTrapViews;
        for (int i = 0; i < _trapModels.size(); i++) {
            if (!_trapModels[i]->update()) {
                pendingTrapModels.emplace_back(_trapModels[i]);
                pendingTrapViews.emplace_back(_trapViews[i]);
                _trapViews[i]->update();
            } else {
                _trapViews[i]->removeChildFrom(_scene);
            }
        }
        _trapModels = pendingTrapModels;
        _trapViews = pendingTrapViews;
    }
};

#endif /* _HUNTER_MODEL_H__ */
