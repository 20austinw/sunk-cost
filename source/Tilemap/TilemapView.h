//
//  TilemapView.h
//  Sunk Cost
//
//  This module provides the TilemapView class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILEMAP_VIEW_H__
#define _TILEMAP_VIEW_H__

#include <cugl/cugl.h>
using namespace cugl;

/**
 * A class representing the interface for the entire tile map view.
 *
 * There is very little that this class does that a SceneNode (or
 * PolygonNode) does not already do. So technically we could use
 * one of those classes for the view. But by pulling it out as its
 * own class, we make the relationship explicit.
 */
class TilemapView {
    
#pragma mark Internal References
private:
    /** Main tilemap view */
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark Main Functions
public:
    /**
     * Initializes the view.
     *
     * In addition to the passed arguments, the node's anchor is set to the bottom left
     * and it should not be tinted relative to it's parent's color.
     *
     * @param position      The bottom left corner of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapView(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize) {
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setPolygon(Rect(position,tileSize*dimensions));
        _node->setRelativeColor(false);
        _node->setColor(color);
    }

    /**
     * Deletes this TileView
     *
     * Upon destruction, you should remove the node from its parent.
     */
    ~TilemapView() {
        _node->removeFromParent();
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `scene`.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_node);
    }
    
    /**
     * Removes the view component children from the given `scene`.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_node);
    }
    
    /** Returns the main tilemap view */
    const std::shared_ptr<scene2::PolygonNode>& getNode() const {
        return _node;
    }

#pragma mark Setters
public:
    
    /**
     * Sets the bottom left corner of the tilemap
     *
     * @param position Bottom left corner of the tilemap
     */
    void setPosition(Vec2 position) {
        _node->setPosition(position);
    }
    
    /**
     * Sets the size of the tilemap.
     *
     * The size of the tilemap should be `dimensions * tileSize`. As
     * the view is a PolygonNode, this means setting a new polygon
     * that is a rectangle whose size is that of the tilemap. The
     * rectangle however, is only the shape, so it can have a zero
     * origin.
     *
     * @param size    The size of the tilemap based on its dimensions and tileSize
     */
    void setSize(Size size) {
        _node->setPolygon(Rect(Vec2(0,0),size));
    }
    
};

#endif /* _TILEMAP_VIEW_H__ */
