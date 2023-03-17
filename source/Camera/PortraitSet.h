//
//  PortraitSet.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __PORTRAIT_SET_H__
#define __PORTRAIT_SET_H__
#include <cugl/cugl.h>

using namespace cugl;

class PortraitSet {
#pragma mark State
private:
  /** The index for the camera that is being viewed. */
  int _index;

#pragma mark Main Fucntions
public:
  /**
   * Constructor for Camera Controller
   */
  PortraitSet() : _index{-1} {}

#pragma mark Getters
public:
  /**
   * Getter for index of current portrait
   */
  int getIndex() { return _index; }

#pragma mark Setters
public:
  /**
   * Setter for index of current portrait
   *
   * @param index index of the new portrait that is being viewed.
   * -1 if none.
   */
  void setIndex(int index) { _index = index; }
};

#endif /* __PORTRAIT_SET_H__ */
