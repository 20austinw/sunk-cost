//
//  SpiritController.h
//  Sunk Cost
//
//  This module provides the SpiritController class.
//
//  Author: Soon Jae Park
//  Version: 2/22/23.
//

#ifndef _SPIRIT_CONTROLLER_H
#define _SPIRIT_CONTROLLER_H

#include "../Camera/PortraitSetController.h"
#include "InputController.h"
#include "SpiritModel.h"
#include "SpiritView.h"
#include "TilemapController.h"
#include <cugl/cugl.h>

using namespace cugl;

/** TODO: Implement Me */
class SpiritController {
#pragma mark Internal References
  private:
    /** The model for Spirit */
    std::unique_ptr<SpiritModel> _model;
    std::unique_ptr<SpiritView> _view;

#pragma mark External References
  private:
    /** The set of accessible portraits */
    std::shared_ptr<PortraitSetController> _portraits;
    Size _screenSize;
    std::shared_ptr<cugl::Scene2> _scene;
    bool _trapAdded;
    Vec2 _lastTrapPos;

#pragma mark Constants
  private:
    /** The preset cooltime for cameras */
    float _cameraCool;
    /** The preset cooltime for clams */
    float _clamCool;
    /** The preset cooltime for close doors */
    float _doorCool;

    float CAMERA_COOL = 180;

#pragma mark Main Functions
  public:
    /**
     * TODO: Implement Me
     * The Constructor should set up the preset cooltimes as well as all other
     * class variables.
     */
    SpiritController();

    /**
     * Constructor to initialize SpiritController with PortraitSetController
     */
    SpiritController(const std::shared_ptr<cugl::AssetManager>& assets,
                     std::shared_ptr<cugl::Scene2> scene,
                     std::shared_ptr<PortraitSetController> portraits,
                     Size screenSize);

    /**
     * TODO: Implement Me (Not for Gameplay Prototype)
     */
    Vec2 closeDoors();

    /**
     * TODO: Implement Me (Not for Gameplay Prototype)
     */
    Vec2 placeTraps();

    /**
     * Gets the viewpoint for current portrait
     *
     * @return rectangle that should be drawn
     */
    const Rect getCameraView() { return _portraits->getViewPortrait(); };

    /**
     * TODO: Implement Me
     * This function should
     * (1) get the view from portraitsetcontroller
     * (2) attach the viewport to the scene
     */
    void render(std::shared_ptr<cugl::SpriteBatch>& batch, Size size);

    /**
     * TODO: Implement Me
     * This function should (for gameplay prototype)
     * (1) detect camera change
     * (2) modify portraitsetcontroller to reflect the change
     */
    void update(const std::shared_ptr<TilemapController> _tilemap,
                bool canPlaceTrap);

#pragma mark Setters
  public:
    /**
     * Updates the number of clams available.
     *
     * @param clams the number of clams
     */
    void updateClams(float clams) { _model->setClams(clams); }

    /**
     * Updates the number of close doors available.
     *
     * @param doors the number of close doors available
     */
    void updateDoors(float doors) { _model->setDoors(doors); }

    /**
     * Updates available energy
     *
     * @param energy the energy level available
     */
    void updateEnergy(float energy) { _model->setEnergy(energy); }

    /**
     * Updates remaining camera cooldown time for this spirit
     *
     * @param cameraCool the new remaining cooltime
     */
    void updateCameraCoolDown(float cameraCool) {
        _model->setCameraCooldown(cameraCool);
    }

    /**
     * Updates remaining place clam cooldown time for this spirit
     *
     * @param clamCool the new remaining cooltime
     */
    void updateClamCooldown(float clamCool) {
        _model->setClamCooldown(clamCool);
    }

    /**
     * Updates remaining close door cooldown time for this spirit
     *
     * @param doorCool the new remaining cooltime
     */
    void updateDoorCooldown(float doorCool) {
        _model->setDoorCooldown(doorCool);
    }

    void decreaseCameraCool() {
        _cameraCool--;
        _model->setCameraCooldown(_cameraCool);
    }

    void resetCameraCool() {
        _cameraCool = CAMERA_COOL;
        _model->setCameraCooldown(_cameraCool);
    }

    bool isSwitchable() {
        return _cameraCool <= 0;
        
    }
    
    bool getTrapAdded() { return _trapAdded; }
    
    void setTrapAdded(bool trapAdded) { _trapAdded = trapAdded; }
    
    Vec2 getLastTrapPos() { return _lastTrapPos; }
    
    void setLastTrapPos(Vec2 pos) { _lastTrapPos = pos; }

#pragma mark Helpers
  public:
    Rect screenToWorld(Rect rect);
    
    void addHunter(Vec2 pos) {
        _model->addHunter(pos);
    }
    
    void moveHunter(Vec2 pos) {
        _model->moveHunter(pos);
    }
};

#endif /* _SPIRIT_CONTROLLER_H */
