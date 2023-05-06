//
//  TilemapModel.h
//  Sunk Cost
//
//  This module provides the TilemapModel class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILEMAP_MODEL_H__
#define _TILEMAP_MODEL_H__

#include <cugl/cugl.h>
using namespace cugl;

/**
 * A class representing the entire tilemap.
 */
class TilemapModel {

#pragma mark State
  private:
    /** Bottom left corner of the tilemap */
    Vec2 _position;
    /** The number of tiles per column and row   */
    Vec2 _dimensions;
    /** Tilemap color */
    Color4 _color;
    /** Width and height of a single tile */
    Size _tileSize;

  public:
    const Vec2& position;
    const Vec2& dimensions;
    const Color4& color;
    const Size& tileSize;

#pragma mark Main Functions
  public:
    /**
     *  Creates the default tilemap.
     *
     *  The default tilemap is a black square whose bottom left corner is
     *  aligned with the bottom left corner of the screen. It's dimensions
     *  are 10 by 10 with tiles of size 5 by 5.
     */
    TilemapModel()
        : position(_position), dimensions(_dimensions), color(_color),
          tileSize(_tileSize) {
        setPosition(Vec2(0, 0));
        setDimensions(Vec2(10, 10));
        setColor(Color4::BLACK);
        setTileSize(Size(5, 5));
    }

    /**
     * Creates the model state.
     *
     * @param position      The bottom left corner of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapModel(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize)
        : position(_position), dimensions(_dimensions), color(_color),
          tileSize(_tileSize) {
        setPosition(position);
        setDimensions(dimensions);
        setColor(color);
        setTileSize(tileSize);
    }

#pragma mark Setters
  public:
    /**
     *  Sets the bottom left corner of the tilemap
     *
     *  @param position Bottom left corner of the tilemap
     */
    void setPosition(Vec2 position) { _position = position; }

    /**
     *  Sets the dimensions of the tilemap.
     *
     *  @param dimensions    The number of columns and rows in the tilemap
     */
    void setDimensions(Vec2 dimensions) { _dimensions = dimensions; }

    /**
     *  Sets the color of the tilemap.
     *
     *  @param color The color of the tilemap
     */
    void setColor(Color4 color) { _color = color; }

    /**
     *  Sets the size of all tiles in the tilemap.
     *
     *  @param tileSize  The width and height of a tile
     */
    void setTileSize(Size tileSize) { _tileSize = tileSize; }

    Vec2 getDimensions() { return _dimensions; }

    Vec2 getTileSize() { return _tileSize; }
};

#endif /* _TILEMAP_MODEL_H__ */
