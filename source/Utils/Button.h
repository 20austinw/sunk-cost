//
//  Button.h
//  Sunk
//
//  Created by Austin Wu on 4/27/23.
//

#ifndef Button_h
#define Button_h

#include "InputController.h"
#include "PortraitSetController.h"
#include "TilemapController.h"
#include <cugl/cugl.h>

using namespace cugl;

class Button {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _node;
    std::shared_ptr<Scene2> _scene;
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<InputController> _inputController;
    std::shared_ptr<PortraitSetController> _portraits;

    float _buttonHeight;
    float _scale;

    Vec2 _defaultPosition;
    Vec2 _position;

    bool _active;
    bool _isDragged;

    bool _selectionPhase;
    int cameraIdx = -1;

#pragma mark Main Functions
  public:
    Button(std::shared_ptr<Texture> texture,
           const std::shared_ptr<Scene2>& scene, bool& selectionPhase,
           const std::shared_ptr<PortraitSetController>& portraits)
        : _texture(texture), _scene(scene), _selectionPhase(selectionPhase),
          _portraits(portraits) {
        _buttonHeight = 400;
        _scale = _buttonHeight / texture->getSize().height;
        _node = scene2::PolygonNode::allocWithTexture(texture);
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setScale(_scale / getZoom());
        _inputController = InputController::getInstance();
        _isDragged = false;
    };

    ~Button() {}

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

    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_node);
    }

#pragma mark Setter Methods
  public:
    void setDefaultPosition(Vec2 pos) {
        _defaultPosition = pos;
        _position = pos;
    }

    bool update() {
        CULog("Update called!");
        _node->setScale(_scale / getZoom());
        _node->setPosition(_scene->getCamera()->screenToWorldCoords(_position));
        if (_inputController->isTouchDown()) {
            if (isClicked(_inputController->getTouchPos())) {
                Vec2 worldPos =
                    _scene->getCamera()->screenToWorldCoords(_position);
                cameraIdx = _portraits->getNearest(worldPos);
            }
        } else {
            if (!_selectionPhase && cameraIdx != -1) {
                _portraits->setIndex(cameraIdx);
                _scene->getCamera()->setPosition(
                    _portraits->getPosition(_portraits->getIndex()));
                std::dynamic_pointer_cast<OrthographicCamera>(
                    _scene->getCamera())
                    ->setZoom(0.85);
            }
            reset();
        }
        return _selectionPhase;
    }

    bool isClicked(Vec2 position) {
        if (!_active) {
            return _selectionPhase;
        }
        if (!_inputController->isTouchDown()) {
            return _selectionPhase;
        }
        // Check if position on button
        if (position.x < _position.x ||
            position.x > _position.x + _buttonHeight) {
            return _selectionPhase;
        }
        if (position.y > _position.y ||
            position.y < _position.y - _buttonHeight) {
            return _selectionPhase;
        }

        if (!_selectionPhase) {
            _selectionPhase = true;
            return _selectionPhase;
        }

        // Currently being dragged
        if (_isDragged) {
            if (_inputController->isTouchDown()) {
                _position = position - Vec2(_texture->getSize().width / 2,
                                            -_texture->getSize().height / 2);
            }
        }
        // Currently not dragged
        else {
            if (_inputController->isTouchDown()) {
                _isDragged = true;
            }
        }
        return _selectionPhase;
    }

    void setVisible(bool b) { _node->setVisible(b); }

    void setInteractive(bool b) { _active = b; }

    void reset() {
        _position = _defaultPosition;
        _isDragged = false;
        _selectionPhase = false;
    }

#pragma mark Getter Methods
  public:
    bool getDragged() { return _isDragged; }

    bool getClicked(Vec2 position) {
        if (position.x < _position.x ||
            position.x > _position.x + _buttonHeight) {
            return false;
        }
        if (position.y > _position.y ||
            position.y < _position.y - _buttonHeight) {
            return false;
        }
        return true;
    }

#pragma mark Utils
  private:
    float getZoom() {
        return std::dynamic_pointer_cast<OrthographicCamera>(
                   _scene->getCamera())
            ->getZoom();
    }
};

#endif /* Button_h */
