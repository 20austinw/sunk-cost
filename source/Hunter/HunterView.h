//
//  HunterView.h
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//

#ifndef _HUNTER_VIEW_H
#define _HUNTER_VIEW_H
#include <cugl/cugl.h>

using namespace cugl;

class HunterView {
#pragma mark Internal References
private:
    std::shared_ptr<scene2::PolygonNode> _node;
    std::vector<std::shared_ptr<cugl::Texture>> _spriteSheets;
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> _spriteNodes;
    int  _frameNum;




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

    HunterView(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position, Size size){

        _frameNum = 8;
        
        CULog("truee %d", assets->get<Texture>("hunterrunning")==nullptr);
        

        _spriteSheets.push_back(assets->get<Texture>("hunterrunning"));
        _spriteSheets.push_back(assets->get<Texture>("hunterleft"));
        
        
        float width = size.width * 1.5f;
        
        for (int i = 0; i < _spriteSheets.size(); i++) {
//            CULog("sizee %d",_spriteSheets.size());
            _spriteNodes.push_back(scene2::SpriteNode::allocWithSheet(_spriteSheets[i], 2, 8, 10));
            _spriteNodes[i]->setScale(0.5);
            _spriteNodes[i]->setFrame(8);
            _spriteNodes[i]->setAnchor(Vec2::ANCHOR_CENTER);
            _spriteNodes[i]->setPosition(Vec2(0, width / 2.5f));
            _spriteNodes[i]->setVisible(false);
        }
        _spriteNodes[0]->setVisible(true);
        

    };
    
    
    /** Deletes this HunterView */
    ~HunterView() {
        for (int i = 0; i< _spriteNodes.size();i++){
            _spriteNodes[i]->removeFromParent();
        }
    }
    
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        for (int i = 0; i< _spriteNodes.size();i++){
            scene->addChild(_spriteNodes[i]);
        }
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        for (int i = 0; i< _spriteNodes.size();i++){
            scene->removeChild(_spriteNodes[i]);
        }
    }
#pragma mark Getters
public:
    /** Returns the SpriteNodes */
    const std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> getSpriteNode() const {
        // TODO: Implement me
        return _spriteNodes;
    }
    
#pragma mark Setters
    /**
     * Sets the position of the bottom left corner of the tile.
     *
     * @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        for (int i = 0; i< _spriteNodes.size();i++){
            _spriteNodes[i]->setPosition(position);
        }
    }
    
    /**
     * Sets the size of the tile.
     *
     * As the view is a PolygonNode, this means setting a new polygon
     * that is a rectangle whose size is the one given. The rectangle
     * however, is only the shape, so it can have a zero origin.
     *
     * @param size Width and height of a single tile
     */
    void setSize(Size size) {
        for (int i = 0; i< _spriteNodes.size();i++){
            _spriteNodes[i]->setContentSize(size);
        }
       
    }
    
    /**
     * Determines the next animation frame for the ship and applies it to the sprite.
     *
     * This method includes some dampening of the turn, and should be called before
     * moving the ship.
     */
    void advanceFrame(int forward, int right) {
            if(_frameNum >=9){
                _frameNum = 0;
            }
            if(forward==0 && right == 0){
                _spriteNodes[0]->setFrame(8);
                _spriteNodes[1]->setFrame(8);
            }
    
            else if (right == -1){
                _spriteNodes[0]->setVisible(false);
                _spriteNodes[1]->setVisible(true);
                
                _spriteNodes[1]->setFrame(_frameNum);
                _frameNum++;
            }else{
                _spriteNodes[1]->setVisible(false);
                _spriteNodes[0]->setVisible(true);
                _spriteNodes[0]->setFrame(_frameNum);
                _frameNum++;
            }
            
        }
};


#endif /* _HUNTER_VIEW_H__ */
