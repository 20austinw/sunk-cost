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
    SpiritView(){};

    /** Deletes this HunterView */
    ~SpiritView() { _node->removeFromParent(); }

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

#pragma mark Setters
};

#endif /* _SPIRIT_VIEW_H__ */
