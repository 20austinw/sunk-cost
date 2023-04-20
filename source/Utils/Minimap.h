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
    std::shared_ptr<LevelModel> _level;
    float _minHeight;
    float _maxHeight;
    float _scale;
    float _delta;
    float _minScale;
    float _maxScale;
    bool _inTransition;
    bool _minimized;
    Vec2 clickedPos;
    
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

    Minimap(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<Scene2>& scene, const std::shared_ptr<LevelModel> level) {
        _level = level;
        _minHeight = 300;
        _maxHeight = scene->getSize().height-100;
        _inTransition = false;
        _assets = assets;
        _scene = scene;
        _node = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("minimap"));
        _minScale = _minHeight/_node->getSize().height;
        _maxScale = _maxHeight/_node->getSize().height;
        _scale = _minScale;
        _delta = (_maxScale-_minScale)/15;
        _minimized = true;
        _node->setScale(_scale/getZoom());
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setVisible(true);
        Vec2 pos = scene->getCamera()->screenToWorldCoords(Vec2(scene->getSize().width-_node->getSize().width*getZoom()-20, _node->getSize().height*getZoom()+20));
        _node->setPosition(pos);
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
        if(_inTransition) {
            _scale += _delta;
            if(_scale >= _maxScale) {
                _inTransition = false;
                _minimized = false;
                _delta *= -1;
            }else if(_scale <= _minScale) {
                _inTransition = false;
                _minimized = true;
                _delta *= -1;
            }
        }
        _node->setScale(_scale/getZoom());
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(Vec2(_scene->getSize().width-_node->getSize().width*getZoom()-20, _node->getSize().height*getZoom()+20));
        _node->setPosition(pos);
    }
    
    bool isClicked(Vec2 position) {
        Vec2 worldPos = _scene->getCamera()->screenToWorldCoords(position);
        // Check if contained in the minimap
        Size size = _node->getSize();
        if(worldPos.x < _node->getPosition().x || worldPos.x > _node->getPosition().x+size.width) {
            if(!_minimized) {
                _inTransition = true;
            }
            return false;
        }
        if(worldPos.y < _node->getPosition().y || worldPos.y > _node->getPosition().y+size.height) {
            if(!_minimized) {
                _inTransition = true;
            }
            return false;
        }
        if(!_minimized && !_inTransition) {
            // Translate screen position to map position
            float xScale = (worldPos.x-_node->getPosition().x)/size.width;
            float yScale = (worldPos.y-_node->getPosition().y)/size.width;
            CULog("Clicked!");
            clickedPos = Vec2(xScale*_level->getDimensions().width, yScale*_level->getDimensions().height);
            return true;
        }
        if(_minimized && !_inTransition) {
            _inTransition = true;
            return false;
        }
        return false;
    }
    
#pragma mark Getter Methods
public:
    Vec2 getMapPosition() {
        CULog("Cliked on map: %f, %f", clickedPos.x, clickedPos.y);
        return clickedPos;
    }
};

#endif /* Minimap_h */
