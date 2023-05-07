//
//  PortraitView.h
//  Sunk
//
//  Created by Austin Wu on 3/16/23.
//

#ifndef PortraitView_h
#define PortraitView_h

#include <cugl/cugl.h>

using namespace cugl;

class PortraitView {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _node;
    int _frameNum;
    int _tick = 0;
    std::shared_ptr<cugl::Texture> _spriteSheet;
    std::shared_ptr<cugl::scene2::SpriteNode> _spriteNode;
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
    PortraitView(const std::shared_ptr<cugl::AssetManager>& assets,
                 Vec2 position,bool ishunter) {
        _frameNum = 0;
        _spriteSheet = assets->get<Texture>("painting_animation");
        _spriteNode =
        scene2::SpriteNode::allocWithSheet(_spriteSheet, 5, 5, 23);
        if (ishunter){
            _spriteSheet = assets->get<Texture>("painting_animation_hunter");
            _spriteNode =
            scene2::SpriteNode::allocWithSheet(_spriteSheet, 7, 5, 32);
        }
        _spriteNode->setScale(0.5);
        _spriteNode->setFrame(_frameNum);
        _spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _spriteNode->setPosition(position - _spriteNode->getSize() / 2);
        _spriteNode->setVisible(true);
    };

    /** Deletes this HunterView */
    ~PortraitView() { _spriteNode->removeFromParent(); }

#pragma mark Getters
  public:
    //    std::shared_ptr<scene2::PolygonNode> getNode() { return _spriteNode; }
    std::shared_ptr<cugl::scene2::SpriteNode> getNode() { return _spriteNode; }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->addChild(_spriteNode);
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_spriteNode);
    }

#pragma mark Setters
    void setPosition(Vec2 position) { _spriteNode->setPosition(position); }
    void reset() {
        _frameNum = 0;
        _spriteNode->setFrame(0);
    }
    int v = 1;
    void update() {
        if (_tick % 15 == 0) {
            _tick = 0;
            _frameNum = (_frameNum + 1) % _spriteNode->getSpan();
            _spriteNode->setFrame(_frameNum);
        }
        _tick++;
    }

    int getTick() { return _tick; }
};

#endif /* PortraitView_h */
