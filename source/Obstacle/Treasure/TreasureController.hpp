//
//  TreasureController.hpp
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#ifndef TreasureController_hpp
#define TreasureController_hpp

#include <stdio.h>

#include <cugl/cugl.h>
#include "TreasureModel.hpp"
#include "TreasureView.h"

using namespace cugl;

class TreasureController{
#pragma mark Internal References
private:
    /** Model reference */
    std::shared_ptr<TreasureModel> _model;
    /** View reference */
    std::unique_ptr<TreasureView> _view;
    Size _screenSize;
public:
    TreasureController() {};
    
    TreasureController(const std::shared_ptr<cugl::AssetManager>& assets, Size screenSize);
    
    std::shared_ptr<TreasureModel> getModel();
    void setViewFrame();
    Vec2 getPosition();
    void setPosition(cugl::Vec2 position);
    bool update();
    void setTrigger(bool isTriggered);
    bool getTrigger();
    b2Body* getTreasureBody();
    std::shared_ptr<scene2::PolygonNode> getNode();
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);
    void setAsObstacle(std::shared_ptr<cugl::physics2::ObstacleWorld>);
    
};




#endif /* TreasureController_hpp */
