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

#include <cugl/cugl.h>
#include "TrapView.h"
#include "TrapModel.hpp"

class SpiritModel {
  #pragma mark State
  private:
    /** Energy that the spirit has left over */
    float _energy;
    /** Number of traps that the spirit has left */
    int _clams;
    /** Number of doors that the spirit has left */
    int _doors;
    /** Cooldown time for close door */
    float _doorCool;
    /** Cooldown time for place clam */
    float _clamCool;
    /** Cooldown time for switching camera */
    float _cameraCool;
    
    std::unordered_set<std::shared_ptr<TrapModel>> trapModels;
    std::unordered_set<std::shared_ptr<TrapView>> trapViews;

  public:
    /** A public accessible, read-only version of the energy level */
    float& energy;
    /** A public accessible, read-only version of the energy level */
    int& clams;
    /** A public accessible, read-only version of the energy level */
    int& doors;
    /** A public accessible, read-only version of the door cool down */
    float& doorCool;
    /** A public accessible, read-only version of the clam cool down */
    float& clamCool;
    /** A public accessible, read-only version of the camera cool down */
    float& cameraCool;

  #pragma mark Main Functions
  public:
    /**
     * Constructor for spirit model
     *
     * @param clams the number of clams left
     * @param doors the number of close doors left
     * @param energy the energy left for this player
     */
    SpiritModel(int clams, int doors, float energy) :
    clams(_clams),
    doors(_doors),
    energy(_energy),
    cameraCool(_cameraCool),
    clamCool(_clamCool),
    doorCool(_doorCool)
    {
      setClams(clams);
      setDoors(doors);
      setEnergy(energy);
      setCameraCooldown(0);
      setClamCooldown(0);
      setDoorCooldown(0);
    }

  #pragma mark Setters
  public:

    /**
     * Sets cooldown time for camera
     *
     * @param cameraCool cooldown for camera
     */
    void setCameraCooldown(float cameraCool) {
      _cameraCool = cameraCool;
    };

    /**
     * Sets cooldown time for clams
     *
     * @param clamCool cooldown for clams
     */
    void setClamCooldown(float clamCool) {
      _clamCool = clamCool;
    };

    /**
     * Sets cooldown time for close door
     *
     * @param doorCool cooldown for close door
     */
    void setDoorCooldown(float doorCool) {
      _doorCool = doorCool;
    };

    /**
     * Sets the number of available clams for the spirit
     *
     * @param clams the number of clams
     */
    void setClams(int clams) {
      _clams = clams;
    };

    /**
     * Sets the number of available doors for the spirit
     *
     * @param doors the number of doors
     */
    void setDoors(int doors) {
      _doors = doors;
    };

    /**
     * Sets the available energy for the spirit
     *
     * @param energy the energy level
     */
    void setEnergy(float energy) {
      _energy = energy;
    };

    void addTrap(Vec2 position) {
        trapModels.insert(std::make_shared<TrapModel>(position));
        trapViews.insert(std::make_shared<TrapView>(position));
    }
    
    void removeTrap() {
        
    }
};

#endif /* _SPIRITMODEL_H */
