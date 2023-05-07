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
    int _tick;
    int _idx;
    bool _win;
    bool _spirit;
    bool _added;
    bool halt;
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> _spriteNodes;
    std::shared_ptr<cugl::Scene2> _scene;
#pragma mark Internal functions
  private:
    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }
#pragma mark Main Functions
  public:
    EndScene(const std::shared_ptr<cugl::Scene2>& scene,
             const std::shared_ptr<cugl::AssetManager>& assets, bool spirit,
             bool win) {

        _scene = scene;
        _win = win;
        _spirit = spirit;
        _tick = 0;
        _frameNum = 0;
        _idx = 0;
        _added = false;
        halt = false;

        if (spirit) {
            if (win) {
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("spirit_win_part_1"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("spirit_win_part_2"), 5, 2, 9));
            } else {
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("spirit_lose_part_1"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("spirit_lose_part_2"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("spirit_lose_part_3"), 3, 2, 6));
            }
        } else {
            if (win) {
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("hunter_win_part_1"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("hunter_win_part_2"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("hunter_win_part_3"), 3, 2, 6));
            } else {
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("hunter_lose_part_1"), 5, 2, 10));
                _spriteNodes.emplace_back(scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("hunter_lose_part_2"), 5, 2, 9));
            }
        }
        for (auto& spriteNode : _spriteNodes) {
            spriteNode->setScale(_scene->getSize().height /
                                 spriteNode->getSize().height / getZoom());
            spriteNode->setFrame(0);
            spriteNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
            spriteNode->setVisible(false);
        }
        _spriteNodes[0]->setVisible(true);
    };

    /** Deletes this HunterView */
    ~EndScene() {
        for (auto& spriteNode : _spriteNodes) {
            spriteNode->removeFromParent();
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
        for (auto& spriteNode : _spriteNodes) {
            scene->addChild(spriteNode);
        }
    }
    
    bool isAdded() {
        return _added;
    }
    
    void setAdded(bool add) {
        _added = add;
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        for (auto& spriteNode : _spriteNodes) {
            scene->removeChild(spriteNode);
        }
    }

#pragma mark Setters
    void setPosition(Vec2 position) {
        _spriteNodes[_idx]->setPosition(position);
    }

    
    void update() {
        if (!halt) {

            if (_tick % 7 == 0) {
                _frameNum++;
                _spriteNodes[_idx]->setFrame(_frameNum);
            }

            if (_frameNum >= _spriteNodes[_idx]->getSpan()) {
                _spriteNodes[_idx]->setVisible(false);
                _idx++;
                _frameNum = 0;
            }

            if (_idx == _spriteNodes.size() - 1 &&
                _frameNum == _spriteNodes[_idx]->getSpan() - 1) {
                halt = true;
            }

            _tick++;
        }

        _spriteNodes[_idx]->setVisible(true);
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height));
        _spriteNodes[_idx]->setPosition(pos);
    }
    
    bool sUpdate() {
        if (!halt) {

            if (_tick % 7 == 0) {
                _frameNum++;
                _spriteNodes[_idx]->setFrame(_frameNum);
            }

            if (_frameNum >= _spriteNodes[_idx]->getSpan()) {
                _spriteNodes[_idx]->setVisible(false);
                _idx++;
                _frameNum = 0;
            }

            if (_idx == _spriteNodes.size() - 1 &&
                _frameNum == _spriteNodes[_idx]->getSpan() - 1) {
                halt = true;
            }

            _tick++;
        }

        _spriteNodes[_idx]->setVisible(true);
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            Vec2(0, _scene->getSize().height));
        _spriteNodes[_idx]->setPosition(pos);
        return halt;
    }
};

#endif /* EndScene_h */
