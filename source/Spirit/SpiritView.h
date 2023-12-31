//
//  Spirit.h
//  Sunk Cost
//
//  Created by Austin Wu
//  Version: 3/5/23.
//

#ifndef _SPIRIT_VIEW_H
#define _SPIRIT_VIEW_H
#include <cugl/cugl.h>

using namespace cugl;

class SpiritView {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _node;

    std::vector<std::shared_ptr<scene2::PolygonNode>> _locks;

    std::vector<std::shared_ptr<scene2::PolygonNode>> _trapButtons;

    std::vector<std::shared_ptr<scene2::PolygonNode>> _lockExtra;

    std::vector<std::shared_ptr<scene2::PolygonNode>> _trapExtra;

    std::shared_ptr<scene2::SpriteNode> _killButton;

    std::shared_ptr<cugl::Texture> _lockAsset;

    std::shared_ptr<cugl::Texture> _trapAsset;

    std::shared_ptr<cugl::Texture> _killAsset;

    std::shared_ptr<cugl::Texture> _oneBtnAsset;

    std::shared_ptr<cugl::Texture> _twoBtnAsset;

    std::shared_ptr<cugl::Texture> _threeBtnAsset;

    Size _lockSize;

    Size _trapSize;

    Size _killSize;

    std::shared_ptr<cugl::Scene2> _scene;

    float _buttonSize = 350;

    float _trapScaleFactor;
    float _lockScaleFactor;
    float _killScaleFactor;

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

    void setVisible(bool b) {
        for (auto& lock : _locks) {
            lock->setVisible(b);
        }

        for (auto& trapButton : _trapButtons) {
            trapButton->setVisible(b);
        }

        for (auto& lockExtra : _lockExtra) {
            lockExtra->setVisible(b);
        }

        for (auto& trapExtra : _trapExtra) {
            trapExtra->setVisible(b);
        }
    }

    SpiritView(int locks, int traps,
               const std::shared_ptr<cugl::AssetManager>& assets,
               std::shared_ptr<cugl::Scene2>& scene) {
        _scene = scene;
        _oneBtnAsset = assets->get<Texture>("extra");
        _twoBtnAsset = assets->get<Texture>("extra_1");
        _threeBtnAsset = assets->get<Texture>("extra_2");

        _lockAsset = assets->get<Texture>("lock_button");
        _trapAsset = assets->get<Texture>("trap_button");
        _killAsset = assets->get<Texture>("kill_button");
        _lockScaleFactor = _buttonSize / _lockAsset->getSize().width;
        _trapScaleFactor = _buttonSize / _trapAsset->getSize().width;
        _killScaleFactor = _buttonSize / scene2::SpriteNode::allocWithSheet(
                                             _killAsset, 4, 4, 13)
                                             ->getWidth();
        for (int i = 0; i < locks; i++) {
            _locks.emplace_back(
                                scene2::PolygonNode::allocWithTexture(_lockAsset));
            _locks[0]->setScale(_lockScaleFactor / getZoom());
            addExtra(_lockExtra, i + 1);
        }

        for (int i = 0; i < traps; i++) {
            _trapButtons.emplace_back(
                scene2::PolygonNode::allocWithTexture(_trapAsset));
            _trapButtons.at(i)->setScale(_trapScaleFactor / getZoom());
            addExtra(_trapExtra, i + 1);
        }

        _killButton = scene2::SpriteNode::allocWithSheet(_killAsset, 4, 4, 13);
        _killButton->setFrame(0);
        _killButton->setScale(_killScaleFactor / getZoom());

        _trapSize = _trapButtons.at(0)->getSize();
        _lockSize = _locks.at(0)->getSize();
        _killSize = _killButton->getSize();
    }

    //    /** Deletes this HunterView */
    //    ~SpiritView() { _node->removeFromParent(); }

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<Scene2>& scene) {
        scene->addChild(_node);
    }

    void addLastLockExtraTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        addExtra(_lockExtra, _locks.size());
        node->addChild(_lockExtra.at(_lockExtra.size() - 1));
    }

    void addLastTrapExtraTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        addExtra(_trapExtra, _trapButtons.size());
        node->addChild(_trapExtra.at(_trapExtra.size() - 1));
    }

    void addKillButtonTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->addChild(_killButton);
    }

    void
    removeLastLockExtraTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        removeExtra(node, _lockExtra);
    }

    void
    removeLastTrapExtraTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        removeExtra(node, _trapExtra);
    }

    void addLocksTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        for (int i = 0; i < _locks.size(); i++) {
            node->addChild(_locks.at(i));
        }
        for (int i = 0; i < _lockExtra.size(); i++) {
            node->addChild(_lockExtra.at(i));
        }
    }

    void addTrapButtonsTo(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        for (int i = 0; i < _trapButtons.size(); i++) {
            node->addChild(_trapButtons.at(i));
        }
        for (int i = 0; i < _trapExtra.size(); i++) {
            node->addChild(_trapExtra.at(i));
        }
    }

    void updateUnusedLocksPos(bool selection) {
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            _scene->getSize() - Vec2(_buttonSize, _buttonSize) / 2);

        for (int i = 0; i < _locks.size(); i++) {
            _locks.at(i)->setVisible(true);
            if (selection)
                _locks.at(i)->setVisible(false);
            _locks.at(i)->setPosition(pos);
            _locks[i]->setScale(_lockScaleFactor / getZoom());
        }
        for (int i = 0; i < _lockExtra.size(); i++) {
            _lockExtra.at(i)->setVisible(true);
            if (selection)
                _lockExtra.at(i)->setVisible(false);
            _lockExtra.at(i)->setPosition(pos);
            _lockExtra[i]->setScale(_trapScaleFactor / getZoom());
        }
    }

    void updateUnusedTrapsPos(bool selection) {
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            _scene->getSize() - Vec2(_buttonSize, _buttonSize) / 2 -
            Vec2(0, _buttonSize));

        for (int i = 0; i < _trapButtons.size(); i++) {
            _trapButtons.at(i)->setVisible(true);
            if (selection)
                _trapButtons.at(i)->setVisible(false);
            _trapButtons.at(i)->setPosition(pos);
            _trapButtons[i]->setScale(_trapScaleFactor / getZoom());
        }
        for (int i = 0; i < _trapExtra.size(); i++) {
            _trapExtra.at(i)->setVisible(true);
            if (selection)
                _trapExtra.at(i)->setVisible(false);
            _trapExtra.at(i)->setPosition(pos);
            _trapExtra[i]->setScale(_trapScaleFactor / getZoom());
        }
    }

    void updateUnusedKillPos(bool selection) {
        Vec2 pos = _scene->getCamera()->screenToWorldCoords(
            _scene->getSize() - Vec2(_buttonSize, _buttonSize) / 2 -
            2 * Vec2(0, _buttonSize));

        _killButton->setVisible(true);
        if (selection) {
            _killButton->setVisible(false);
        }

        _killButton->setPosition(pos);
        _killButton->setScale(_killScaleFactor / getZoom());
    }

    Size getKillSize() { return _killSize; }

    int getKillFrame() { return _killButton->getFrame(); }

    void updateKillInProgress(Vec2 touchPos) {
        _killButton->setPosition(touchPos);
        _killButton->setScale(_killScaleFactor / getZoom());
    }

    void updateLockInProgress(Vec2 touchPos) {
        _locks.at(_locks.size() - 1)->setPosition(touchPos);
        _locks[_locks.size() - 1]->setScale(_lockScaleFactor / getZoom());
    }

    void updateTrapInProgress(Vec2 touchPos) {
        _trapButtons.at(_trapButtons.size() - 1)->setPosition(touchPos);
        _trapButtons[_trapButtons.size() - 1]->setScale(_lockScaleFactor /
                                                        getZoom());
    }

    void removeKillFrom(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->removeChild(_killButton);
    }

    Vec2 getKillBtnPos() { return _killButton->getPosition(); }

    void setKillFrame(int frame) { _killButton->setFrame(frame); }

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<Scene2>& scene) {
        scene->removeChild(_node);
    }

    void removeLocksFrom(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        for (int i = 0; i < _locks.size(); i++) {
            node->removeChild(_locks.at(i));
        }
        for (int i = 0; i < _lockExtra.size(); i++) {
            node->removeChild(_lockExtra.at(i));
        }
    }

    void removeTrapsFrom(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        for (int i = 0; i < _trapButtons.size(); i++) {
            node->removeChild(_trapButtons.at(i));
        }
        for (int i = 0; i < _trapExtra.size(); i++) {
            node->removeChild(_trapExtra.at(i));
        }
    }

    void removeLastLock(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->removeChild(_locks.at(_locks.size() - 1));
        _locks.pop_back();
    }

    void
    removeLastTrapButton(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        node->removeChild(_trapButtons.at(_trapButtons.size() - 1));
        _trapButtons.pop_back();
    }

    Size getLockSize() { return _lockSize; }

    Size getTrapSize() { return _trapSize; }

    Vec2 getLastLockPos() {
        return _locks.at(_locks.size() - 1)->getPosition();
    }

    Vec2 getLastTrapBtnPos() {
        return _trapButtons.at(_trapButtons.size() - 1)->getPosition();
    }

    void addNewTrap(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        _trapButtons.emplace_back(
            scene2::PolygonNode::allocWithTexture(_trapAsset));
        _trapButtons[_trapButtons.size() - 1]->setScale(_trapScaleFactor);
        addExtra(_trapExtra, _trapButtons.size());
        node->addChild(_trapButtons.at(_trapButtons.size() - 1));
        node->addChild(_trapExtra.at(_trapExtra.size() - 1));
    }

    void addNewLock(std::shared_ptr<cugl::scene2::PolygonNode>& node) {
        _locks.emplace_back(
                            scene2::PolygonNode::allocWithTexture(_lockAsset));
        _locks.at(_locks.size() - 1)->setScale(_lockScaleFactor);
        addExtra(_lockExtra, _locks.size());
        node->addChild(_locks.at(_locks.size() - 1));
        node->addChild(_lockExtra.at(_lockExtra.size() - 1));
    }

    void addExtra(std::vector<std::shared_ptr<scene2::PolygonNode>>& extra,
                  int size) {
        if (size < 1 || size > 3) {
            return;
        }
        if (size == 1) {
            extra.emplace_back(
                scene2::PolygonNode::allocWithTexture(_oneBtnAsset));
        } else if (size == 2) {
            extra.emplace_back(
                scene2::PolygonNode::allocWithTexture(_twoBtnAsset));
        } else if (size == 3) {
            extra.emplace_back(
                scene2::PolygonNode::allocWithTexture(_threeBtnAsset));
        }
        extra.at(extra.size() - 1)->setScale(_lockScaleFactor);
    }

    void removeExtra(std::shared_ptr<cugl::scene2::PolygonNode>& node,
                     std::vector<std::shared_ptr<scene2::PolygonNode>>& extra) {
        node->removeChild(extra.at(extra.size() - 1));
        extra.pop_back();
    }

#pragma mark Setters
};

#endif /* _SPIRIT_VIEW_H__ */
