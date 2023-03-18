//
//  TreasureView.h
//  Sunk
//
//  Created by 任清扬 on 16/3/23.
//

#ifndef TreasureView_h
#define TreasureView_h

#include <cugl/cugl.h>

using namespace cugl;

class TreasureView {
#pragma mark Internal References
private:
    std::shared_ptr<scene2::PolygonNode> _node;
//    std::vector<std::shared_ptr<cugl::Texture>> _spriteSheets;
//    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> _spriteNodes;
//    int  _frameNum;
    
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
    TreasureView(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position, Size size){
//        _node = scene2::PolygonNode::alloc();
//        _node->setColor(Color4::PAPYRUS);
//        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//        setPosition(position - size);
//        _node->setPolygon(Rect(position, size));
        
        _node = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("treasure"));
        _node ->setScale(0.15);
//        _frameNum = 0;
//
//        _spriteSheets.push_back(assets->get<Texture>("clamtrap_anim"));
//
//
//        float width = size.width * 1.5f;
//
//        for (int i = 0; i < _spriteSheets.size(); i++) {
//            _spriteNodes.push_back(scene2::SpriteNode::allocWithSheet(_spriteSheets[i], 1, 8, 8));
//            _spriteNodes[i]->setScale(0.4);
//            _spriteNodes[i]->setFrame(0);
//
//            _spriteNodes[i]->setAnchor(Vec2::ANCHOR_CENTER);
//            _spriteNodes[i]->setPosition(Vec2(0, width / 2.5f));
//            _spriteNodes[i]->setVisible(false);
//            _node->addChild(_spriteNodes[i]);
//        }
//        _spriteNodes[0]->setVisible(true);
//
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setPosition(position);
        _node->setPolygon(Rect(Vec2::ZERO, size));
    };
    
    
    /** Deletes this HunterView */
    ~TreasureView() {
        _node->removeFromParent();
    }
    
#pragma mark Getters
public:
    std::shared_ptr<scene2::PolygonNode> getNode() {
        return _node;
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<scene2::SceneNode>& sceneNode) {
        sceneNode->addChild(_node);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<scene2::SceneNode>& sceneNode) {
        sceneNode->removeChild(_node);
    }
    
    
//    /**
//     * Determines the next animation frame for the ship and applies it to the sprite.
//     *
//     * This method includes some dampening of the turn, and should be called before
//     * moving the ship.
//     */
//    void advanceFrame() {
//
//        if(_frameNum >=7){
//            _spriteNodes[0]->setVisible(false);
//            _frameNum=0;
//        }
//        _spriteNodes[0]->setFrame(_frameNum);
//        _frameNum++;
//
//        }
    
#pragma mark Setters
    void setPosition(Vec2 position) {
        _node->setPosition(position);
    }
};


#endif /* TreasureView_h */
