//
//  HunterController.h
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/2023
//

#ifndef _HUNTER_CONTROLLER_H
#define _HUNTER_CONTROLLER_H

#include "../Camera/CameraController.h"
#include "../Input/InputController.h"
#include "HunterModel.h"
#include "HunterView.h"
#include <cugl/cugl.h>

using namespace cugl;

class HunterController {
#pragma mark Internal References
  private:
    /** Model reference */
    std::shared_ptr<HunterModel> _model;
    /** View reference */
    std::unique_ptr<HunterView> _view;

    /** Position of the hunter */
    cugl::Vec2 _pos;
    /** Velocity of the hunter */
    cugl::Vec2 _vel;
    cugl::Vec2 _scale;

    // The following are protected, because they have no accessors
    /** Current angle of the hunter */
    float _ang;
    /** Accumulator variable to turn faster as key is held down */
    float _dAng;
    
    std::shared_ptr<cugl::Scene2> _scene;

#pragma mark External References
  private:
    /** Camera reference */
    std::unique_ptr<CameraController> _camera;
    /** The controller to manage the hunter */
    InputController _input;
    Size _screenSize;
    

#pragma mark Constants
  private:
    /** The preset cooltime for hiding */
    float _hideCool;

#pragma mark Main Functions
  public:
    HunterController() {}

    /**
     * TODO: Implement Me
     * The constructor should set up the model, view, and camera controller
     */

    HunterController(const std::shared_ptr<cugl::AssetManager>& assets, Size screenSize, const std::shared_ptr<cugl::Scene2> scene, Vec2 playerSize, int color,float scale);
    
    /**
     * Gets the viewpoint for the hunter's camera
     *
     * @return rectangle that should be drawn
     */
    const Rect getCameraView() { return _camera->getViewPort(); }

    /**
     * TODO: Implement Me
     * This function should
     * (1) get the view from CameraController
     * (2) attach the viewport to the scene
     */
    void render(const std::shared_ptr<cugl::SpriteBatch>& batch);

    /**
     * TODO: Implement Me
     * This function should (for gameplay prototype)
     * (1) detect camera change
     * (2) modify camera to reflect the change
     */

    void setViewFrame(float forward, float right);

    void update();

    bool detectedDoor(cugl::Vec2 position);
    
    void collisionWithTrap();
  
    std::vector<std::shared_ptr<TrapModel>> getTraps();
    
    std::vector<std::shared_ptr<TrapView>> getTrapViews();
    
    void applyForce(cugl::Vec2 force);

#pragma mark Setters
  public:
    /**
     * Updates  position of the hunter
     *
     * @param position  hunter's new position
     */
    void updatePosition(cugl::Vec2 position) {
        _model->setPosition(position);
        _view->setPosition(position);
    }

    /**
     * Updates remaining hide cooldown time for this hunter
     *
     * @param hideCool the new remaining cooltime
     */
    void updateHideCooldown(float hideCool) {
        _model->setHideCooldown(hideCool);
    }

    /**
     * Sets the angle that this hunter is facing.
     *
     * The angle is specified in radians. The angle is counter clockwise
     * from the line facing east.
     *
     * @param value the angle of the hunter
     */
    void setAngle(float value) { _ang = value; }

    /**
     * Moves the hunter by the specified amount.
     *
     * @param forward   Amount to move forward
     * @param rightward   Amount to move the hunter rightwards
     */
    void move(float forward, float rightward);
    Vec2 getPosition();
    
    std::shared_ptr<HunterModel> getModel();
    
    void setPosition(Vec2 position);
    
    b2Body* getHunterBody();
    
    void setAsObstacle(std::shared_ptr<cugl::physics2::ObstacleWorld> world);
#pragma mark View Methods
    /**
     * Adds the TilemapView as a child to the given `scene`.
     *
     * The TilemapView is the view reference held within the model.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);

    /**
     * Removes the TilemapView child from the given `scene`.
     *
     * The TilemapView is the view reference held within the model.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);
    
    void addTrap(Vec2 position) { _model->addTrap(position); }
    
    void removeTrap(int index) { _model->removeTrap(index); }
    
    int getTrapSize(){ return _model->getTrapSize();}
};
#endif /* _HUNTER_CONTROLLER_H__ */
