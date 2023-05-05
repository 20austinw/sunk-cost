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
    std::vector<std::shared_ptr<PortraitView>> _portraitViews;
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<cugl::Scene2> _scene;
    Size _screenSize;

    /** The max battery */
    float _maxBattery;

    bool _prevState;

    int _index;

    /** The scale for noBattery*/
//    float _noBatteryScale;
    float _greenBatteryScale;
    float _redBatteryScale;
//    float _resetScale;

    float _buttonSize = 400;

    std::shared_ptr<scene2::PolygonNode> _block;
    std::shared_ptr<scene2::SpriteNode> _redBattery;
    std::shared_ptr<scene2::SpriteNode> _greenBattery;
    
    std::shared_ptr<scene2::SpriteNode> _curBattery;
//    std::shared_ptr<scene2::PolygonNode> _noBattery;

  private:
    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }

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
        _index = 0;
        _prevState = true;
        //        initializePortraitSet();
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

    void updatespecific(int id) {
        for (int i = 0; i < _portraitViews.size(); i++) {
            if (i == id) {
                _portraitViews[i]->update();
            } else {
                _portraitViews[i]->reset();
            }
        }
    }

    int getTickSpec(int id) { return _portraitViews[id]->getTick(); }

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
    void addPortrait(std::vector<std::shared_ptr<scene2::PolygonNode>>& vector,
                     int id, Vec3 cameraPosition, Vec3 portraitPosition,
                     Vec3 direction, Vec2 directionLimits, float battery = 600,
                     int type = 2) {
        _portraits.push_back(makePortrait(id, cameraPosition, portraitPosition,
                                          direction, directionLimits, battery,
                                          type, vector));
    }

    //    /**
    //     * Adds a new portrait to the portraitset
    //     *
    //     * @param id
    //     * @param position
    //     * @param direction
    //     * @param directionLimits
    //     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
    //     * default camera is camera that is viewing a black screen
    //     */
    //    void insertPortraitTo(std::shared_ptr<cugl::scene2::PolygonNode>&
    //    node, int index, int id, Vec3 cameraPosition,
    //                          Vec3 portraitPosition, Vec3 direction,
    //                          Vec2 directionLimits, float battery = 600,
    //                          int type = 2) {
    //        _portraits.insert(getIteratorForIndex(index),
    //                          makePortrait(id, cameraPosition,
    //                          portraitPosition,
    //                                       direction, directionLimits,
    //                                       battery, type, node));
    //    }

    //    /**
    //     * Initializes the portrait set
    //     *
    //     * @param id
    //     * @param position
    //     * @param direction
    //     * @param directionLimits
    //     * @param type camera type - (0 = default) (1 = player) (2 = portrait)
    //     * default camera is camera that is viewing a black screen
    //     */
    //    void initializePortraitSet(int id = 0, Vec3 cameraPosition =
    //    Vec3::ZERO,
    //                               Vec3 portraitPosition = Vec3::ZERO,
    //                               Vec3 direction = Vec3::ZERO,
    //                               Vec2 directionLimits = Vec2::ZERO,
    //                               float battery = 600, int type = 2) {
    //        _portraits.push_back(makePortrait(id, cameraPosition,
    //        portraitPosition,
    //                                          direction, directionLimits,
    //                                          battery, type));
    //        _index = 0;
    //        _prevState = true;
    //    }

    void initializeSheets(std::shared_ptr<cugl::Texture> green,
                          std::shared_ptr<cugl::Texture> red,
                          std::shared_ptr<cugl::Texture> no) {
        _greenBattery = scene2::SpriteNode::allocWithSheet(green, 5, 8, 40);
        _redBattery = scene2::SpriteNode::allocWithSheet(red, 5, 8, 40);
//        _noBattery = scene2::PolygonNode::allocWithTexture(no);
        _greenBatteryScale = _buttonSize / _greenBattery->getSize().width;
        _redBatteryScale = _buttonSize / _redBattery->getSize().width;
//        _noBatteryScale = 0;
//        _resetScale =
//            _greenBattery->getSize().width / _noBattery->getSize().width;
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
        for (int i = 1; i < _portraits.size(); i++) {
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

    void updateBattery(bool selection) {
        if(!selection){
            for (int i = 1; i < _portraits.size(); i++) {
                if (((_index != _portraits[i]->getID()) ||
                     (!_portraits[i]->getState())) &&
                    (_portraits[i]->getBattery() < _maxBattery)) {
                    float newBattery = _portraits[i]->getBattery() + 1;
                    if (newBattery < _maxBattery) {
                        _portraits[i]->updateBattery(newBattery);
                    } else {
                        _portraits[i]->updateBattery(_maxBattery);
                    }
                } else if (_index == _portraits[i]->getID() && _portraits[i]->getState() &&
                           _portraits[i]->getBattery() > 0) {
                    float newBattery = _portraits[i]->getBattery() - 0.5;
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
    }

    void updateBatteryNode(std::shared_ptr<cugl::scene2::PolygonNode>& node,
                           float offset) {
        _greenBattery->setScale(_greenBatteryScale / getZoom());
        _redBattery->setScale(_redBatteryScale / getZoom());
//        _noBattery->setScale(_noBatteryScale / getZoom());
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            Vec2(_greenBattery->getSize().width * getZoom() / 2,
                 _scene->getSize().height -
                     _greenBattery->getSize().height * getZoom() / 2));
        int frame = getCurFrame();
        _greenBattery->setFrame(frame);
        _redBattery->setFrame(frame);
        _greenBattery->setPosition(pos);
        _redBattery->setPosition(pos);
//        _noBattery->setPosition(pos);
//        if (_noBatteryScale > 0) {
//            _noBatteryScale -= 0.01;
//        }
        node->removeChild(_curBattery);
        if (getCurState()){
            _curBattery = _greenBattery;
        } else {
            _curBattery = _redBattery;
        }
        node->addChild(_curBattery);
//        if (curState != _prevState) {
//            if (curState) {
//                node->removeChild(_redBattery); // TODO: add to node instead
//                node->addChild(_greenBattery);
//            } else {
//                node->removeChild(_greenBattery);
//                node->addChild(_redBattery);
//            }
////            node->removeChild(_noBattery);
////            node->addChild(_noBattery);
//        }
    }

    float getCurBattery() {
        return (float)_portraits[_index]->getBattery() / _maxBattery;
    }

    bool getCurState() { return _portraits[_index]->getState(); }

    void setMaxbattery(float maxBattery) { _maxBattery = maxBattery; }

    void addBlock(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        _block->setScale(1 / getZoom());
        _block->setPosition(_scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height)));
        node->addChild(_block); // TODO: third
    }

    void removeBlock(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->removeChild(_block);
    }

    void refreshBatteryNodes(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        if (getCurState()) {
            node->removeChild(_greenBattery); // TODO: delete
            node->addChild(_greenBattery);
        } else {
            node->removeChild(_redBattery);
            node->addChild(_redBattery);
        }
//        node->removeChild(_noBattery);
//        node->addChild(_noBattery);
    }

    void
    initializeBatteryNodes(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        _block = scene2::PolygonNode::allocWithPoly(
            Rect(0, 0, _scene->getSize().width, _scene->getSize().height));
        _block->setColor(Color4::BLACK);
        _block->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _greenBattery->setFrame(0);
        _curBattery = _greenBattery;
        node->addChild(_curBattery);
//        node->addChild(_noBattery);
    }

    void resetScale() {
//        CULog("%f",
//              _greenBattery->getSize().width / _noBattery->getSize().width);
//        _noBatteryScale = _resetScale * _greenBatteryScale * 1.2;
    }

#pragma mark Helpers
  private:
    std::unique_ptr<CameraController>
    makePortrait(int id, Vec3 cameraPosition, Vec3 portraitPosition,
                 Vec3 direction, Vec2 directionLimits, float battery, int type,
                 std::vector<std::shared_ptr<scene2::PolygonNode>>& vector) {
        std::unique_ptr<CameraController> camera =
            std::make_unique<CameraController>(id, _screenSize);
        camera->updatePosition(cameraPosition);
        camera->lookAt(direction);
        camera->updateDirectionLimits(directionLimits);
        camera->updateType(CameraType(type));
        camera->updateBattery(battery);
        _portraitModels.push_back(
            std::make_unique<PortraitModel>(cameraPosition));

        _portraitViews.push_back(std::make_shared<PortraitView>(
            _assets, portraitPosition +
                         Vec2(_assets->get<Texture>("map")->getSize() / 2)));
        vector.emplace_back(_portraitViews[id]->getNode());
        //        _portraitViews[id]->addChildTo(node); //TODO: add to node
        return camera;
    }

    std::vector<std::unique_ptr<CameraController>>::iterator
    getIteratorForIndex(int idx) {
        return _portraits.begin() + idx;
    }

    int getCurFrame() {
        int i = getCurBattery() * _greenBattery->getSpan();
        if (getCurState()) {
            if (i > 0) {
                return _greenBattery->getSpan() - i;
            }
            return _greenBattery->getSpan() - 1;
        } else {
            return _greenBattery->getSpan() - i - 1;
        }
        if (i < _greenBattery->getSpan()) {
            return i;
        }
        return _greenBattery->getSpan() - 1;
    }
};

#endif /* __PORTRAIT_SET_CONTROLLER_H__ */
