//
//  CollisionController.hpp
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#ifndef CollisionController_hpp
#define CollisionController_hpp

#include <cugl/cugl.h>
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>


class CollisionController {
protected:
//  MARK: - Properties
    
    // Models
    /** A reference to the model to represent all models within the game */
    b2Body* _hunterBody;
    b2Body* _trapBody;
    b2Body* _treasureBody;
public:
    bool didHitObstacle=false;
    bool didHitTrap=false;
    bool didHitTreasure=false;
    
//  MARK: - Constructors
    
    /**
     * Constructs a Collision Controller
     */
    CollisionController() {};
    
    /**
     * Destructs a Collision Controller
     */
    ~CollisionController() { dispose(); }
    
    /**
     * Disposes of all resources in this instance of Collision Controller
     */
    void dispose() { }
    
    /**
     * Initializes a Collision Controller
     */
    bool init(b2Body* hunterBody, b2Body* treasureBody);
//    bool init(b2Body* hunterBody, b2Body* trapBody, b2Body* treasureBody);
    
    
//  MARK: - Callbacks
    
    /**
     * Callback for when two obstacles in the world begin colliding
     */
    void beginContact(b2Contact* contact);
    
    /**
     * Callback for when two obstacles in the world end colliding
     */
    void endContact(b2Contact* contact);

    /**
     * Callback for determining if two obstacles in the world should collide.
     */
    bool shouldCollide(b2Fixture* f1, b2Fixture* f2);
    
};

#endif /* CollisionController_hpp */

