//
//  TilemapController.cpp
//  Sunk Cost
//
//  This module provides the TilemapController class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#include "TilemapController.h"

using namespace MVC;

#pragma mark Main Functions
/** Creates the default model, view and tilemap vector. */
TilemapController::TilemapController() {
    _model = std::make_unique<TilemapModel>();
    _view = std::make_unique<TilemapView>(_model->position,
                                          _model->dimensions,
                                          _model->color,
                                          _model->tileSize);
    initializeTilemap();
 }

/**
 * Creates the model, view and tilemap vector.
 *
 * @param position      The center position of the tilemap
 * @param dimensions    The number of columns and rows in the tilemap
 * @param color         The background color of the tilemap
 * @param tileSize      The width and height of a tile
 */
TilemapController::TilemapController(Vec2 position, Vec2 dimensions,
                                     Color4 color, Size tileSize) {
    _model = std::make_unique<TilemapModel>(centerToBottomLeftPosition(position, tileSize*dimensions), dimensions, color, tileSize);
    _view = std::make_unique<TilemapView>(centerToBottomLeftPosition(position, tileSize*dimensions), dimensions, color, tileSize);
    initializeTilemap();
}

#pragma mark -
#pragma mark Model Methods
/**
 *  Updates the model and view with the position of this tilemap.
 *
 *  @param position The center of the tilemap
 */
void TilemapController::updatePosition(Vec2 position) {
    Vec2 newPos(centerToBottomLeftPosition(position, Size(_model->dimensions)*_model->tileSize));
    _model->setPosition(newPos);
    _view->setPosition(newPos);
}

/**
 *  Updates the model and view with the color of the tilemap.
 *
 *  @param color    The color of the tilemap
 */
void TilemapController::updateColor(Color4 color) {
    _model->setColor(color);
    _view->setColor(color);
}

#pragma mark -
#pragma mark Controller Methods
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
void TilemapController::addTile(int col, int row, Color4 color) {
    Vec2 pos(_model->tileSize.width*(col), _model->tileSize.height*row);
    _tilemap[row][col] = std::move(std::make_unique<TileController>(pos,_model->tileSize,color));
    _tilemap[row][col]->addChildTo(_view->getNode());
}

#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given scene.
 *
 * @param scene The scene to add the view to
 */
void TilemapController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
    scene->addChild(_view->getNode());
}

/**
 * Removes the TilemapView child from the given scene.
 *
 * @param scene The scene to remove the view from
 */
void TilemapController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
    scene->removeChild(_view->getNode());
}

#pragma mark -
#pragma mark Provided Methods
/**
 *  Initializes the tilemap with empty tiles to match the current dimensions.
 *
 *  This function is implemented for you because it can be particularly
 *  tricky if you don't understand how to work with unique pointers. It
 *  is difficult to initialize a 2D vector of unique pointers since when
 *  adding rows to the vector, the compiler infers you're trying to copy
 *  this into the vector. Doing this will lead to an error about a `call
 *  to an implicitly deleted copy-constructor`. The copy constructor for
 *  unique pointers is deleted because they're unique! To instead directly
 *  move these pointers without copying them, use `std::move`.
 */
void TilemapController::initializeTilemap() {
    for(int i = 0; i < _model->dimensions.y; i++) {
        std::vector<Tile> tileVec(_model->dimensions.x);
        // The compiler infers that tileVec contains unique pointers so std::move must be used to avoid copys
        _tilemap.push_back(std::move(tileVec));
    }
}

/**
 *  Clears the tilemap of all tiles.
 *
 *  This function is implemented for you to ensure that templates are
 *  removed in an efficient manner. Here we remake the entire view rather
 *  than removing tiles manually. That is because removing a child
 *  from the scenegraph requires changing indices which makes this an
 *  O(n^2) operation. This is notably slow when working with the large
 *  procedural generation templates.
 */
void TilemapController::clearMap() {
    /// It is much more efficient to remake the view than to remove tiles one by one.
    auto scene = _view->getNode()->getScene();
    scene->removeChild(_view->getNode());
    _model = std::make_unique<TilemapModel>();
    _view = std::make_unique<TilemapView>(_model->position,
                                          _model->dimensions,
                                          _model->color,
                                          _model->tileSize);
    scene->addChild(_view->getNode());
    _tilemap.clear();
    initializeTilemap();
}
