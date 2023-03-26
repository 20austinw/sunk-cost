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
    
    std::shared_ptr<cugl::Texture> _lockAsset;
    
    Size _size;

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
    SpiritView(int locks, std::shared_ptr<cugl::Texture> lock){
        _lockAsset = lock;
        for (int i=0; i<locks; i++){
            _locks.emplace_back(scene2::SpriteNode::allocWithSheet(_lockAsset, 2, 8, 12));
        }
        _size = _locks.at(0)->getSize();
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
    
    void updateUnusedLocksPos(Vec2 pos){
        for (int i=0 ; i<_locks.size(); i++){
            _locks.at(i)->setPosition(pos + _size/15*i);
        }
    }
    
    void updateLockInProgress(Vec2 touchPos){
        
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
    
    void removeLastLock(const std::shared_ptr<Scene2>& scene){
        scene->removeChild(_locks.at(_locks.size()-1));
        _locks.pop_back();
    }
    
    Size getLockSize(){
        return _size;
    }
    
    Vec2 getLastLockPos(){
        return _locks.at(_locks.size()-1)->getPosition();
    }

#pragma mark Setters
};

#endif /* _SPIRIT_VIEW_H__ */
