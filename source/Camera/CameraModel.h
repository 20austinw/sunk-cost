//
//  CameraModel.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/21/23
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
  
  #pragma mark Internal References
  private: 
    
    

  #pragma mark State
  private: 
    /** Portrait or Viewpoint or Default */
    CameraType _type; 
    /** Limits on angles for the camera. [d.x,d.y] should be the range of possible values for angle. */
    Vec2 _directionLimits;

  public:
    CameraType& type;
    Vec2& directionLimits;

  #pragma mark Main Functions
  public:
    /** 
     * Default Constructor for Camera Model
     */
    Camera() :
    type(_type),
    directionLimits(_directionLimits) {
      setDirectionLimits(Vec2::ZERO);
      setType(0);
    }

  #pragma mark Setters
  // Note: Call update after updating camera
  public:
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
    }
}





#endif /* __CAMERA_MODEL_H__ */
