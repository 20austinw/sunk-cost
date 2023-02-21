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
    // TODO: Implement me
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
    // TODO: Implement me
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
    // TODO: Implement me
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
    // TODO: Implement me
    Vec2 pos(_model->tileSize.width*(col), _model->tileSize.height*row);
    _tilemap[row][col] = std::move(std::make_unique<TileController>(pos,_model->tileSize,color));
    _tilemap[row][col]->addChildTo(_view->getNode());
}

/**
 * Inverts the color of the tilemap and it's tiles.
 *
 * Examples:
 *      Inverting white (255, 255, 255, 0) gives black (0, 0, 0, 0)
 *      Inverting red (255, 0, 0, 0) gives cyan (0, 255, 255, 0)
 *      Inverting light purple (150, 100, 200, 0) gives a dull green
 *          (105, 155, 55, 0)
 */
void TilemapController::invertColor() {
    // TODO: Implement me
    Color4 invertedColor(Color4::WHITE - _model->color);
    invertedColor.a = _model->color.a; 
    _model->setColor(invertedColor);
    _view->setColor(invertedColor);
    for (int i = 0; i < _model->dimensions.y; i++) {
      for (int j = 0; j < _model->dimensions.x; j++) {
        if (_tilemap[i][j]) {
          _tilemap[i][j]->invertColor();
        }
      }
    }
}

/**
 * Modifies the current number of columns and rows by
 *
 * The values are modified by `colIncrement` and `rowIncrement`,
 *  respectively. The values can be negative.
 *
 * @param colIncrement The number of columns to increment
 * @param rowIncrement The number of rows to increment
 */
void TilemapController::modifyDimensions(int colIncrement, int rowIncrement) {
    // TODO: Implement me
    Vec2 dim(colIncrement + _model->dimensions.x, rowIncrement + _model->dimensions.y);
    updateDimensions(dim);
}

/**
 *  Modifies the current width and height by `xFactor` and `yFactor`.
 *
 *  @param xFactor The factor to multiply the current width by
 *  @param yFactor The factor to multiply the current height by
 */
void TilemapController::modifyTileSize(float xFactor, float yFactor) {
    // TODO: Implement me
    updateTileSize(Size(xFactor,yFactor)*_model->tileSize);
}

/**
 *  Updates the model and view with the dimensions of the tilemap.
 *
 *  Note this function will do nothing if any of the dimensions provided
 *  are negative.
 *
 *  @param dimensions   The number of columns and rows in the tilemap
 */
void TilemapController::updateDimensions(Vec2 dimensions) {
  // TODO: Implement me
  if (dimensions.x < 0 || dimensions.y < 0){
    return;
  }
  Tilemap temp(std::move(_tilemap));
  Vec2 initsize(_model->dimensions);
  _model->setDimensions(dimensions);
  _view->setSize(dimensions * _model->tileSize);
  initializeTilemap();
  for (int y = 0; y < initsize.y; y++) {
    for (int x = 0; x < initsize.x; x++) {
      if (y < dimensions.y && x < dimensions.x) {
        _tilemap[y][x] = std::move(temp[y][x]);
      }
    }
  }
}

/**
 * Updates the size of all tiles in the tilemap.
 *
 * Note this function will do nothing if any of the sizes provided
 * are negative.
 *
 * @param tileSize  The width and height of a tile
 */
void TilemapController::updateTileSize(Size tileSize) {
    // TODO: Implement me
    if (tileSize.width < 0 || tileSize.height < 0){
      return;
    }   
    Vec2 center(bottomLeftToCenterPosition(_model->position, _model->tileSize * _model->dimensions));
    _model->setTileSize(tileSize);
    _view->setSize(_model->dimensions * tileSize);
    _model->setPosition(centerToBottomLeftPosition(center,tileSize * _model->dimensions));
    _view->setPosition(centerToBottomLeftPosition(center,tileSize * _model->dimensions));
    for(int i = 0; i < _model->dimensions.y; i++) {
      for (int j = 0; j < _model->dimensions.x; j++) {
        if (_tilemap[i][j]) {
          _tilemap[i][j]->updateSize(tileSize);
          _tilemap[i][j]->updatePosition(Vec2(j,i)*tileSize);
        }
      }
    }
}

#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given scene.
 *
 * @param scene The scene to add the view to
 */
void TilemapController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    scene->addChild(_view->getNode());
}

/**
 * Removes the TilemapView child from the given scene.
 *
 * @param scene The scene to remove the view from
 */
void TilemapController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
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
