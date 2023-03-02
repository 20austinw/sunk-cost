//
//  SpiritController.cpp
//  Sunk Cost
//
//  This module provides the SpiritController class.
//
//  Author: Soon Jae Park
//  Version: 2/22/23.
//
#include "SpiritController.h"

#pragma mark Main Functions
  /**
   * TODO: Implement Me
   * The Constructor should set up the preset cooltimes as well as all other class variables.
   */
  SpiritController::SpiritController() {
      // Placeholder values
      _cameraCool = 5;
      _clamCool = 20;
      _doorCool = 20;
      _model = std::make_unique<SpiritModel>(NUM_CLAMS, NUM_DOORS, INITIAL_ENERGY);
      _portraits = std::make_unique<PortraitSetController>(NUM_ROOMS);
  }

  /**
   * TODO: Implement Me (Not for Gameplay Prototype)
   */
  Vec2 SpiritController::closeDoors() {
      return Vec2::ZERO;
  }

  /**
   * TODO: Implement Me (Not for Gameplay Prototype)
   */
  Vec2 SpiritController::placeTraps() {
      return Vec2::ZERO;
  }

  /**
     * TODO: Implement Me
     * This function should
     * (1) get the view from portraitsetcontroller
     * (2) attach the viewport to the scene
     */
void SpiritController::render(Scene2 &scene) {
    scene.addChild(scene2::PolygonNode::allocWithPoly(getCameraView()));
}

  /**
   * TODO: Implement Me
   * This function should (for gameplay prototype)
   * (1) detect camera change
   * (2) modify portraitsetcontroller to reflect the change
   */
void SpiritController::update() {
    
}

