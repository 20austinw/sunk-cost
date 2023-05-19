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
    
    //0: front; 1: left; 2: right
    int _type;

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

    PortraitModel(Vec2 position, int type) {
        setPosition(position);
        setType(type);
    };

#pragma mark Getters
  public:
    /**
     * Updates  position of the portrait
     *
     * @param position  portrait's new position
     */
    Vec2 getPosition() { return _position; }
    
    int getType() {
        return _type;
    }

#pragma mark Setters
  public:
    /**
     * Sets position for this potrait
     *
     * @param position portrait position
     */
    void setPosition(cugl::Vec2 position) { _position = position; }
    
    void setType(int type) {
        _type = type;
    }
};
#endif /* PortraitModel_h */
