//
//  DoorController.hpp
//  Sunk
//
//  Created by Xiaoyue Jin on 3/24/23.
//

#ifndef DoorController_hpp
#define DoorController_hpp

#include "DoorModel.h"
#include "DoorView.h"
#include <cugl/cugl.h>
#include <stdio.h>

using namespace cugl;

class DoorController {
#pragma mark Internal References
  private:
    /** Model reference */
    std::shared_ptr<DoorModel> _model;
    /** View reference */
    std::unique_ptr<DoorView> _view;

#pragma mark Main Functions
  public:
    // playerType: 0 for spirit; 1 for hunter
    DoorController(const std::shared_ptr<cugl::AssetManager>& assets,
                   Vec2 position, int type, int playerType) {
        _model = std::make_unique<DoorModel>(position, type);
        _view = std::make_unique<DoorView>(assets, position, type, playerType);
    }

    bool update(bool started, bool released, Vec2 touchPos);

    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        _view->addChildTo(scene);
    }

    void addChildToVector(
        std::vector<std::shared_ptr<scene2::PolygonNode>>& vector) {
        vector.emplace_back(_view->getNode());
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        _view->removeChildFrom(scene);
    }

    void resetToUnlock();

    void resetHunterUnlock();

#pragma mark Getters
    Vec2 getPosition() { return _model->getPosition(); }

    int getFrame() { return _model->getFrame(); }

    std::shared_ptr<DoorModel> getModel() { return _model; }

    Vec2 getModelPosition();

    Vec2 getViewPosition();

    bool isLocked();

    bool isInBound(Vec2 pos);

#pragma mark Setters
    void setFrame(int frame) {
        _model->setFrame(frame);
        _view->setFrame(frame);
    }

#pragma mark Helpers:
  private:
    void updateFrame(Vec2 touchPos);

    void setLockFrame();
};

#endif /* DoorController_hpp */
