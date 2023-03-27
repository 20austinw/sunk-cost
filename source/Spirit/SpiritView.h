//
//  Spirit.h
//  Sunk Cost
//
//  Created by Austin Wu
//  Version: 3/5/23.
//

#ifndef _SPIRIT_VIEW_H
#define _SPIRIT_VIEW_H
#include <cugl/cugl.h>

using namespace cugl;

class SpiritView {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _node;
    
    std::vector<std::shared_ptr<scene2::SpriteNode>> _locks;
    
    std::vector<std::shared_ptr<scene2::PolygonNode>> _trapButtons;
    
    std::shared_ptr<cugl::Texture> _lockAsset;
    
    std::shared_ptr<cugl::Texture> _trapAsset;
    
    Size _lockSize;
    
    Size _trapSize;
    
    std::shared_ptr<cugl::Scene2> _scene;

#pragma mark Main Functions
  public:
    /**
     * TODO: Implement Me
     * Creates the view.
     *
     * In addition to the passed arguments, the node's anchor is set to the
     * bottom left and it should not be tinted relative to it's parent's color.
     *
     * @param position  The bottom left corner of the tile
     * @param size The width and height of a tile
     * @param color The tile color tint
     */
    SpiritView(int locks, int traps, std::shared_ptr<cugl::Texture> lock, std::shared_ptr<cugl::Texture> trap, std::shared_ptr<cugl::Scene2> &scene){
        _scene = scene;
        _lockAsset = lock;
        for (int i=0; i<locks; i++){
            _locks.emplace_back(scene2::SpriteNode::allocWithSheet(_lockAsset, 2, 8, 12));
            _locks.at(i)->setFrame(0);
            _locks.at(i)->setScale(_lockAsset->getSize().height/_scene->getSize().height);
        }
        _lockSize = _locks.at(0)->getSize();
        
        _trapAsset = trap;
        for (int i=0; i<traps; i++){
            _trapButtons.emplace_back(scene2::PolygonNode::allocWithTexture(_trapAsset));
            _trapButtons.at(i)->setScale(_lockSize.height/_trapAsset->getSize().height);
        }
        _trapSize = _trapButtons.at(0)->getSize();
    }

    /** Deletes this HunterView */
    ~SpiritView() { _node->removeFromParent(); }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<Scene2>& scene) {
        scene->addChild(_node);
    }
    
    void addLocksTo(const std::shared_ptr<Scene2>& scene) {
        for (int i=0; i<_locks.size(); i++){
            scene->addChild(_locks.at(i));
        }
    }
    
    void addTrapButtonsTo(const std::shared_ptr<Scene2>& scene) {
        for (int i=0; i<_trapButtons.size(); i++){
            scene->addChild(_trapButtons.at(i));
        }
    }
    
    void updateUnusedLocksPos(Vec2 pos){
        for (int i=0 ; i<_locks.size(); i++){
            _locks.at(i)->setPosition(pos + _lockSize/15*i);
        }
    }
    
    void updateUnusedTrapsPos(Vec2 pos){
        for (int i=0 ; i<_trapButtons.size(); i++){
            _trapButtons.at(i)->setPosition(pos + _trapSize/15*i);
        }
    }
    
    void updateLockInProgress(Vec2 touchPos){
        _locks.at(_locks.size()-1)->setPosition(touchPos);
    }
    
    void updateTrapInProgress(Vec2 touchPos){
        _trapButtons.at(_trapButtons.size()-1)->setPosition(touchPos);
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<Scene2>& scene) {
        scene->removeChild(_node);
    }
    
    void removeLocksFrom(const std::shared_ptr<Scene2>& scene) {
        for (int i=0; i<_locks.size(); i++){
            scene->removeChild(_locks.at(i));
        }
    }
    
    void removeTrapsFrom(const std::shared_ptr<Scene2>& scene) {
        for (int i=0; i<_trapButtons.size(); i++){
            scene->removeChild(_trapButtons.at(i));
        }
    }
    
    void removeLastLock(const std::shared_ptr<Scene2>& scene){
        scene->removeChild(_locks.at(_locks.size()-1));
        _locks.pop_back();
    }
    
    void removeLastTrapButton(const std::shared_ptr<Scene2>& scene){
        scene->removeChild(_trapButtons.at(_trapButtons.size()-1));
        _trapButtons.pop_back();
    }
    
    Size getLockSize(){
        return _lockSize;
    }
    
    Size getTrapSize(){
        return _trapSize;
    }
    
    Vec2 getLastLockPos(){
        return _locks.at(_locks.size()-1)->getPosition();
    }
    
    Vec2 getLastTrapBtnPos() {
        return _trapButtons.at(_trapButtons.size()-1)->getPosition();
    }
    
    void addNewTrap(const std::shared_ptr<Scene2>& scene){
        _trapButtons.emplace_back(scene2::PolygonNode::allocWithTexture(_trapAsset));
        _trapButtons[_trapButtons.size()-1]->setScale(_lockSize.height/_trapAsset->getSize().height);
        scene->addChild(_trapButtons.at(_trapButtons.size()-1));
    }

#pragma mark Setters
};

#endif /* _SPIRIT_VIEW_H__ */
