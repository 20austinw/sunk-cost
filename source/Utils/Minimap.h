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
    std::shared_ptr<TilemapController> _tilemap;
    float _minHeight;
    float _maxHeight;
    float _maxWidth;
    float _scale;
    float _scaleDelta;
    float _minScale;
    float _maxScale;
    bool _inTransition;
    bool _minimized;
    float _widthMargin;
    float _heightMargin;
    Vec2 clickedPos;
    Vec2 _minPos;
    Vec2 _maxPos;
    Vec2 _posDelta;
    Vec2 _pos;

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
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }

    Minimap(const std::shared_ptr<cugl::AssetManager>& assets,
            const std::shared_ptr<Scene2>& scene,
            const std::shared_ptr<TilemapController>& tilemap)
        : _assets(assets), _scene(scene), _tilemap(tilemap) {
        _minHeight = 300;
        _heightMargin = 100;
        _widthMargin = 100;
        _maxHeight = scene->getSize().height - _heightMargin;
        _maxWidth = scene->getSize().width - _widthMargin;
        _inTransition = false;
        _node = scene2::PolygonNode::allocWithTexture(
            assets->get<Texture>("minimap"));
        _minScale = _minHeight / _node->getSize().height;
        if (_maxWidth / _maxHeight >
            _node->getSize().width / _node->getSize().height) {
            // Use height scaling
            _maxScale = _maxHeight / _node->getSize().height;
            _maxWidth = _node->getSize().width * _maxScale;
        } else {
            // Use width scaling
            _maxScale = _maxWidth / _node->getSize().width;
            _maxHeight = _node->getSize().height * _maxScale;
            //            CULog("Max dimensions: %f, %f", _maxWidth,
            //            _maxHeight); CULog("%f, %f", _scene->getSize().width,
            //            _scene->getSize().height);
        }
        _scale = _minScale;
        _scaleDelta = (_maxScale - _minScale) / 15;
        _minimized = true;
        _node->setScale(_scale / getZoom());
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setVisible(true);
        float minWidth =
            _minHeight * _node->getSize().width / _node->getSize().height;
        _minPos = Vec2(scene->getSize().width - minWidth - 20, _minHeight + 20);
        _maxPos = Vec2((scene->getSize().width - _maxWidth) / 2,
                       (_scene->getSize().height + _maxHeight) / 2);
        //        CULog("%f, %f", _minPos.x, _minPos.y);
        //        CULog("%f, %f", _maxPos.x, _maxPos.y);
        _pos = _minPos;
        _posDelta = (_maxPos - _minPos) / 15;
        _node->setPosition(_pos);
    };

    /** Deletes this HunterView */
    ~Minimap() {}

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

    void addChildToNode(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->addChild(_node);
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
        if (_inTransition) {
            _scale += _scaleDelta;
            _pos += _posDelta;
            if (_scale >= _maxScale) {
                _inTransition = false;
                _minimized = false;
                _scaleDelta *= -1;
                _posDelta *= -1;
                _scale = _maxScale;
                _pos = _maxPos;
            } else if (_scale <= _minScale) {
                _inTransition = false;
                _minimized = true;
                _scaleDelta *= -1;
                _posDelta *= -1;
                _scale = _minScale;
                _pos = _minPos;
            }
        }
        _node->setScale(_scale / getZoom());
        _node->setPosition(_scene->getCamera()->screenToWorldCoords(_pos));
    }

    bool isClicked(Vec2 position) {
        Vec2 worldPos = _scene->getCamera()->screenToWorldCoords(position);
        // Check if contained in the minimap
        Size size = _node->getSize();
        if (worldPos.x < _node->getPosition().x ||
            worldPos.x > _node->getPosition().x + size.width) {
            if (!_minimized) {
                _inTransition = true;
            }
            return false;
        }
        if (worldPos.y < _node->getPosition().y ||
            worldPos.y > _node->getPosition().y + size.height) {
            if (!_minimized) {
                _inTransition = true;
            }
            return false;
        }
        if (!_minimized && !_inTransition) {
            // Translate screen position to map position
            float xScale = (worldPos.x - _node->getPosition().x) / size.width;
            float yScale = (worldPos.y - _node->getPosition().y) / size.height;
            float mapWidth =
                _tilemap->getDimensions().width * _tilemap->getTileSize().width;
            float mapHeight = _tilemap->getDimensions().height *
                              _tilemap->getTileSize().height;
            clickedPos = Vec2(xScale * mapWidth, yScale * mapHeight);
            return true;
        }
        if (_minimized && !_inTransition) {
            _inTransition = true;
            return false;
        }
        return false;
    }

#pragma mark Getter Methods
  public:
    Vec2 getMapPosition() { return clickedPos; }
};

#endif /* Minimap_h */
