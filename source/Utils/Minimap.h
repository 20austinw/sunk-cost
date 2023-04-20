//
//  Minimap.h
//  Sunk
//
//  Created by Austin Wu on 4/19/23.
//

#ifndef Minimap_h
#define Minimap_h

#include <cugl/cugl.h>

using namespace cugl;

class Minimap {
#pragma mark Internal References
private:
    std::shared_ptr<scene2::PolygonNode> _node;
    std::shared_ptr<Scene2> _scene;
    std::shared_ptr<cugl::AssetManager> _assets;
    float _minWidth = 500;
    float _scale;
    
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
    
    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())
        ->getZoom();
    }

    Minimap(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<Scene2>& scene) {
        _assets = assets;
        _scene = scene;
        _node = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("minimap"));
        _scale = _minWidth/_node->getSize().width;
//        _node->setContentSize(_node->getSize()*scale/getZoom());
        _node->setScale(_scale/getZoom());
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setVisible(true);
        Vec2 pos = scene->getCamera()->screenToWorldCoords(Vec2(scene->getSize().width-_node->getSize().width*getZoom()-20, _node->getSize().height*getZoom()+20));
        _node->setPosition(pos);
//        CULog("Minimap size: %f, %f", _node->getSize().width, _node->getSize().height);
//        CULog("Minimap position: %f, %f", pos.x, pos.y);
//        CULog("Scene size: %f, %f", _scene->getSize().width, _scene->getSize().height);
    };
    
    /** Deletes this HunterView */
    ~Minimap() {
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
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
    
#pragma mark Setter Methods
public:
    void update() {
        _node->setScale(_scale/getZoom());
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(Vec2(_scene->getSize().width-_node->getSize().width*getZoom()-20, _node->getSize().height*getZoom()+20));
        _node->setPosition(pos);
    }
};

#endif /* Minimap_h */
