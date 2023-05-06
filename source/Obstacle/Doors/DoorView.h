//
//  DoorView.h
//  Sunk
//
//  Created by Xiaoyue Jin on 3/24/23.
//

#ifndef DoorView_h
#define DoorView_h

#include <cugl/cugl.h>

using namespace cugl;

class DoorView {

#pragma mark Internal References
  private:
    std::shared_ptr<scene2::SpriteNode> _door;
    Size _size;
    Vec2 _position;

#pragma mark Main Functions
  public:
    DoorView(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position,
             int type, int playerType = 0) {
        _position = position;
        if (playerType == 1) {
            if (type == 0) {
                _door = scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("front_lock"), 3, 7, 21);
            } else if (type == 1) {
                _door = scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("side_lock"), 4, 5, 18);
            }
            _size = _door->getSize();
            _door->setPosition(position);
            _door->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
            setFrame(0);
            return;
        } else {
            if (type == 0) {
                _door = scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("door_animation_hunter"), 3, 5, 12);
            } else if (type == 1) {
                _door = scene2::SpriteNode::allocWithSheet(
                    assets->get<Texture>("side_lock"), 4, 5, 18);
            }
            _size = _door->getSize();
            _door->setPosition(position);
            _door->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
            setFrame(0);
            return;
        }
    }

#pragma mark Getters
    Size getSize() { return _size; }

    Vec2 getPosition() { return _position; }

    std::shared_ptr<scene2::SpriteNode> getNode() { return _door; }

#pragma mark Setters
    void setFrame(int frame) { _door->setFrame(frame); }

#pragma mark Scene Methods

  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_door);
    }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_door);
    }
};

#endif /* DoorView_h */
