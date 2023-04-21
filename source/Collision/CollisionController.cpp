//
//  CollisionController.cpp
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#include "CollisionController.hpp"
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>
//  MARK: - Constructors

/**
 * Initializes a Collision Controller
 */
//bool init(b2Body* hunterBody, b2Body* trapBody, b2Body* treasureBody);
bool CollisionController::init( b2Body* hunterBody, b2Body* treasureBody) {
//    CULog("collision init!!");
    _hunterBody = hunterBody;
//    _trapBody = trapBody;
    _treasureBody = treasureBody;
    didHitObstacle = false;
    didHitTreasure = false;
    didHitTrap = false;
    return true;
}

//  MARK: - Callbacks

/**
 * Callback for when two obstacles in the world begin colliding
 */
void CollisionController::beginContact(b2Contact* contact) {
//    b2Body* body1 = contact->GetFixtureA()->GetBody();
//    b2Body* body2 = contact->GetFixtureB()->GetBody();
////    CULog("Collision begin Contact");
//    if (body1 == _hunterBody || body2 == _hunterBody) {
//        CULog("Hunter body here");
//        didHitObstacle = true;
//    }
//
//    if (body1 == _treasureBody || body2 == _treasureBody) {
//        CULog("Treasure body here");
//        didHitTreasure = true;
//    }

    // Check all of the traps

//    if ((_trapBody == body1 && _hunterBody == body2) ||
//        (_trapBody == body2 && _hunterBody == body1)) {
//        CULog("TRAP body here!");
//        didHitTrap = true;
//    }

//
//    didHitObstacle = didHitObstacle && !didHitTrap;
}

/**
 * Callback for when two obstacles in the world end colliding
 */
void CollisionController::endContact(b2Contact* contact) {
//    b2Body* body1 = contact->GetFixtureA()->GetBody();
//    b2Body* body2 = contact->GetFixtureB()->GetBody();
//    didHitTrap = false;
//    didHitTreasure = false;


}

/**
 * Callback for determining if two obstacles in the world should collide.
 */
bool CollisionController::shouldCollide(b2Fixture* f1, b2Fixture* f2) {
    const b2Filter& filterA = f1->GetFilterData();
    const b2Filter& filterB = f2->GetFilterData();

    return filterA.maskBits & filterB.maskBits;
}
