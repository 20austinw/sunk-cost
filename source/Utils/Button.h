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
    //    std::shared_ptr<scene2::PolygonNode> _node;
    std::shared_ptr<scene2::SpriteNode> _node;

    std::shared_ptr<Scene2> _scene;
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<InputController> _inputController;
    std::shared_ptr<PortraitSetController> _portraits;

    float _buttonSize;
    float _scale;

    Vec2 _defaultPosition;
    Vec2 _position;

    bool _active;
    bool _isDragged;

    bool _selectionPhase;
    int cameraIdx = -1;

    int _cooldown;

    float COOLDOWN = 180;

#pragma mark Main Functions
  public:
    Button(std::shared_ptr<Texture> texture,
           const std::shared_ptr<Scene2>& scene, bool& selectionPhase,
           const std::shared_ptr<PortraitSetController>& portraits)
        : _texture(texture), _scene(scene), _selectionPhase(selectionPhase),
          _portraits(portraits) {
        _buttonSize = 350;
        _scale =
            _buttonSize / scene2::SpriteNode::allocWithSheet(texture, 2, 8, 16)
                              ->getSize()
                              .width;
        _node = scene2::SpriteNode::allocWithSheet(texture, 2, 8, 16);
        _node->setFrame(0);
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setScale(_scale / getZoom());
        _inputController = InputController::getInstance();
        _isDragged = false;
        _cooldown = 0;
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

    void addChildToNode(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->addChild(_node);
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

    void setCooldown(int i) { _cooldown = i; }

    void updateFrame() {
        if (!canSwitch()) {
            _cooldown--;
        }
        if (canSwitch()) {
            _node->setFrame(15);
            return;
        }
        float step = COOLDOWN / 14;
        int frame = _cooldown / step;
        _node->setFrame(15 - frame);
    }

    bool canSwitch() { return _cooldown <= 0; }

    bool update() {
        updateFrame();
        _scene->getCamera()->update();
        _node->setScale(_scale / getZoom());
        _node->setPosition(_scene->getCamera()->screenToWorldCoords(
            _position - Vec2(_buttonSize, -_buttonSize) / 2));
        if (_inputController->isTouchDown() && canSwitch()) {
            if (isClicked(_inputController->getTouchPos())) {
                Vec2 worldPos =
                    _scene->getCamera()->screenToWorldCoords(_position);
                cameraIdx = _portraits->getNearest(worldPos);
                //                CULog("%i", cameraIdx);
            }
        } else {
            reset();
        }
        return _selectionPhase;
    }

    void setButtonFrame(int frame) { _node->setFrame(frame); }

    float getMaxCool() { return COOLDOWN; }

    int getCameraIndex() { return cameraIdx; }

    void setCameraIndex(int i) { cameraIdx = i; }

    bool isClicked(Vec2 position) {
        if (!_active) {
            return _selectionPhase;
        }
        if (!_inputController->isTouchDown()) {
            return _selectionPhase;
        }
        // Check if position on button
        if (position.x < _position.x - _buttonSize / 2 ||
            position.x > _position.x + _buttonSize / 2) {
            return _selectionPhase;
        }
        if (position.y > _position.y + _buttonSize / 2 ||
            position.y < _position.y - _buttonSize / 2) {
            return _selectionPhase;
        }

        if (!_selectionPhase) {
            _selectionPhase = true;
            return _selectionPhase;
        }
        // Currently being dragged
        if (_isDragged) {
            if (_inputController->isTouchDown()) {
                _position = position;
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

    Vec2 getPosition() { return _position; }

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
            position.x > _position.x + _buttonSize) {
            return false;
        }
        if (position.y > _position.y ||
            position.y < _position.y - _buttonSize) {
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
