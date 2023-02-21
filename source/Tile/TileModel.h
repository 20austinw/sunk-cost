//
//  TileModel.h
//  Sunk Cost
//
//  This module provides the TileModel class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILE_MODEL_H__
#define _TILE_MODEL_H__

#include <cugl/cugl.h>
using namespace cugl;

/**
 * A class representing a single tile
 */
class TileModel {
    
#pragma mark State
private:
    /** Bottom left corner of the tile */
    Vec2 _position;
    /** Width and height of a single tile */
    Size _size;
    /** Tile color */
    Color4 _color;
    /** Whether this tile is traversable or not */
    bool _traversable;
    
public:
    /** A public accessible, read-only version of the color */
    const Color4& color;
    /** A public accessible, read-only version of _traversable */
    const bool& traversable;
    
    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position     The bottom left corner of the tile
     * @param size         The width and height of a tile
     * @param color        The tile color
     * @param traversable  The tile traversability
     */
    TileModel(Vec2 position, Size size, Color4 color, bool traversable): 
    color(_color), 
    traversable(_traversable) {
        setPosition(position);
        setSize(size);
        setColor(color);
        setTraversable(traversable);
    }

#pragma mark Setters
public:
    /**
     *  Sets the position of the bottom left corner of the tile.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }
    
    /**
     *  Sets the size of the tile.
     *
     *  @param size Width and height of a single tile
     */
    void setSize(Size size) {
        _size = size;
    }
    
    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) {
        _color = color;
    }

    /**
     *  Sets the traversability of the tile.
     *
     *  @param traversable The traversability of the tile
     */
    void setTraversable(bool traversable) {
        _traversable = traversable;
    }
    
};

#endif /* _TILE_MODEL_H__ */
