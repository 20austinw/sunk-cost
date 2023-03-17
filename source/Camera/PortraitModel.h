//
//  PortraitModel.h
//  Sunk
//
//  Created by Austin Wu on 3/16/23.
//

#ifndef PortraitModel_h
#define PortraitModel_h

#include <cugl/cugl.h>

using namespace cugl;

class PortraitModel {
#pragma mark State
private:
  cugl::Vec2 _position;

#pragma mark Main Functions
public:
  /**
   * Constructor for the portrait model
   *
   * @param position the portrait's position
   * @param direction the portrait's direction
   * @param speed the portrait's movement speed
   */

  PortraitModel(){};

  PortraitModel(Vec2 position) { setPosition(position); };

#pragma mark Getters
public:
  /**
   * Updates  position of the portrait
   *
   * @param position  portrait's new position
   */
  Vec2 getPosition() { return _position; }

#pragma mark Setters
public:
  /**
   * Sets position for this potrait
   *
   * @param position portrait position
   */
  void setPosition(cugl::Vec2 position) { _position = position; }
};
#endif /* PortraitModel_h */
