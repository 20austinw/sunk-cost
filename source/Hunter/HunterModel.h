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
#include "TrapModel.hpp"
#include "TrapView.h"

using namespace cugl;

class HunterModel : public cugl::physics2::WheelObstacle {
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
    HunterModel(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::Scene2> scene,float scale) :
    position(_position),
    direction(_direction),
    speed(_speed),
    hideCool(_hideCool) {

        // Call the parent's initializer
        physics2::WheelObstacle::init(Vec2(10000,10000), 10);
        // Set physics properties for the body
        setBodyType(b2_dynamicBody);
        setDensity(DEFAULT_DENSITY);
        //setFriction(DEFAULT_FRICTION);
        //setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
//        setDebugColor(Color4::RED);
        //setPosition(Vec2(100000,100000)*scale);
//        setDirection(_direction);
        //setSpeed(_speed);
        _assets = assets;
        _scene = scene;
    }

#pragma mark Getters
  public:
    /**
     * Get position of the hunter
     */
    Vec2 getPosition() {
        return _position;
    }
    /**
     * Get trap models
     */
    std::vector<std::shared_ptr<TrapModel>> getTraps(){
        return _trapModels;
    }
    /**
     * Get trap views
     */
    std::vector<std::shared_ptr<TrapView>> getTrapViews(){
        return _trapViews;
    }
    
    int getTrapSize(){
        return _trapModels.size();
    }
    
#pragma mark Setters
  public:
    /**
     * Sets position for this hunter
     *
     * @param position hunter position
     */
//    void setPosition(cugl::Vec2 position) { _position = position;
//
//    }
    
//    void setBody(cugl::Vec2 position){
//        _body->SetTransform(b2Vec2(position.x,position.y),0);
//        setPosition(Vec2(_body->GetPosition().x,_body->GetPosition().y));
//    }

    /**
     * Sets direction for this hunter
     *
     * @param direction hunter direction
     */
    void setDirection(cugl::Vec2 direction) { _direction = direction; }

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
        trap->setVisible(false);
    }
    
    void removeTrap(int index){
        _trapViews[index]->removeChildFrom(_scene);
        _trapModels.erase(_trapModels.begin()+index);
    }
    
    void applyForce(cugl::Vec2 force) {
            // Push the player in the direction they want to go
            b2Vec2 b2force(force.y*50000, force.x*50000);
//        CULog(" forceeee x%f",force.x);
//        CULog(" forceeee y%f",force.y);
            
            // If the player has reached max speed
            if (getLinearVelocity() >= Vec2(20,20)) {
                _body->SetLinearVelocity(b2force);
                _body->ApplyForceToCenter(b2force, true);
            }
            else{
                _body->SetLinearVelocity(b2force);
                _body->ApplyForceToCenter(b2force, true);
            }
            float a=_body->GetPosition().x;
            float b=_body->GetPosition().y;
            _position = Vec2(a,b);
        CULog("position xxxxxxx%f",_position.x);
        CULog("position yyyyyyyy%f",_position.y);
        
        

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
