//
//  PortraitSetController.h
//  Sunk Cost
//
//  Author:  Soon Jae Park
//  Version: 2/22/23
//
#ifndef __PORTRAIT_SET_CONTROLLER_H__
#define __PORTRAIT_SET_CONTROLLER_H__
#include <cugl/cugl.h>
#include <memory>
#include "PortraitSet.h"
#include "CameraController.h"

using namespace cugl;

class PortraitSetController {
  #pragma mark Internal References
  private:
    /** The set of portraits */
    std::vector<std::unique_ptr<CameraController>> _portraits;
    /** The Camera object */
    std::unique_ptr<PortraitSet> _model;
    Size _screenSize;
    /** The max batteryt */
    int _maxBattery;
    
    std::shared_ptr<scene2::PolygonNode> _bound;
    std::shared_ptr<scene2::PolygonNode> _charge;
    std::shared_ptr<scene2::PolygonNode> _block;

  #pragma mark Main Functions
  public:
    /**
     * Constructor for Camera Controller
     */
    PortraitSetController(int id = 0, Size screenSize = Size::ZERO, int maxBattery = 600) {
        CULog("Initialized!");
      _model = std::make_unique<PortraitSet>();
        _screenSize = screenSize;
        _maxBattery = maxBattery;
      initializePortraitSet();
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
    void addPortrait(int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int battery=600, int type = 2) {
        _portraits.push_back(makePortrait(id, position, direction, directionLimits, battery, type));
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
    void insertPortraitTo(int index, int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int battery = 600, int type = 2) {
        _portraits.insert(getIteratorForIndex(index), makePortrait(id, position, direction, directionLimits, battery, type));
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
    void initializePortraitSet(int id = 0, Vec3 position = Vec2::ZERO, Vec3 direction = Vec3::ZERO, Vec2 directionLimits = Vec2::ZERO, int battery = 600, int type = 2) {
        _portraits.push_back(makePortrait(id, position, direction, directionLimits, battery, type));
      _model->setIndex(0);
    }

    /**
     * Adds a new portrait to the portraitset
     */
    void clearPortraitSet() {
      _portraits.clear();
      _model->setIndex(-1);
    }
    
    #pragma mark Main Functions
    void setIndex(int index) {
        _model->setIndex(index);
    }

  #pragma mark Getters
  public:
    /**
     * Getter for viewport of this camera
     *
     * @return Rectangle that contains the camera viewpoint
     */
    const Rect getViewPortrait() {
      return _portraits[_model->getIndex()]->getViewPort();
    }

    /**
     * Getter for current camera index
     *
     * @return Rectangle that contains the camera viewpoint
     */
    int getIndex() {
      return _model->getIndex();
    }
    
    Vec3 getPosition(int index) {
        return _portraits[index]->getPosition();
    }

  #pragma mark Main Functions
  public:
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
     * Updates camera direction limits. [d.x,d.y] should be the range of possible values for angle.
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
    void updateType(CameraType type) {
      _portraits[_model->getIndex()]->updateType(type);
    }

    /**
     * Rotates the camera by angle
     *
     * @param angle how much the camera will be rotated by
     */
    void rotate(float angle) {
      _portraits[_model->getIndex()]->rotate(angle);
    }

    /**
     * Modifies the camera to look at the given target
     *
     * @param target the point to look at
     */
    void lookAt(const Vec3 target) {
      _portraits[_model->getIndex()]->lookAt(target);
    }
    
    void updateBattery(){
        for (int i=0;i<_portraits.size();i++){
            if(((_model->getIndex() != _portraits[i]->getID()) || (!_portraits[i]->getState())) && (_portraits[i]->getBattery() < _maxBattery)){
                _portraits[i]->updateBattery(_portraits[i]->getBattery()+1);
            } else if (_model->getIndex() == _portraits[i]->getID() && _portraits[i]->getBattery() > 0){
                _portraits[i]->updateBattery(_portraits[i]->getBattery()-1);
            }
            if (_portraits[i]->getBattery() == 0){
                _portraits[i]->updateState(false);
            } else if (!_portraits[i]->getState() && _portraits[i]->getBattery() >= 0.8 * _maxBattery){
                _portraits[i]->updateState(true);
            }
        }
    }
    
    void updateBatteryNode(){
        Rect offset = _portraits[_model->getIndex()] -> getViewPort();
        Vec2 pos = _portraits[_model->getIndex()]->getPosition() + Vec2(offset.size)/2;
        _bound->setPosition(pos);
        _charge->setPolygon(Rect(0, 0, getCurBattery()*100, 20));
        _charge->setPosition(pos + Vec2((getCurBattery()*100-100)/2,0));
        if(getCurState()){
            _charge->setColor(Color4::GREEN);
        }else{
            _charge->setColor(Color4::RED);
        }
        
        
    }
    
    float getCurBattery(){
        return (float)_portraits[_model->getIndex()]->getBattery() / _maxBattery;
    }
    
    bool getCurState(){
        return _portraits[_model->getIndex()]->getState();
    }
    
    void setMaxbattery(int maxBattery){
        _maxBattery = maxBattery;
    }
    
    void addBlock(const std::shared_ptr<cugl::Scene2>& scene){
        scene->addChild(_block);
    }
    
    void removeBlock(const std::shared_ptr<cugl::Scene2>& scene){
        scene->removeChild(_block);
    }
    
    void refreshBatteryNodes(const std::shared_ptr<cugl::Scene2>& scene){
        scene->removeChild(_bound);
        scene->removeChild(_charge);
        scene->addChild(_bound);
        scene->addChild(_charge);
    }
    
    void initializeBatteryNodes(const std::shared_ptr<cugl::Scene2>& scene){
        _bound = scene2::PolygonNode::allocWithPoly(Rect(0, 0, 100, 20));
        _bound->setColor(Color4::WHITE);
        _charge = scene2::PolygonNode::allocWithPoly(Rect(0, 0, 100, 20));
        _charge->setColor(Color4::GREEN);
        _block = scene2::PolygonNode::allocWithPoly(Rect(0, 0, scene->getSize().width, scene->getSize().height));
        _block->setColor(Color4::BLACK);
        scene->addChild(_bound);
        scene->addChild(_charge);
    }

  #pragma mark Helpers
  private:
    std::unique_ptr<CameraController> makePortrait(int id, Vec3 position, Vec3 direction, Vec2 directionLimits, int battery, int type) {
        std::unique_ptr<CameraController> camera = std::make_unique<CameraController>(id, _screenSize);
        camera->updatePosition(position);
        camera->lookAt(direction);
        camera->updateDirectionLimits(directionLimits);
        camera->updateType(CameraType(type));
        camera->updateBattery(battery);
      return camera;
    }

    std::vector<std::unique_ptr<CameraController>>::iterator getIteratorForIndex(int idx) {
      return _portraits.begin() + idx;
    }
};




#endif /* __PORTRAIT_SET_CONTROLLER_H__ */
