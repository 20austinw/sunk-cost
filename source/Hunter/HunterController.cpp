//
//  HunterController.cpp
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//
#include "HunterController.h"
#include "HunterModel.h"
#include "HunterView.h"

#pragma mark Main Functions
/**
 * TODO: Implement Me
 * The constructor should set up the model, view, and camera controller
 */
HunterController::HunterController(const std::shared_ptr<cugl::AssetManager>& assets, Size screenSize, const std::shared_ptr<cugl::Scene2> scene, Vec2 playerSize, int color,float scale) {


    _model = std::make_shared<HunterModel>(assets, scene,scale);
    _model->setPosition(Vec2(10000,10000));

    _scene = scene;

    _view = std::make_unique<HunterView>(assets, Vec2(10000,10000), playerSize, color);

    _screenSize = screenSize;
    // A default camera ID = 1 if not specified
    _camera = std::make_unique<CameraController>(1, _screenSize);
    _camera->updatePosition(Vec3(5, 0, 15));

    _hideCool = 0.0;
    _pos = _model->position;
    _ang = 0;
    _dAng = 0;
    _vel = Vec2(7, 7);
}

bool HunterController::detectedDoor(cugl::Vec2 position){
    if (abs(_model->getPosition().x-position.x)<300 && abs(_model->getPosition().y-position.y)<300){
        return true;
    }
    return false;
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

void HunterController::setViewFrame(float forward, float right) {
    _view->advanceFrame(forward, right);
}

std::vector<std::shared_ptr<TrapModel>> HunterController::getTraps(){
    return _model->getTraps();
}

std::vector<std::shared_ptr<TrapView>> HunterController::getTrapViews(){
    return _model->getTrapViews();
}


/**
 * TODO: Implement Me
 * This function should (for gameplay prototype)
 * (1) detect camera change
 * (2) modify camera to reflect the change
 */
void HunterController::update() {
    _input.readInput();
    //move(_input.getForward(), _input.getRight());
    applyForce(Vec2(_input.getForward(), _input.getRight()));
    
    CULog("x:%f", _model->getPosition().x);
    CULog("y:%f", _model->getPosition().y);
}



void HunterController::applyForce(cugl::Vec2 force) {
    _model->applyForce(force);
    }


/**
 * Moves the hunter by the specified amount.
 *
 * Forward is the amount to move forward, while turn is the angle to turn the
 * ship (used for the "banking" animation. This method performs no collision
 * detection. Collisions are resolved afterwards.
 *
 * @param forward    Amount to move forward
 * @param rightward        Amount to move the hunter rightward
 */
// void HunterController::move(float forward, float rightward) {
//     // Process the hunter thrust.
//     if (forward != 0.0f) {
//         // Thrust key pressed; increase the hunters velocity.
//         float rads = M_PI*_ang/180.0f+M_PI_2;
//         Vec2 dir(cosf(rads),sinf(rads));
//         _vel += dir * forward;
//     }
//     if (_vel.length() > 10.f) {
//         _vel.normalize();
//         _vel = 10.0f*_vel;
//     }
//
//     // Move the hunter, updating it.
//     // Adjust the angle by the change in angle
//     setAngle(_ang+_dAng);
//
//     // INVARIANT: 0 <= ang < 360
//     if (_ang > 360)
//         _ang -= 360;
//     if (_ang < 0)
//         _ang += 360;
//
//     // Move the hunter position by the hunter velocity
//     _pos += _vel;
//
// }
void HunterController::move(float forward, float rightward) {
    
    applyForce(Vec2(forward, rightward));
    _view->setPosition(_model->getPosition());
}

Vec2 HunterController::getPosition() { return _model->getPosition(); }

std::shared_ptr<HunterModel> HunterController::getModel(){return _model;}

void HunterController::setPosition(Vec2 position) { _model->setPosition(position);
    _view->setPosition(position);
    
}



b2Body* HunterController::getHunterBody(){
    return _model->getBody();
}

void HunterController::setAsObstacle(std::shared_ptr<cugl::physics2::ObstacleWorld> world){
    world->addObstacle(_model);
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
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> vec =
        _view->getSpriteNode();

    for (int i = 0; i < vec.size(); i++) {
        scene->addChild(vec[i]);
    }
}

/**
 * Removes the TilemapView child from the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to remove the view from
 */
void HunterController::removeChildFrom(
    const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> vec =
        _view->getSpriteNode();

    for (int i = 0; i < vec.size(); i++) {
        scene->removeChild(vec[i]);
    }
}
