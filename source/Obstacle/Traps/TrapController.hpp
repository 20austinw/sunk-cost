//
//  TrapController.hpp
//  Sunk
//
//  Created by 任清扬 on 14/3/23.
//

#ifndef TrapController_hpp
#define TrapController_hpp

#include <stdio.h>

#include "TrapModel.hpp"
#include "TrapView.h"
#include <cugl/cugl.h>

using namespace cugl;

class TrapController {
#pragma mark Internal References
  private:
    /** Model reference */
    std::unique_ptr<TrapModel> _model;
    /** View reference */
    std::unique_ptr<TrapView> _view;
    Size _screenSize;

  public:
    TrapController(){};

    TrapController(const std::shared_ptr<cugl::AssetManager>& assets,
                   Size screenSize);

    std::unique_ptr<TrapModel> getModel();
    void setViewFrame();
    Vec2 getPosition();
    void setPosition(cugl::Vec2 position);
    bool update();
    void setTrigger(bool isTriggered);
    bool getTrigger();
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);
};

#endif /* TrapController_hpp */
