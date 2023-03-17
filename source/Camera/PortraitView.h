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
                 Vec2 position) {
        _frameNum = 0;
        _spriteSheet = assets->get<Texture>("painting_animation");
        _spriteNode =
            scene2::SpriteNode::allocWithSheet(_spriteSheet, 2, 8, 16);
        _spriteNode->setScale(1);
        _spriteNode->setFrame(_frameNum);
        _spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        CULog("%f, %f", _spriteNode->getSize().width,
              _spriteNode->getSize().height);
        _spriteNode->setPosition(position);
        _spriteNode->setVisible(true);
    };

    /** Deletes this HunterView */
    ~PortraitView() { _spriteNode->removeFromParent(); }

#pragma mark Getters
  public:
    std::shared_ptr<scene2::PolygonNode> getNode() { return _spriteNode; }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_spriteNode);
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
    int v = 1;
    void update() {
        // For hunters: Probably want to have a parameter (i.e.
        // update(hunter={true, false})) that determines which animation to use
        //        if(_tick % 5 == 0 && _frameNum < _spriteNode->getSpan()-1) {
        //            _tick = 0;
        //            _frameNum++;
        //            _spriteNode->setFrame(_frameNum);
        //        }
        //        _tick++;
        int threshold = 10;
        if (_tick % threshold == 0) {
            _tick = 0;
            if (_frameNum == _spriteNode->getSpan() / 2 - 1)
                v = -1;
            if (_frameNum == 0)
                v = 1;
            _frameNum = _frameNum + v;
            _spriteNode->setFrame(_frameNum);
        }
        _tick += 1;
    }
};

#endif /* PortraitView_h */
