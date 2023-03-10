//
//  HunterView.h
//  Sunk Cost
//
//  Created by Chloe Chu
//  Version: 2/22/23.
//

#ifndef _HUNTER_VIEW_H
#define _HUNTER_VIEW_H
#include <cugl/cugl.h>

using namespace cugl;

class HunterView {
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

    HunterView(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position, Size size){
//        _node = scene2::PolygonNode::allocWithPoly(Rect(Vec2::ZERO, size));
//        _node = scene2::PolygonNode::alloc();
        _node = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("hunter"));
//        const Rect a(Rect(Vec2::ZERO, size));
//        _node->initWithPoly(a);

        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setPosition(position);
        _node->setPolygon(Rect(Vec2::ZERO, size));

    };
    
    
    /** Deletes this HunterView */
    ~HunterView() {
        _node->removeFromParent();
    }
    
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        // TODO: Implement me
        scene->addChild(_node);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_node);
    }
#pragma mark Getters
public:
    /** Returns the TileView */
    const std::shared_ptr<scene2::PolygonNode> getNode() const {
        // TODO: Implement me
        return _node;
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
    

    
    /**
     * Sets the texture for this ship.
     *
     * The texture should be formated as a sprite sheet, and the size and
     * layout of the sprite sheet should already be specified in the
     * initializing JSON. If so, this method will construct a sprite sheet
     * from this texture. Otherwise, the texture will be ignored.
     *
     * @param texture   The texture for the sprite sheet
     */
//    void setTexture(const std::shared_ptr<cugl::Texture>& texture) {
//        if (_framecols > 0) {
//            int rows = _framesize/_framecols;
//            if (_framesize % _framecols != 0) {
//                rows++;
//            }
//            _sprite = SpriteSheet::alloc(texture, rows, _framecols, _framesize);
//            _sprite->setFrame(_frameflat);
//            _radius = std::max(_sprite->getFrameSize().width, _sprite->getFrameSize().height)/2;
//            _sprite->setOrigin(_sprite->getFrameSize()/2);
//        }
//    }
};


#endif /* _HUNTER_VIEW_H__ */
