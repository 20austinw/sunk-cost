//
//  SpiritView.h
//  Sunk Cost
//
//  Created by Austin Wu
//  Version: 2/27/23.
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
    SpiritView();
    
    /** Deletes this SpiritView */
    ~SpiritView() {
        _node->removeFromParent();
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
    
#pragma mark Setters
    /**
     * Sets the position of the bottom left corner of the tile.
     *
     * @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _node->setPosition(position);
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
        _node->setContentSize(size);
    }
    
    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) {
        _node->setColor(color);
    }
};

#endif /* _SPIRIT_VIEW_H__ */