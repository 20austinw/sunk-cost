//
//  PortraitSetController.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __PORTRAIT_SET_CONTROLLER_H__
#define __PORTRAIT_SET_CONTROLLER_H__
#include <cugl/cugl.h>
#include <memory>
#include "PortraitSet.h"
#include "CameraController.h"

using namespace cugl;

class PortraitSetController {
  #pragma mark Internal References
  private:
    /** The set of portraits */
    std::vector<std::unique_ptr<CameraController>> _portraits;
    /** The Camera object */
    std::unique_ptr<PortraitSet> _model;
    

  #pragma mark Main Functions
  public:
    /**
     * Constructor for Camera Controller
     */
    PortraitSetController(int id = 0) {
      _model = std::make_unique<PortraitSet>();
      initializePortraitSet();
    }

    /**
     * Adds a new portrait to the portraitset
     * 
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */ 
    void addPortrait(int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int type = 2) {
      _portraits.push_back(std::move(makePortrait(id, position, direction, directionLimits, type)));
    }

    /**
     * Adds a new portrait to the portraitset
     * 
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */ 
    void insertPortraitTo(int index, int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int type = 2) {
      _portraits.insert(getIteratorForIndex(index), std::move(makePortrait(id, position, direction, directionLimits, type)));
    }

    /**
     * Initializes the portrait set
     * 
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */ 
    void initializePortraitSet(int id = 0, Vec3 position = Vec2::ZERO, Vec3 direction = Vec3::ZERO, Vec2 directionLimits = Vec2::ZERO, int type = 2) {
      _portraits.push_back(std::move(makePortrait(id, position, direction, directionLimits, type)));
      _model->setIndex(0);
    }

    /**
     * Adds a new portrait to the portraitset
     */ 
    void clearPortraitSet() {
      _portraits.clear();
      _model->setIndex(-1);
    }

  #pragma mark Getters
  public:
    /** 
     * Getter for viewport of this camera 
     * 
     * @return Rectangle that contains the camera viewpoint
     */
    const Rect getViewPortrait() {
      return _portraits[_model->getIndex()]->getViewPort();
    }

    /** 
     * Getter for current camera index
     * 
     * @return Rectangle that contains the camera viewpoint
     */
    int getIndex() {
      return _model->getIndex();
    }

  #pragma mark Main Functions
  public:
    /** 
     * Updates camera position
     * 
     * @return viewport
     */
    void updatePositionIndex(Vec3 position, int index) {
      _portraits[index]->updatePosition(position);
    }

    /** 
     * Updates direction of the camera
     */
    void updateDirectionIndex(Vec3 direction, int index) {
      _portraits[index]->updateDirection(direction);
    }

    /** 
     * Updates camera direction limits. [d.x,d.y] should be the range of possible values for angle.
     */
    void updateDirectionLimitsIndex(Vec2 directionLimits, int index) {
      _portraits[index]->updateDirectionLimits(directionLimits);
    }

    /** 
     * Updates camera type for the camera at the given index
     */
    void updateTypeIndex(CameraType type, int index) {
      _portraits[index]->updateType(type);
    }

    /**
     * Updates camera type for the current camera
     */
    void updateType(CameraType type) {
      _portraits[_model->getIndex()]->updateType(type);
    }

    /**
     * Rotates the camera by angle
     * 
     * @param angle how much the camera will be rotated by
     */ 
    void rotate(float angle) {
      _portraits[_model->getIndex()]->rotate(angle);
    }

    /**
     * Modifies the camera to look at the given target
     * 
     * @param target the point to look at
     */
    void lookAt(const Vec3 target) {
      _portraits[_model->getIndex()]->lookAt(target);
    }

  #pragma mark Helpers
  private: 
    std::unique_ptr<CameraController> makePortrait(int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int type) {
      std::unique_ptr<CameraController> camera = std::move(std::make_unique<CameraController>(id));
      camera->updatePosition(position);
      camera->updateDirection(direction);
      camera->updateDirectionLimits(directionLimits);
      camera->updateType(CameraType(0));
      return camera;
    }

    std::vector<std::unique_ptr<CameraController>>::iterator getIteratorForIndex(int idx) {
      return _portraits.begin() + idx;
    }
};




#endif /* __PORTRAIT_SET_CONTROLLER_H__ */
