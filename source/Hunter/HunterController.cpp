//
//  HunterController.cpp
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//
#include "HunterModel.h"
#include "HunterView.h"
#include "HunterController.h"

#pragma mark Main Functions
/**
 * TODO: Implement Me
 * The constructor should set up the model, view, and camera controller
 */
HunterController::HunterController(const std::shared_ptr<cugl::AssetManager>& assets) {
    _model = std::make_unique<HunterModel>();
    _model->setPosition(Vec2(400,400));
    _view = std::make_unique<HunterView>(assets, Vec2(400,400), Vec2(40,40));
    
    //A default camera ID = 1 if not specified
    _camera = std::make_unique<CameraController>(1);
    _camera->updatePosition(Vec3(5,0,15));

    _hideCool = 0.0;
    _pos = _model->position;
    _ang  = 0;
    _dAng = 0;
    _vel = Vec2(7,7);
   
}

/**
 * TODO: Implement Me
 * This function should
 * (1) get the view from CameraController
 * (2) attach the viewport to the scene
 */
void HunterController::render(const std::shared_ptr<cugl::SpriteBatch>& batch) {
    // Set the viewport as the rendering target
    batch->begin();
    batch->setPerspective(_camera->getView());
    
    batch->end();
//        // Render the scene
//        scene->render(cugl::Application::get()->getAssets()->getRenderer());
//
//        // End rendering to the viewport
//        cugl::Application::get()->getAssets()->getRenderer()->drawEnd();

}

/**
 * TODO: Implement Me
 * This function should (for gameplay prototype)
 * (1) detect camera change
 * (2) modify camera to reflect the change
 */
void HunterController::update() {
    _input.readInput();
    move(_input.getForward(),_input.getRight());
    
}


/**
 * Moves the hunter by the specified amount.
 *
 * Forward is the amount to move forward, while turn is the angle to turn the ship
 * (used for the "banking" animation. This method performs no collision detection.
 * Collisions are resolved afterwards.
 *
 * @param forward    Amount to move forward
 * @param rightward        Amount to move the hunter rightward
 */
//void HunterController::move(float forward, float rightward) {
//    // Process the hunter thrust.
//    if (forward != 0.0f) {
//        // Thrust key pressed; increase the hunters velocity.
//        float rads = M_PI*_ang/180.0f+M_PI_2;
//        Vec2 dir(cosf(rads),sinf(rads));
//        _vel += dir * forward;
//    }
//    if (_vel.length() > 10.f) {
//        _vel.normalize();
//        _vel = 10.0f*_vel;
//    }
//
//    // Move the hunter, updating it.
//    // Adjust the angle by the change in angle
//    setAngle(_ang+_dAng);
//
//    // INVARIANT: 0 <= ang < 360
//    if (_ang > 360)
//        _ang -= 360;
//    if (_ang < 0)
//        _ang += 360;
//
//    // Move the hunter position by the hunter velocity
//    _pos += _vel;
//
//}
void HunterController::move(float forward, float rightward) {
    // Process the hunter thrust.
//    float pos.x+=rightward*_vel;
//    _pos.y += forward*_vel;
    Vec2 pos = _model->getPosition();
//     int posx;
//     int posy;
//     Vec3 currPos = (pos-Vec2(280,90));
//     posx =(int) (currPos.x)/45;
//     posy=(int)((currPos.y))/45;
    
//     if(posx>15 ){
//         pos.x-=12;
        
//     }else if(posx<=0 ){
//         pos.x+=12;
        
//     }
//     else if(posy>11 ){
//         pos.y -= 12;
//     }else if(posy<=-0){
//         pos.y+=12;
//     }
//     else{
        pos.x+=rightward*_vel.x;
        pos.y += forward*_vel.y;
 //   }
    _model->setPosition(pos);
    _view->setPosition(pos);
    
    if (forward == 1 && rightward == 1) {
        // Thrust key pressed; increase the hunters velocity.
        setAngle(45);

    }else if (forward == 1 && rightward == 0){
        setAngle(0);
    }else if (forward == 1 && rightward == -1){
        setAngle(315);
    }else if (forward == 0 && rightward == -1){
        setAngle(270);
    }else if (forward == 0 && rightward == 1){
        setAngle(90);
    }else if (forward == -1 && rightward == 1){
        setAngle(135);
    }else if (forward == -1 && rightward == 0){
        setAngle(180);
    }else if (forward == -1 && rightward == -1){
        setAngle(225);
    }
    
}

Vec2 HunterController::getPosition() {
    return _model->getPosition();
}

#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to add the view to
 */
void HunterController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
    scene->addChild(_view->getNode());
}


/**
 * Removes the TilemapView child from the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to remove the view from
 */
void HunterController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    scene->removeChild(_view->getNode());
}







