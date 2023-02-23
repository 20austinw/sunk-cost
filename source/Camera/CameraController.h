//
//  CameraController.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __CAMERA_CONTROLLER_H__
#define __CAMERA_CONTROLLER_H__
#include <cugl/cugl.h>
#include "CameraModel.h"


using namespace cugl;

enum CameraType {Default = 0, Player = 1, Portrait = 2};

class CameraController {
  #pragma mark Internal References
  private:
    /** The Camera object */
    std::shared_ptr<Camera> _camera;
    /** Camera Model */
    std::unique_ptr<CameraModel> _model;
      
  #pragma mark Main Functions
  public:
    /**
     * Constructor for Camera Controller
     */
    CameraController(int id) :
    // Need to test out orthographic camera vs perspective
    // Orthographic sounds like the better option
    _camera{std::make_shared<OrthographicCamera>()},
    _model{std::make_unique<CameraModel>(id)}{}

    /**
     * Rotates the camera by angle
     * 
     * @param angle how much the camera will be rotated by
     */ 
    void rotate(float angle) {
      _camera->rotate(angle);
    }

    /**
     * Modifies the camera to look at the given target
     * 
     * @param target the point to look at
     */
    void lookAt(const Vec3 target) {
      _camera->normalizeUp();
      _camera->lookAt(target);
    }

  #pragma mark Getters
  public:
    /** 
     * Getter for viewport of this camera 
     * 
     * @return Rectangle that contains the camera viewpoint
     */
    const Rect getViewPort() {
        return _camera->getViewport();
    }
    

  #pragma mark Setters
  public:
    /** 
     * Updates camera position
     * 
     * @return viewport
     */
    void updatePosition(Vec3 position) {
      _camera->setPosition(position);
    }

    /** 
     * Updates direction of the camera
     */
    void updateDirection(Vec3 direction) {
      _camera->setDirection(direction);
    }

    /** 
     * Updates camera directoin limits. [d.x,d.y] should be the range of possible values for angle.
     */
    void updateDirectionLimits(Vec2 directionLimits) {
      _model->setDirectionLimits(directionLimits);
    }

    /** 
     * Updates camera type
     */
    void updateType(CameraType type) {
      _model->setType(type);
    }
};

#endif /* __CAMERA_CONTROLLER_H__ */
