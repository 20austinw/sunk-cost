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

    SpiritController::SpiritController() {
        
    }
  /**
   * TODO: Implement Me
   * The Constructor should set up the preset cooltimes as well as all other class variables.
   */
  SpiritController::SpiritController(std::shared_ptr<PortraitSetController> portraits, Size screenSize) {
      _model = std::make_unique<SpiritModel>(3, 3, 30);
      _view = std::make_unique<SpiritView>();
      _portraits = portraits;
      _screenSize = screenSize;
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
    
    // Helper function
    Rect SpiritController::screenToWorld(Rect rect) {
        Size size = rect.size;
        const float WIDTH = _screenSize.width;
        const float HEIGHT = _screenSize.height;
        return Rect(2*rect.getMinX()/WIDTH, 2*rect.getMinY()/HEIGHT, 2*size.width/WIDTH, 2*size.height/HEIGHT);
    }

  /**
     * TODO: Implement Me
     * This function should
     * (1) get the view from portraitsetcontroller
     * (2) attach the viewport to the scene
     */
void SpiritController::render(std::shared_ptr<cugl::SpriteBatch>& batch, Size size) {
//    Rect cameraView = getCameraView();
//    batch->setColor(Color4::YELLOW);
//    batch->fill(screenToWorld(cameraView.translate(-size.width/2, -size.height/2)));
}

  /**
   * TODO: Implement Me
   * This function should (for gameplay prototype)
   * (1) detect camera change
   * (2) modify portraitsetcontroller to reflect the change
   */
void SpiritController::update() {
    
}

