//
//  SpiritModel.h
//  Sunk Cost
//
//  This module provides the SpiritModel class.
//
//  Author: Soon Jae Park
//  Version: 2/22/23.
//

#ifndef _SPIRITMODEL_H
#define _SPIRITMODEL_H

#include "HunterModel.h"
#include "HunterView.h"
#include "TrapModel.hpp"
#include "TrapView.h"
#include <cugl/cugl.h>

class SpiritModel {
#pragma mark State
  private:
    /** Energy that the spirit has left over */
    float _energy;
    /** Number of traps that the spirit has left */
    int _traps;
    /** Number of doors that the spirit has left */
    int _doors;
    /** Cooldown time for close door */
    float _doorCool;
    /** Cooldown time for place clam */
    float _clamCool;
    /** Cooldown time for switching camera */
    float _cameraCool;

    std::vector<std::shared_ptr<TrapModel>> _trapModels;
    std::vector<std::shared_ptr<TrapView>> _trapViews;

    std::shared_ptr<cugl::Scene2> _scene;
    std::shared_ptr<cugl::AssetManager> _assets;

    std::shared_ptr<HunterModel> _hunterModel;
    std::shared_ptr<HunterView> _hunterView;
    int forward = 0, right = 0;
    int _ticks = 0;

    bool _isOnLock;

    bool _isOnTrap;

    Vec2 _lastTrapPos;

  public:
    /** A public accessible, read-only version of the energy level */
    float& energy;
    /** A public accessible, read-only version of the energy level */
    int& traps;
    /** A public accessible, read-only version of the energy level */
    int& doors;
    /** A public accessible, read-only version of the door cool down */
    float& doorCool;
    /** A public accessible, read-only version of the clam cool down */
    float& clamCool;
    /** A public accessible, read-only version of the camera cool down */
    float& cameraCool;
    /** A public accessible, read-only version of the isOnLock */
    bool& isOnLock;
    /** A public accessible, read-only version of the isOnTrap */
    bool& isOnTrap;
    /** A public accessible, read-only version of the lastTrapPos */
    Vec2& lastTrapPos;

#pragma mark Main Functions
  public:
    /**
     * Constructor for spirit model
     *
     * @param clams the number of clams left
     * @param doors the number of close doors left
     * @param energy the energy left for this player
     */
    SpiritModel(const std::shared_ptr<cugl::AssetManager>& assets,
                std::shared_ptr<cugl::Scene2> scene, int clams, int doors,
                float energy)
        : traps(_traps), doors(_doors), energy(_energy),
          cameraCool(_cameraCool), clamCool(_clamCool), doorCool(_doorCool),
          isOnLock(_isOnLock), isOnTrap(_isOnTrap), lastTrapPos(_lastTrapPos) {
        setTraps(clams);
        setDoors(doors);
        setEnergy(energy);
        setCameraCooldown(0);
        setClamCooldown(0);
        setDoorCooldown(0);
        _scene = scene;
        _assets = assets;
        setLockState(false);
        setTrapState(false);
        setLastTrapPos(Vec2::ZERO);
    }

#pragma mark Setters
  public:
    /**
     * Sets cooldown time for camera
     *
     * @param cameraCool cooldown for camera
     */
    void setCameraCooldown(float cameraCool) { _cameraCool = cameraCool; };

    /**
     * Sets cooldown time for clams
     *
     * @param clamCool cooldown for clams
     */
    void setClamCooldown(float clamCool) { _clamCool = clamCool; };

    /**
     * Sets cooldown time for close door
     *
     * @param doorCool cooldown for close door
     */
    void setDoorCooldown(float doorCool) { _doorCool = doorCool; };

    /**
     * Sets the number of available clams for the spirit
     *
     * @param clams the number of clams
     */
    void setTraps(int traps) { _traps = traps; };

    /**
     * Sets the number of available doors for the spirit
     *
     * @param doors the number of doors
     */
    void setDoors(int doors) { _doors = doors; };

    void setLockState(bool lock) { _isOnLock = lock; }

    void setTrapState(bool trap) { _isOnTrap = trap; }

    void setLastTrapPos(Vec2 pos) { _lastTrapPos = pos; }

    /**
     * Sets the available energy for the spirit
     *
     * @param energy the energy level
     */
    void setEnergy(float energy) { _energy = energy; };

    void addTrap(Vec2 position,
                 std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        if (_trapModels.size() >= 3)
            return;
        // a trap will last for 15s for now
        _trapModels.emplace_back(std::make_shared<TrapModel>(position, 900));
        auto trap = std::make_shared<TrapView>(_assets, position, 20);
        _trapViews.emplace_back(trap);
        trap->addChildToNode(node);
        //        trap->addChildTo(_scene); //TODO: add trap
    }

    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }

    // 0: nothing; 1: remove 1 trap; 2: remove 2 traps
    int update(bool trapTriggered, Vec2 pos,
               std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        bool result = 0;
        std::vector<std::shared_ptr<TrapModel>> pendingTrapModels;
        std::vector<std::shared_ptr<TrapView>> pendingTrapViews;

        int target = cloestTrapToHunter(pos);
        if (trapTriggered && target != -1) {
            result = 1;
            // remove the trap closest to the hunter position
            //            _trapViews[target]->removeChildFrom(_scene);
            _trapViews[target]->removeChildFromNode(node);
        }

        for (int i = 0; i < _trapModels.size(); i++) {
            if (!(trapTriggered && target == i)) {
                pendingTrapModels.emplace_back(_trapModels[i]);
                _trapViews[i]->update();
                pendingTrapViews.emplace_back(_trapViews[i]);
            }
        }

        _trapModels = pendingTrapModels;
        _trapViews = pendingTrapViews;
        if (_hunterView && _ticks == 0) {
            //            CULog("%d, %d", right, forward);
            _hunterView->advanceFrame(forward, right,false);
            //to be edited for hurting animation
            forward = 0;
            right = 0;
        }
        _ticks = (_ticks + 1) % 6;

        return result;
    }

    void
    addHunter(Vec2 position,
              std::vector<std::shared_ptr<scene2::PolygonNode>>& hunterNodes) {
        _hunterModel = std::make_shared<HunterModel>(_assets, _scene, 1024);
        // A place holder for hunterID, 0 is blue, 1 is orange, 2 is green
        int hunterID = 0;
        _hunterView = std::make_shared<HunterView>(_assets, position,
                                                   Vec2(40, 40), hunterID);

        //        _hunterView->addChildTo(_scene);
        _hunterView->addChildToNode(hunterNodes);
    }

    void moveHunter(Vec2 position) {
        Vec2 diff = position - _hunterModel->position;
        forward = diff.y;
        right = diff.x;
        _hunterModel->setPosition(position);
        _hunterView->setPosition(position);
    }

    void alertTreasure(Vec2 position) {}

    int cloestTrapToHunter(Vec2 pos) {
        int result = -1;
        float minDis = 100000;
        if (!_hunterModel) {
            return -1;
        }
        Vec2 hunterPos = _hunterModel->getPosition();
        for (int i = 0; i < _trapModels.size(); i++) {
            float dis = hunterPos.distance(_trapModels.at(i)->getPosition());
            if (dis < minDis) {
                minDis = dis;
                result = i;
            }
        }
        return result;
    }
};

#endif /* _SPIRITMODEL_H */
