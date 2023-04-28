//
//  TrapView.h
//  Sunk
//
//  Created by Austin Wu on 3/11/23.
//

#ifndef TrapView_h
#define TrapView_h

#include <cugl/cugl.h>

using namespace cugl;

class TrapView {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _shadow;
    float _radius;
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
    TrapView(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position,
             float radius) {
        _frameNum = 0;
        _radius = radius;
        _spriteSheet = assets->get<Texture>("trap_animation2");
        _spriteNode =
            scene2::SpriteNode::allocWithSheet(_spriteSheet, 1, 3, 3);
        _spriteNode->setScale(0.5);
        _spriteNode->setFrame(_frameNum);
        _spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _spriteNode->setPosition(position - _spriteNode->getSize() / 2);
        _spriteNode->setVisible(true);

        // Shadow
        _shadow = scene2::PolygonNode::allocWithTexture(
            assets->get<Texture>("shadow"));
        _shadow->setScale(1);
        _shadow->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _shadow->setPosition(position - _shadow->getSize() / 2 -
                             Vec2(0, _spriteNode->getSize().height / 4));
        _shadow->setVisible(true);
    };

    /** Deletes this HunterView */
    ~TrapView() {
        _spriteNode->removeFromParent();
        _shadow->removeFromParent();
    }

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
        scene->addChild(_shadow);
        scene->addChild(_spriteNode);
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_shadow);
        scene->removeChild(_spriteNode);
    }
    
    void setVisible(bool isVisible,int framenum){
        _shadow->setVisible(isVisible);
        _spriteNode->setVisible(isVisible);
       
        _spriteNode->setFrame(int(framenum/4)%3);
        
        
        
    }

#pragma mark Setters
    void setPosition(Vec2 position) { _spriteNode->setPosition(position); }
    
    void update() {
        if (_tick % 4 == 0 && _frameNum < 3) {
            _frameNum = (_frameNum + 1) % _spriteNode->getSpan();
            _spriteNode->setFrame(_frameNum);
        }
        _tick += 1;
    }
};

#endif /* TrapView_h */
