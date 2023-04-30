//
//  TileView.h
//  Sunk Cost
//
//  This module provides the TileView class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILE_VIEW_H__
#define _TILE_VIEW_H__
#include <cugl/cugl.h>
using namespace cugl;

/**
 * A class representing the interface for a single tile view.
 *
 * There is very little that this class does that a SceneNode (or
 * PolygonNode) does not already do. So technically we could use
 * one of those classes for the view. But by pulling it out as its
 * own class, we make the relationship explicit.
 */
class TileView {

#pragma mark Internal References
  private:
    /** Main tile view */
    std::shared_ptr<scene2::PolygonNode> _node;

#pragma mark Main Functions
  public:
    /**
     * Creates the view.
     *
     * In addition to the passed arguments, the node's anchor is set to the
     * bottom left and it should not be tinted relative to it's parent's color.
     *
     * @param position  The bottom left corner of the tile
     * @param size The width and height of a tile
     * @param color The tile color tint
     */
    TileView(Vec2 position, Size size, Color4 color,
             const std::shared_ptr<Texture>& texture) {
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setTexture(texture);
        _node->setPosition(position);
        _node->setColor(color);
        _node->setPolygon(Rect(Vec2::ZERO, size));
    }

    /**
     * Deletes this TileView
     *
     * Upon destruction, you should remove the node from its parent.
     */
    ~TileView() { _node->removeFromParent(); }

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
  public:
    /**
     * Sets the position of the bottom left corner of the tile.
     *
     * @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) { _node->setPosition(position); }

    /**
     * Sets the size of the tile.
     *
     * As the view is a PolygonNode, this means setting a new polygon
     * that is a rectangle whose size is the one given. The rectangle
     * however, is only the shape, so it can have a zero origin.
     *
     * @param size Width and height of a single tile
     */
    void setSize(Size size) { _node->setPolygon(Rect(Vec2::ZERO, size)); }

    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) { _node->setColor(color); }
};

#endif /* _TILE_VIEW_H__ */
