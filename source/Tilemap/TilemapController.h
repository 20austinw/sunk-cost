//
//  TilemapController.h
//  Sunk Cost
//
//  This module provides the TilemapController class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILEMAP_CONTROLLER_H__
#define _TILEMAP_CONTROLLER_H__

#include "../Tile/TileController.h"
#include "TilemapModel.h"
#include "TilemapView.h"
#include <memory>

/**
 * A class communicating between the model and the view. It controls
 * the entire tile map.
 */
class TilemapController {

#pragma mark Internal References
  private:
    /** Model reference */
    std::unique_ptr<TilemapModel> _model;
    /** View reference */
    std::unique_ptr<TilemapView> _view;

    /** doors view(place holder might change later)*/
    std::vector<std::shared_ptr<scene2::PolygonNode>> _doors;

#pragma mark External References
  private:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<TileController> Tile;
    typedef std::vector<std::vector<Tile>> Tilemap;
    Tilemap _tilemap;

#pragma mark Main Methods
  public:
    /** Creates the default model, view and tilemap vector. */
    TilemapController();

    /**
     * Creates the model, view and tilemap vector.
     *
     * @param position      The center position of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapController(Vec2 position, Vec2 dimensions, Color4 color,
                      Size tileSize);

#pragma mark Model Methods
  public:
    void addDoor(int col, int row, const std::shared_ptr<Texture>& texture);

    /**
     * Updates the model and view with the position of this tilemap.
     *
     * @param position The center of the tilemap
     */
    void updatePosition(Vec2 position);

    /**
     * Updates the model and view with the dimensions of the tilemap.
     *
     * Note this function will do nothing if any of the dimensions provided
     * are negative.
     *
     * @param dimensions   The number of columns and rows in the tilemap
     */
    void updateDimensions(Vec2 dimensions);

    /**
     * Updates the model and view with the color of the tilemap.
     *
     * @param color    The color of the tilemap
     */
    void updateColor(Color4 color);

    /**
     * Updates the size of all tiles in the tilemap.
     *
     * Note this function will do nothing if any of the sizes provided
     * are negative.
     *
     * @param tileSize  The width and height of a tile
     */
    void updateTileSize(Size tileSize);

#pragma mark View Methods
  public:
    /**
     * Adds the TilemapView as a child to the given scene.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);
    
    void addChildToNode(std::shared_ptr<cugl::scene2::PolygonNode>& node);

    void addDoorTo(const std::shared_ptr<cugl::Scene2>& scene);

    /**
     * Removes the TilemapView child from the given scene.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);

    void removeDoorFrom(const std::shared_ptr<cugl::Scene2>& scene);

#pragma mark Controller Methods
  public:
    /**
     * Adds a tile to (`col`, `row`) in the tilemap.
     *
     * The position is computed with the origin at the bottom right of
     * the tilemap.
     *
     * Precondition: The columns and rows must be at least 0 and at most
     * their corresponding `tileDimensions` - 1.
     *
     * @param col   The column to place the tile starting from left to right
     * @param row   The row to place the tile starting from bottom to top
     * @param color The color of the tile.
     */
    void addTile(int col, int row, Color4 color, bool traversable,
                 const std::shared_ptr<Texture>& texture);

    /**
     * Returns whether the tile at the given position is traversable.
     *
     * @param mapPos the non-grid position on the map
     * Map position is the position in pixels with origin at bottom left of the
     * map So, bottom left of the map is (0,0).
     *
     * @returns whether the tile at the given map position is traversable
     */
    bool isTileTraversable(Vec2 mapPos) {
        Vec2 gridPos(mapPosToGridPos(mapPos));
        if(gridPos.x>=0 && gridPos.y>=0 && gridPos.x < _model->getDimensions().x && gridPos.y < _model->getDimensions().y){
            return _tilemap[gridPos.y][gridPos.x]->isTraversable();
        }
        return false;
    }

    /**
     * Clears the tilemap of all tiles.
     *
     * This function is implemented for you to ensure that templates are
     * removed in an efficient manner. Here we remake the entire view rather
     * than removing tiles manually. That is because removing a child
     * from the scenegraph requires changing indices which makes this an
     * O(n^2) operation. This is notably slow when working with the large
     * procedural generation templates.
     */
    void clearMap();

#pragma mark Helpers
    /**
     * Converts position from a center position to a bottom left position
     *
     * @param position Center of the tilemap
     * @param size     Size of the tilemap
     *
     * @returns The bottom left of the tilemap
     */
    Vec2 centerToBottomLeftPosition(Vec2 position, Size size) {
        return position - size / 2;
    }

    /**
     * Converts position from a bottom left position to a center position
     *
     * @param position Bottom left of the tilemap
     * @param size     Size of the tilemap
     *
     * @returns The center of the tilemap
     */
    Vec2 bottomLeftToCenterPosition(Vec2 position, Size size) {
        return position + size / 2;
    }

    /**
     * Initializes the tilemap with empty tiles to match the current dimensions.
     *
     * This function is implemented for you because it can be particularly
     * tricky if you don't understand how to work with unique pointers. It
     * is difficult to initialize a 2D vector of unique pointers since when
     * adding rows to the vector, the compiler infers you're trying to copy
     * this into the vector. Doing this will lead to an error about a `call
     * to an implicitly deleted copy-constructor`. The copy constructor for
     * unique pointers is deleted because they're unique! To instead directly
     * move these pointers without copying them, use `std::move`.
     */
    void initializeTilemap();

    /**
     * Converts the map coordinate to grid coordinate.
     *
     * Precondition: mapPos must be a valid position
     *
     * @returns Grid position of the given map position or (-1,-1) if the given
     * position is invalid.
     */
    Vec2 mapPosToGridPos(Vec2 mapPos) {
        Vec2 gridPos((int)mapPos.x / _model->tileSize.width,
                     (int)mapPos.y / _model->tileSize.height);
        return gridPos;
    };
    
    Size getDimensions();
    
    Size getTileSize();
};

#endif /* _TILEMAP_CONTROLLER_H__ */
