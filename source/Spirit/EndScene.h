//
//  EndScene.h
//  Sunk
//
//  Created by Austin Wu on 3/24/23.
//

#ifndef EndScene_h
#define EndScene_h

#include <cugl/cugl.h>

using namespace cugl;

class EndScene {
#pragma mark Internal References
  private:
    int _frameNum;
    int _tick = 0;
    bool _win;
    std::shared_ptr<cugl::Texture> _spriteSheet;
    std::shared_ptr<cugl::scene2::SpriteNode> _spriteNode;
    std::shared_ptr<scene2::PolygonNode> _blurNode;
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
    EndScene(const std::shared_ptr<cugl::AssetManager>& assets, bool win) {
        _win = win;
        _frameNum = 0;
        _spriteSheet = win ? assets->get<Texture>("spirit_win_animation") : assets->get<Texture>("spirit_win_animation");
        _spriteNode = scene2::SpriteNode::allocWithSheet(_spriteSheet, 7, 5, 32);
        _spriteNode->setScale(1);
        _spriteNode->setFrame(_frameNum);
        _spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _spriteNode->setVisible(true);
        _blurNode = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("blur"));
        _blurNode->setScale(5);
        _blurNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _blurNode->setVisible(true);
    };

    /** Deletes this HunterView */
    ~EndScene() {
        _spriteNode->removeFromParent();
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
        _blurNode->setPosition(Vec2(scene->getCamera()->getPosition()) - _blurNode->getSize()/2);
        scene->addChild(_blurNode);
        _spriteNode->setPosition(Vec2(scene->getCamera()->getPosition()) - _spriteNode->getSize()/2);
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
    
    void update() {
        if(_win) {
            // First round full animation
            if(_tick % 10 == 0) {
                if(_frameNum == _spriteNode->getSpan() - 1){
                    _frameNum = 11;
                }
                _tick = 0;
                _frameNum++;
                _spriteNode->setFrame(_frameNum);
            }
            _tick++;
            // Second round
        }else{
            
        }
    }
};

#endif /* EndScene_h */
