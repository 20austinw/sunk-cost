//
//  PortraitSetController.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __PORTRAIT_SET_CONTROLLER_H__
#define __PORTRAIT_SET_CONTROLLER_H__
#include "CameraController.h"
#include "PortraitModel.h"
#include "PortraitView.h"
#include <cugl/cugl.h>
#include <memory>

using namespace cugl;

class PortraitSetController {
#pragma mark Internal References
  private:
    /** The set of portraits */
    std::vector<std::unique_ptr<CameraController>> _portraits;
    /** The Camera object */
    std::vector<std::unique_ptr<PortraitModel>> _portraitModels;
    std::vector<std::unique_ptr<PortraitView>> _portraitViews;
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<cugl::Scene2> _scene;
    Size _screenSize;
    /** The max battery */
    float _maxBattery;

    bool _prevState;

    int _index;

    /** The scale for noBattery*/
    float _scale;

    std::shared_ptr<scene2::PolygonNode> _block;

    std::shared_ptr<scene2::SpriteNode> _redBattery;
    std::shared_ptr<scene2::SpriteNode> _greenBattery;
    std::shared_ptr<scene2::PolygonNode> _noBattery;

#pragma mark Main Functions
  public:
    /**
     * Constructor for Camera Controller
     */
    PortraitSetController(const std::shared_ptr<cugl::AssetManager>& assets,
                          std::shared_ptr<cugl::Scene2>& scene, int id = 0,
                          Size screenSize = Size::ZERO,
                          float maxBattery = 600) {
        _scene = scene;
        _assets = assets;
        _screenSize = screenSize;
        _maxBattery = maxBattery;
        initializePortraitSet();
    }

    void update() {
        for (int i = 0; i < _portraitViews.size(); i++) {
            if (i == _index) {
                _portraitViews[i]->update();
            } else {
                _portraitViews[i]->reset();
            }
        }
    }
    /**
     * Adds a new portrait to the portraitset
     *
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */
    void addPortrait(int id, Vec3 cameraPosition, Vec3 portraitPosition,
                     Vec3 direction, Vec2 directionLimits, float battery = 600,
                     int type = 2) {
        _portraits.push_back(makePortrait(id, cameraPosition, portraitPosition,
                                          direction, directionLimits, battery,
                                          type));
    }

    /**
     * Adds a new portrait to the portraitset
     *
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */
    void insertPortraitTo(int index, int id, Vec3 cameraPosition,
                          Vec3 portraitPosition, Vec3 direction,
                          Vec2 directionLimits, float battery = 600,
                          int type = 2) {
        _portraits.insert(getIteratorForIndex(index),
                          makePortrait(id, cameraPosition, portraitPosition,
                                       direction, directionLimits, battery,
                                       type));
    }

    /**
     * Initializes the portrait set
     *
     * @param id
     * @param position
     * @param direction
     * @param directionLimits
     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
     * default camera is camera that is viewing a black screen
     */
    void initializePortraitSet(int id = 0, Vec3 cameraPosition = Vec3::ZERO,
                               Vec3 portraitPosition = Vec3::ZERO,
                               Vec3 direction = Vec3::ZERO,
                               Vec2 directionLimits = Vec2::ZERO,
                               float battery = 600, int type = 2) {
        _portraits.push_back(makePortrait(id, cameraPosition, portraitPosition,
                                          direction, directionLimits, battery,
                                          type));
        _index = 0;
        _prevState = true;
        _scale = 0;
    }

    void initializeSheets(std::shared_ptr<cugl::Texture> green,
                          std::shared_ptr<cugl::Texture> red,
                          std::shared_ptr<cugl::Texture> no) {
        _greenBattery = scene2::SpriteNode::allocWithSheet(green, 2, 8, 16);
        _redBattery = scene2::SpriteNode::allocWithSheet(red, 2, 8, 16);
        _noBattery = scene2::PolygonNode::allocWithTexture(no);
        _noBattery->setScale(_scale);
    }

    /**
     * Adds a new portrait to the portraitset
     */
    void clearPortraitSet() {
        _portraits.clear();
        _index = -1;
    }

#pragma mark Main Functions
    void setIndex(int index) { _index = index; }

#pragma mark Getters
  public:
    /**
     * Getter for viewport of this camera
     *
     * @return Rectangle that contains the camera viewpoint
     */
    const Rect getViewPortrait() { return _portraits[_index]->getViewPort(); }
    const int getNearest(Vec2 position) {
        int idx = 0;
        int minDist = INT_MAX;
        for (int i = 0; i < _portraits.size(); i++) {
            float dist = _portraits[i]->getPosition().distance(position);
            if (minDist > dist) {
                idx = i;
                minDist = dist;
            }
        }
        return idx;
    }
    /**
     * Getter for current camera index
     *
     * @return Rectangle that contains the camera viewpoint
     */
    int getIndex() { return _index; }

    Vec3 getPosition(int index) { return _portraits[index]->getPosition(); }

    bool getPrevState() { return _prevState; }

#pragma mark Main Functions
  public:
    void setPrevState(bool state) { _prevState = state; }

    /**
     * Updates camera position
     *
     * @return viewport
     */
    void updatePositionIndex(Vec3 position, int index) {
        _portraits[index]->updatePosition(position);
    }

    /**
     * Updates direction of the camera
     */
    void updateDirectionIndex(Vec3 direction, int index) {
        _portraits[index]->updateDirection(direction);
    }

    /**
     * Updates camera direction limits. [d.x,d.y] should be the range of
     * possible values for angle.
     */
    void updateDirectionLimitsIndex(Vec2 directionLimits, int index) {
        _portraits[index]->updateDirectionLimits(directionLimits);
    }

    /**
     * Updates camera type for the camera at the given index
     */
    void updateTypeIndex(CameraType type, int index) {
        _portraits[index]->updateType(type);
    }

    /**
     * Updates camera type for the current camera
     */
    void updateType(CameraType type) { _portraits[_index]->updateType(type); }

    /**
     * Rotates the camera by angle
     *
     * @param angle how much the camera will be rotated by
     */
    void rotate(float angle) { _portraits[_index]->rotate(angle); }

    /**
     * Modifies the camera to look at the given target
     *
     * @param target the point to look at
     */
    void lookAt(const Vec3 target) { _portraits[_index]->lookAt(target); }

    void updateBattery() {
        for (int i = 0; i < _portraits.size(); i++) {
            if (((_index != _portraits[i]->getID()) ||
                 (!_portraits[i]->getState())) &&
                (_portraits[i]->getBattery() < _maxBattery)) {
                float newBattery = _portraits[i]->getBattery() + 0.5;
                if (newBattery < _maxBattery) {
                    _portraits[i]->updateBattery(newBattery);
                } else {
                    _portraits[i]->updateBattery(_maxBattery);
                }
            } else if (_index == _portraits[i]->getID() &&
                       _portraits[i]->getBattery() > 0) {
                float newBattery = _portraits[i]->getBattery() - 1;
                if (newBattery > 0) {
                    _portraits[i]->updateBattery(newBattery);
                } else {
                    _portraits[i]->updateBattery(0);
                }
            }
            if (_portraits[i]->getBattery() <= 0) {
                _portraits[i]->updateState(false);
            } else if (!_portraits[i]->getState() &&
                       _portraits[i]->getBattery() >= _maxBattery) {
                _portraits[i]->updateState(true);
            }
        }
    }

    void updateBatteryNode(Vec2 offset,
                           const std::shared_ptr<cugl::Scene2>& scene) {
        float zoom =
            std::dynamic_pointer_cast<OrthographicCamera>(scene->getCamera())
                ->getZoom();
        Vec2 pos = scene->getCamera()->screenToWorldCoords(
            _scene->getSize() - _greenBattery->getSize() / 2 * zoom);
        bool curState = getCurState();
        int frame = getCurFrame();
        _greenBattery->setFrame(frame);
        _redBattery->setFrame(frame);
        _greenBattery->setPosition(pos);
        _redBattery->setPosition(pos);
        _noBattery->setPosition(pos);
        _noBattery->setScale(_scale);
        if (_scale > 0) {
            _scale -= 0.01;
        }
        if (curState != _prevState) {
            if (curState) {
                scene->removeChild(_redBattery);
                scene->addChild(_greenBattery);
            } else {
                scene->removeChild(_greenBattery);
                scene->addChild(_redBattery);
            }
            scene->removeChild(_noBattery);
            scene->addChild(_noBattery);
        }
    }

    float getCurBattery() {
        return (float)_portraits[_index]->getBattery() / _maxBattery;
    }

    bool getCurState() { return _portraits[_index]->getState(); }

    void setMaxbattery(float maxBattery) { _maxBattery = maxBattery; }

    void addBlock(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_block);
    }

    void removeBlock(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_block);
    }

    void refreshBatteryNodes(const std::shared_ptr<cugl::Scene2>& scene) {
        if (getCurState()) {
            scene->removeChild(_greenBattery);
            scene->addChild(_greenBattery);
        } else {
            scene->removeChild(_redBattery);
            scene->addChild(_redBattery);
        }
        scene->removeChild(_noBattery);
        scene->addChild(_noBattery);
    }

    void initializeBatteryNodes(const std::shared_ptr<cugl::Scene2>& scene) {
        _block = scene2::PolygonNode::allocWithPoly(Rect(0, 0, 4608, 4608));
        _block->setColor(Color4::BLACK);
        _greenBattery->setFrame(0);
        scene->addChild(_greenBattery);
        scene->addChild(_noBattery);
    }

    void resetScale() { _scale = 0.5; }

#pragma mark Helpers
  private:
    std::unique_ptr<CameraController> makePortrait(int id, Vec3 cameraPosition,
                                                   Vec3 portraitPosition,
                                                   Vec3 direction,
                                                   Vec2 directionLimits,
                                                   float battery, int type) {
        std::unique_ptr<CameraController> camera =
            std::make_unique<CameraController>(id, _screenSize);
        camera->updatePosition(cameraPosition);
        camera->lookAt(direction);
        camera->updateDirectionLimits(directionLimits);
        camera->updateType(CameraType(type));
        camera->updateBattery(battery);
        _portraitModels.push_back(
            std::make_unique<PortraitModel>(cameraPosition));
        _portraitViews.push_back(std::make_unique<PortraitView>(
            _assets, portraitPosition +
                         Vec2(_assets->get<Texture>("map")->getSize() / 2)));
        _portraitViews[id]->addChildTo(_scene);
        return camera;
    }

    std::vector<std::unique_ptr<CameraController>>::iterator
    getIteratorForIndex(int idx) {
        return _portraits.begin() + idx;
    }

    int getCurFrame() {
        int i = getCurBattery() * 16;
        if (getCurState()) {
            if (i > 0) {
                return 16 - i;
            }
            return 15;
        }
        if (i < 16) {
            return i;
        }
        return 15;
    }
};

#endif /* __PORTRAIT_SET_CONTROLLER_H__ */
