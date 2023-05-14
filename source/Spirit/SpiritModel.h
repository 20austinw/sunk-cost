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

    int _killTicks;

    bool _killing;

    std::shared_ptr<scene2::SpriteNode> _killAnimation;

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

    bool _isOnKill;

    float _killCool;

    Vec2 _lastTrapPos;

    int _health;

    std::vector<std::shared_ptr<scene2::PolygonNode>> _deadHearts;
    std::vector<std::shared_ptr<scene2::SpriteNode>> _liveHearts;

    bool _hunterAdded;

    float _offset;

    float KILL_COOL = 600;

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
    /** A public accessible, read-only version of the isOnKill */
    bool& isOnKill;
    /** A public accessible, read-only version of the lastTrapPos */
    Vec2& lastTrapPos;
    float& killCool;
    int& health;
    bool& killing;
    bool& hunterAdded;

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
          isOnLock(_isOnLock), isOnTrap(_isOnTrap), lastTrapPos(_lastTrapPos),
          isOnKill(_isOnKill), killCool(_killCool), health(_health),
          killing(_killing), hunterAdded(_hunterAdded) {
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
        setKillState(false);
        setLastTrapPos(Vec2::ZERO);
        setKillCooldown(0);
        setHealth(3);
        initHearts();
        _hunterAdded = false;
        setKilling(false);
        _killAnimation = scene2::SpriteNode::allocWithSheet(
            assets->get<Texture>("hunterhurt"), 5, 3, 14);
        _killAnimation->setFrame(0);
        _killAnimation->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _killAnimation->setScale(0.5);
        _killTicks = 0;
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

    void setHealth(int health) { _health = health; }

    bool isKilling() { return _killing; }

    void setKilling(bool b) { _killing = b; }

    Vec2 getHunterPos() { return _hunterModel->getPosition(); }

    float getMaxKillCool() { return KILL_COOL; }

    /**
     * Sets the number of available doors for the spirit
     *
     * @param doors the number of doors
     */
    void setDoors(int doors) { _doors = doors; };

    void setLockState(bool lock) { _isOnLock = lock; }

    void setTrapState(bool trap) { _isOnTrap = trap; }

    void setLastTrapPos(Vec2 pos) { _lastTrapPos = pos; }

    void setKillCooldown(float cool) { _killCool = cool; }

    bool isKillable() { return _killCool <= 0; }

    void setKillState(bool kill) { _isOnKill = kill; }

    /**
     * Sets the available energy for the spirit
     *
     * @param energy the energy level
     */
    void setEnergy(float energy) { _energy = energy; };

    bool addTrap(Vec2 position,
                 std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        if (_trapModels.size() >= 3)
            return false;
        // Position is too close to either of the other traps
        for (auto& trap : _trapModels) {
            if (position.distance(trap->getPosition()) < 500) {
                return false;
            }
        }
        // a trap will last for 15s for now
        _trapModels.emplace_back(std::make_shared<TrapModel>(position, 900));
        auto trap = std::make_shared<TrapView>(_assets, position, 20);
        _trapViews.emplace_back(trap);
        trap->addChildToNode(node);
        //        trap->addChildTo(_scene); //TODO: add trap
        return true;
    }

    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }

    // 0: nothing; 1: remove 1 trap; 2: remove 2 traps
    int update(bool trapTriggered, Vec2 pos,
               std::shared_ptr<cugl::scene2::PolygonNode>& node) {

        if (_hunterAdded) {
            updateHeartsFrame();
        }

        bool result = 0;
        std::vector<std::shared_ptr<TrapModel>> pendingTrapModels;
        std::vector<std::shared_ptr<TrapView>> pendingTrapViews;

        int target = -1;
        if (trapTriggered) {
            target = cloestTrapToHunter(pos);
        }

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
            _hunterView->advanceFrame(forward, right, false);
            // to be edited for hurting animation
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
        updateHeartsPos(position, _hunterView->getSize());
        for (int i = 0; i < _liveHearts.size(); i++) {
            hunterNodes.emplace_back(_liveHearts.at(i));
        }
        _hunterAdded = true;
    }

    void moveHunter(Vec2 position) {
        if (!_killing) {
            Vec2 diff = position - _hunterModel->position;
            forward = diff.y;
            right = diff.x;
            _hunterModel->setPosition(position);
            _hunterView->setPosition(position);
            updateHeartsPos(position, _hunterView->getSize());
        }
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

    void initHearts() {
        for (int i = 0; i < _health; i++) {
            _liveHearts.emplace_back(scene2::SpriteNode::allocWithSheet(
                _assets->get<Texture>("heart_live"), 2, 8, 11));
            _liveHearts.at(_liveHearts.size() - 1)->setFrame(0);
            _liveHearts.at(_liveHearts.size() - 1)->setScale(0.2);
        }
        _offset = _liveHearts.at(0)->getWidth() / 2;
    }

    void updateHeartsPos(Vec2 pos, Size size) {
        Vec2 curPos = pos + Vec2(-_offset + size.width / 2, size.height * 1.2);
        for (int i = 0; i < _liveHearts.size(); i++) {
            _liveHearts.at(i)->setPosition(curPos);
            curPos.x += _offset;
        }
        for (int i = 0; i < _deadHearts.size(); i++) {
            _deadHearts.at(i)->setPosition(curPos);
            curPos.x += _offset;
        }
    }

    void updateHeartsFrame() {
        for (int i = 0; i < _liveHearts.size(); i++) {
            int frame = _liveHearts.at(i)->getFrame();
            if (frame < 10) {
                _liveHearts.at(i)->setFrame(frame + 1);
            } else {
                _liveHearts.at(i)->setFrame(0);
            }
        }
    }

    void updateHeart(
        std::vector<std::shared_ptr<scene2::PolygonNode>>& hunterNodes) {
        _killing = true;
        hunterNodes.clear();
        _liveHearts.pop_back();
        _killAnimation->setPosition(getHunterPos());

        hunterNodes.emplace_back(_killAnimation);
        for (int i = 0; i < _liveHearts.size(); i++) {
            hunterNodes.emplace_back(_liveHearts.at(i));
        }

        _deadHearts.emplace_back(scene2::PolygonNode::allocWithTexture(
            _assets->get<Texture>("heart_dead")));
        _deadHearts.at(_deadHearts.size() - 1)->setScale(0.2);

        for (int i = 0; i < _deadHearts.size(); i++) {
            hunterNodes.emplace_back(_deadHearts.at(i));
        }

        updateHeartsPos(getHunterPos(), _hunterView->getSize());
    }

    void updateOnKill(
        std::vector<std::shared_ptr<scene2::PolygonNode>>& hunterNodes) {
        int frame = _killAnimation->getFrame();
        if (frame != 13) {
            if (_killTicks % 4 == 0) {
                _killAnimation->setFrame(frame + 1);
            }
            _killTicks++;
        } else {
            _killTicks = 0;
            _killAnimation->setFrame(0);
            _killing = false;
            hunterNodes.clear();
            _hunterView->addChildToNode(hunterNodes);
            for (int i = 0; i < _liveHearts.size(); i++) {
                hunterNodes.emplace_back(_liveHearts.at(i));
            }
            for (int i = 0; i < _deadHearts.size(); i++) {
                hunterNodes.emplace_back(_deadHearts.at(i));
            }
            updateHeartsPos(getHunterPos(), _hunterView->getSize());
        }
    }
};

#endif /* _SPIRITMODEL_H */
