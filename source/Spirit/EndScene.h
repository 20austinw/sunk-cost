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
    bool _spirit;
    std::shared_ptr<cugl::Texture> _spriteSheet;
    std::shared_ptr<cugl::scene2::SpriteNode> _spriteNode;
    std::shared_ptr<cugl::Scene2> _scene;
    std::shared_ptr<cugl::scene2::Label> _endLabel;
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
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
    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }

    EndScene(const std::shared_ptr<cugl::Scene2>& scene,
             const std::shared_ptr<cugl::AssetManager>& assets, bool spirit,
             bool win) {
        _scene = scene;
        _win = win;
        _spirit = spirit;
        //        _frameNum = 0;
        //        _spriteSheet = win ? assets->get<Texture>("hunters_lose")
        //                           : assets->get<Texture>("hunters_win");
        //        _spriteNode =
        //            win ? scene2::SpriteNode::allocWithSheet(_spriteSheet, 2,
        //            8, 14)
        //                : scene2::SpriteNode::allocWithSheet(_spriteSheet, 5,
        //                4, 20);
        //        std::string endText = win ? "You win!" : "You lose!";
        //        _endLabel = cugl::scene2::Label::allocWithText(
        //            Vec2(0, 0), endText, assets->get<Font>("pixel32"));
        //        _spriteNode->setScale(_scene->getSize().height * 0.7 /
        //                              _spriteNode->getSize().height /
        //                              getZoom());
        //        _spriteNode->setFrame(_frameNum);
        //        _spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        //        _spriteNode->setVisible(true);
        if (!spirit) {
            _node = win ? cugl::scene2::PolygonNode::allocWithTexture(
                              assets->get<Texture>("hunter_lose"))
                        : cugl::scene2::PolygonNode::allocWithTexture(
                              assets->get<Texture>("hunter_win"));
        } else {
            _node = win ? cugl::scene2::PolygonNode::allocWithTexture(
                              assets->get<Texture>("spirit_win"))
                        : cugl::scene2::PolygonNode::allocWithTexture(
                              assets->get<Texture>("spirit_lose"));
        }
        _node->setScale(_scene->getSize().height / _node->getSize().height /
                        getZoom());
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height));
        _node->setPosition(pos);
        _node->setVisible(true);
    };

    /** Deletes this HunterView */
    ~EndScene() { _node->removeFromParent(); }

#pragma mark Getters
  public:
    std::shared_ptr<scene2::PolygonNode> getNode() { return _node; }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_node);
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_node);
    }

#pragma mark Setters
    void setPosition(Vec2 position) { _node->setPosition(position); }

    int v = 1;
    void update() {
        //        if (_win) {
        //            // First round full animation
        //            if (_tick % 10 == 0) {
        //                if (_frameNum == _spriteNode->getSpan() - 1) {
        //                    v = -1;
        //                } else if (_frameNum == 8) {
        //                    v = 1;
        //                }
        //                _tick = 0;
        //                _frameNum += v;
        //                _spriteNode->setFrame(_frameNum);
        //            }
        //            _tick++;
        //            // Second round
        //        } else {
        //            _spriteNode->setFrame(_frameNum % 17);
        //            _frameNum++;
        //        }
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height));
        _node->setPosition(pos);
    }
};

#endif /* EndScene_h */
