//
//  GameController.cpp
//  Sunk Cost
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors of Referenced File: Walker White and Gonzalo Gonzalez
//  Version: 2/22/23
//
// This is in the same directory
#include "HGameController.h"
#include "LevelConstants.h"
#include <cstdlib>
using namespace std;

#pragma mark Main Methods
HGameController::HGameController() {
    _hunter = HunterController();
    // Initialize SpiritController
    _spirit = SpiritController();
    _trap = TrapController();
    auto inputController = InputController::getInstance();
    CULog("tsts in controller");
    inputController->initListeners();
}

/**
 * Creates the game controller.
 *
 * This constructor will procedurally generate a tilemap immediately
 * on creation.
 *
 * @param displaySize   The display size of the game window
 * @param randoms        Reference to the random number generator
 */
HGameController::HGameController(
    const Size displaySize, const std::shared_ptr<cugl::AssetManager>& assets)
    : _scene(cugl::Scene2::alloc(displaySize)), _assets(assets) {
    /// Initialize the tilemap and add it to the scene
    //    SCENE_WIDTH = 1024;
    //    SCENE_HEIGHT = 576;
    _count = 0;
    _dimen = Application::get()->getDisplaySize();
    //    _offset = Vec3((_dimen.width)/2.0f,(_dimen.height)/2.0f,50);
    _offset = Vec3(0, 0, 50);
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);

    _hunter = HunterController(assets, displaySize);
    _trap = TrapController(assets, displaySize);

    // Initialize SpiritController
    _spirit = SpiritController();

    _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }
    //    auto inputController = InputController::getInstance();
    //    CULog("tsts");
    //    inputController->initListeners();
    initCamera();
}

#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void HGameController::reset() { CULog("reset"); }

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void HGameController::update(float dt) {
    if (!_levelLoaded) {
        checkLevelLoaded();
    }

    _timer = _timer - 1;
    _timerLabel->setText(std::to_string(int(_timer / 6000)) + ":" +
                         std::to_string(int(_timer / 100) % 60));
    _timerLabel->setPosition(_scene->getCamera()->getPosition() - Vec2(0, 300));
    _timerLabel->setColor(cugl::Color4f::WHITE);

    auto inputController = InputController::getInstance();
    inputController->readInput();
    inputController->update(dt);
    if (inputController->didPressReset()) {
        reset();
    }
    if (inputController->isKeyPressed(KeyCode::NUM_1)) {
        CULog("NUM_1");
    }
    if (inputController->isKeyPressed(KeyCode::NUM_2)) {
        CULog("NUM_2");
    }
    if (inputController->isKeyPressed(KeyCode::NUM_3)) {
        CULog("NUM_3");
    }
    if (inputController->isKeyPressed(KeyCode::NUM_4)) {
        CULog("NUM_4");
    }

    // Will crash the program because the constructor doesn't set up the
    // model/view yet (delete this comment later)

    //    _hunter.update();
    //    _spirit.update();

    std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
    int posx;
    int posxup;
    int posyup;
    int posy;

    int midx;
    int midy;

    Vec3 currPos = (_hunter.getPosition());
    posx = (int)(currPos.x) / _tileWidth;
    posy = (int)((currPos.y)) / _tileHeight;

    posxup = (int)(currPos.x + 40) / _tileWidth;
    posyup = (int)((currPos.y + 40)) / _tileHeight;

    midx = (int)(currPos.x + 20) / _tileWidth;
    midy = (int)((currPos.y + 20)) / _tileHeight;

    int forward = inputController->getForward();
    int rightward = inputController->getRight();

    _count++;
    if (_count == 6) {
        _hunter.setViewFrame((int)forward, (int)rightward);
        _count = 0;
    }

    std::string left = tiles[midy][posx];
    std::string up = tiles[posyup][midx];
    std::string bottom = tiles[posy][midx];
    std::string right = tiles[midy][posxup];

    if (left == "black") {
        if (rightward == -1) {
            rightward = 0;
        }
    }
    if (right == "black") {
        if (rightward == 1) {
            rightward = 0;
        }
    }
    if (up == "black") {
        if (forward == 1) {
            forward = 0;
        }
    }
    if (bottom == "black") {
        if (forward == -1) {
            forward = 0;
        }
    }
    bool age = _trap.update(); // false means trap active
    if (!_trap.getTrigger()) {
        _hunter.move(forward, rightward);
    }
    if (abs(_trap.getPosition().x - _hunter.getPosition().x) <= 80 &&
        abs(_trap.getPosition().y - _hunter.getPosition().y) <= 80 && !age) {
        _trap.setTrigger(true);
    }
    if (_trap.getTrigger() && _count == 5) {
        _trap.setViewFrame();
    }

    updateCamera(dt);

    // TODO: update direction index for portraits on spirit control
    //    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int index#>)
}

/**
 * Draws all this scene to the given SpriteBatch.
 *
 * The default implementation of this method simply draws the scene graph
 * to the sprite batch.  By overriding it, you can do custom drawing
 * in its place.
 *
 * @param batch     The SpriteBatch to draw with.
 */
void HGameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}

void HGameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }

    // Check to see if new level loaded yet
    if (!_levelLoaded && _assets->complete()) {
        _level = nullptr;

        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_TWO_KEY);
        _level->setAssets(_assets);

        // Initialize SpiritController
        _spirit = SpiritController();

        _tileHeight = 256;
        _tileWidth = 256;

        // TODO: implement direction and direction limits
        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(_tileWidth, _tileHeight));

        _map =
            scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("map"));
        _map->setPolygon(Rect(0, 0, 4608, 4608));
        //    _map = scene2::PolygonNode::allocWithPoly(Rect(0, 0, 9216, 9216));
        //    _map ->setTexture(_assets->get<Texture>("map"));
        _scene->addChild(_map);

        for (int i = 0; i < tiles.size() * tiles[0].size(); ++i) {
            int c = i % tiles[0].size();
            int r = i / tiles[0].size();

            if (tiles[r][c] == "black") {
                _tilemap->addTile(c, r, Color4::BLACK, false,
                                  _assets->get<Texture>("black"));
            } else if (tiles[r][c] == "green") {
                _tilemap->addTile(c, r, Color4::GREEN, true,
                                  _assets->get<Texture>("green"));
            } else if (tiles[r][c] == "door") {
                _tilemap->addDoor(c, r, _assets->get<Texture>("fulldoor"));
            }

            //            if (tiles[r][c] == "red") {
            //                _tilemap->addTile(c, r, Color4::RED, true,
            //                _assets->get<Texture>("red"));
            //
            //            }
            //            else if (tiles[r][c] == "blue") {
            //                _tilemap->addTile(c, r, Color4::BLUE, true,
            //                _assets->get<Texture>("blue"));
            //            }
        }

        // Initialize HunterController
        _hunter = HunterController(_assets, _scene->getSize());
        _hunter.addChildTo(_scene);
        _trap = TrapController(_assets, _scene->getSize());
        _trap.addChildTo(_scene);

        _tilemap->addDoorTo(_scene);

        _filterTexture = _assets->get<Texture>("filter");
        _filter = scene2::PolygonNode::allocWithTexture(_filterTexture);
        _filter->setPosition(_scene->getCamera()->getPosition());

        _filter->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _filter->setPolygon(Rect(Vec2::ZERO, Vec2(1280, 720)));
        _scene->addChild(_filter);

        _timer = 12000;
        _timerLabel = cugl::scene2::Label::allocWithText(
            Vec2(200, 200), "2:00", _assets->get<Font>("pixel32"));
        _scene->addChild(_timerLabel);

        _levelLoaded = true;
    }
}

/**
 *
 */
void HGameController::initCamera() {

    Vec3 curr = _scene->getCamera()->getPosition();
    Vec3 next =
        _offset + (Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1));
    _scene->getCamera()->translate(next - curr);

    //    _scene->getCamera()->lookAt(Vec3(_hunter.getPosition().x,
    //    _hunter.getPosition().y, 1));
    _scene->getCamera()->update();
}

/**
 * Updates camera based on the position of the controlled player
 */
void HGameController::updateCamera(float timestep) {

    Vec2 curr = _scene->getCamera()->getPosition();
    _filter->setPosition(_scene->getCamera()->getPosition());
    _filter->setAnchor(Vec2::ANCHOR_CENTER);
    Vec2 next =
        _offset + ((Vec3(_hunter.getPosition().x, _hunter.getPosition().y, 1)));
    _scene->getCamera()->translate((next - curr) * timestep);

    _timerLabel->setPosition(_scene->getCamera()->getPosition() - Vec2(0, 300));

    _filter->setPosition(_scene->getCamera()->getPosition());
    _scene->getCamera()->update();
}

void HGameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
}
