//
//  CameraModel.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __CAMERA_MODEL_H__
#define __CAMERA_MODEL_H__
#include <cugl/cugl.h>

using namespace cugl;

/**
 * A class representing a single camera
 * A camera is an object that can provide a viewpoint, so it includes portraits and hunter viewpoint as well as default viewpoint
 */
class CameraModel {
  #pragma mark State
  private:
    /** Portrait or Viewpoint or Default */
    int _type;
    /** Limits on angles for the camera. [d.x,d.y] should be the range of possible values for angle. */
    Vec2 _directionLimits;
    /** Camera ID, for ordering purpose in Portrait Set */
    int _id;

  #pragma mark External References
  public:
    /** A public accessible, read-only version of the camera type */
    int& type;
    /** A public accessible, read-only version of the direction limits */
    Vec2& directionLimits;
    /** A public accessible, read-only version of the camera id */
    int& id;

  #pragma mark Main Functions
  public:
    /**
     * Constructor for Camera Model
     */
    CameraModel(int id) :
    id(_id),
    directionLimits(_directionLimits),
    type(_type)
     {
      setId(id);
      setDirectionLimits(Vec2::ZERO);
      setType(0);
    }

  #pragma mark Setters
  // Note: Call update after updating camera
  public:
    /**
     * Setter for camera id
     */
    void setId(int id) {
        _id = id;
    }
    
    /**
     * Setter for camera angle bounds
     */
    void setDirectionLimits(Vec2 directionLimits) {
      _directionLimits = directionLimits;
    }

    /**
     * Setter for camera type
     */
    void setType(int type) {
      _type = type;
    };
};

#endif /* __CAMERA_MODEL_H__ */
