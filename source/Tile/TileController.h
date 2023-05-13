//
//  TileController.h
//  Sunk Cost
//
//  This module provides the TileController class.
//
//  Author of Referenced File: Gonzalo Gonzalez
//  Version: 2/21/23.
//
#ifndef _TILE_CONTROLLER_H__
#define _TILE_CONTROLLER_H__

// These do not need to be in angle brackets
#include "TileModel.h"
#include "TileView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class TileController {

#pragma mark Internal References
  private:
    /** Model reference */
    std::unique_ptr<TileModel> _model;
    /** View reference */
    std::unique_ptr<TileView> _view;

#pragma mark Main Methods
  public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color tint
     * @param color     The tile color
     */
    TileController(Vec2 position, Size size, Color4 color, bool traversable,
                   const std::shared_ptr<Texture>& texture, float yPos = -1) {
        _model = std::make_unique<TileModel>(position, size, color, traversable,
                                             yPos);
        _view = std::make_unique<TileView>(position, size, color, texture);
    }

#pragma mark Get Methods
  public:
    /**
     *  Returns whether the tile is traversable
     */
    bool isTraversable() { return _model->traversable; }

    Vec2 getPosition() { return _model->getPosition(); }

    int getYPos() { return _model->yPos; }
    
    bool isObstacle() {
        return _model->obstacle;
    }
    
    void setYPos(float yPos) {
        _model->setYPos(yPos);
    }
    
    void setObstacle(bool b){
        _model->setObstacle(b);
    }

#pragma mark Update Methods
  public:
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position) {
        _model->setPosition(position);
        _view->setPosition(position);
    }

    /**
     *  Updates the model and view with the size of this tile.
     *
     *  @param size  The bottom left corner of the tile
     */
    void updateSize(Size size) {
        _model->setSize(size);
        _view->setSize(size);
    }

    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param color The tile color
     */
    void updateColor(Color4 color) {
        _model->setColor(color);
        _view->setColor(color);
    }

    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param traverse The tile traversability
     */
    void updateTraversable(bool traverse) { _model->setTraversable(traverse); }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<scene2::SceneNode> node) {
        _view->addChildTo(node);
    }

    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<scene2::SceneNode> node) {
        _view->removeChildFrom(node);
    }
};

#endif /* _TILE_CONTROLLER_H__ */
