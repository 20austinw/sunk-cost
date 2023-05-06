//
//  DoorModel.h
//  Sunk
//
//  Created by Xiaoyue Jin on 3/24/23.
//

#ifndef DoorModel_h
#define DoorModel_h

#include <cugl/cugl.h>

using namespace cugl;

class DoorModel {
#define FRAME_NUM_FRONT 21
#define FRAME_NUM_SIDE 18
#define FRAME_NUM_Front_Hunter 13
#pragma mark States
  private:
    /** door type (0 for front, 1 for side) */
    int _type;
    /** door type (0 for unlocked; 1 for locked;  2 for animation in progress)
     */
    int _state;

    Vec2 _position;

    float _step;

    int _frame;

    Poly2 _poly;

#pragma mark Main Functions
  public:
    DoorModel(Vec2 pos, int type) {
        setType(type);
        setPosition(pos);
        setStep(0);
        setState(0);
        setFrame(0);
        setPoly();
    }

#pragma mark Getters
    Vec2 getPosition() { return _position; }

    int getState() { return _state; }

    int getFrame() {
        if (_type == 0) {
            return FRAME_NUM_Front_Hunter - 1;
        } else {
            return FRAME_NUM_SIDE - 1;
        }
    }

    int getType() { return _type; }

    float getStep() { return _step; }

    Poly2 getPoly() { return _poly; }

#pragma mark Setters
    void setStep(float distance) {
        if (_type == 0) {
            _step = distance / (FRAME_NUM_FRONT - 1);
        } else {
            _step = distance / (FRAME_NUM_SIDE - 1);
        }
    }

    void setPosition(Vec2 position) { _position = position; }

    void setType(int type) { _type = type; }

    void setState(int state) { _state = state; }

    void setFrame(int frame) { _frame = frame; }

  private:
    void setPoly() {
        std::vector<Vec2> lineData;
        if (_type == 0) {
            lineData.emplace_back(Vec2(_position.x - 128, _position.y - 128));
            lineData.emplace_back(Vec2(_position.x + 128, _position.y - 128));
        } else {
            lineData.emplace_back(Vec2(_position.x, _position.y + 128));
            lineData.emplace_back(Vec2(_position.x, _position.y - 128));
        }
        SimpleExtruder extruder = SimpleExtruder();
        extruder.set(lineData, false);
        extruder.calculate(500, 100);
        _poly = extruder.getPolygon();
    }
};

#endif /* DoorModel_h */
