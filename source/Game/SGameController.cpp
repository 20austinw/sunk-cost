//
//  SGameController.cpp
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
#include "SGameController.hpp"
#include "LevelConstants.h"

#pragma mark Main Methods
/**
 * Creates the game controller.
 *
 * This constructor will procedurally generate a tilemap immediately
 * on creation.
 *
 * @param displaySize   The display size of the game window
 * @param randoms        Reference to the random number generator
 */
SGameController::SGameController(const Size displaySize, const std::shared_ptr<cugl::AssetManager>& assets):
_scene(cugl::Scene2::alloc(displaySize)),
_assets(assets){
    /// Initialize the tilemap and add it to the scene
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
    // Initialize PortraitSetController
    _portraits = std::make_shared<PortraitSetController>(0, displaySize);

    // Initialize HunterController
//        _hunter.updatePosition(_level->getPlayerPosition());

    // Initialize SpiritController
    _spirit = SpiritController(_portraits, _scene->getSize());
    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
        CULog("Fail!");
    }
}

#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void SGameController::reset() {
    CULog("reset");
}

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void SGameController::update(float dt) {
    if (!_levelLoaded) {
        CULog("Level not loaded!");
        checkLevelLoaded();
        
    }
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    if (inputController->didPressReset()) {
        reset();
    }
    if(inputController->isMouseClicked()) {
        CULog("%f, %f", inputController->getLastMousePos().x, inputController->getLastMousePos().y);
    }
    if(inputController->isKeyPressed(KeyCode::NUM_0)) {
        _portraits->setIndex(0);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())->setZoom(1);
    }
    if(inputController->isKeyPressed(KeyCode::NUM_1)) {
        _portraits->setIndex(1);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())->setZoom(2);
    }
    if(inputController->isKeyPressed(KeyCode::NUM_2)) {
        _portraits->setIndex(2);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())->setZoom(2);
    }
    if(inputController->isKeyPressed(KeyCode::NUM_3)) {
        _portraits->setIndex(3);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())->setZoom(2);
    }
    if(inputController->isKeyPressed(KeyCode::NUM_4)) {
        _portraits->setIndex(4);
        std::dynamic_pointer_cast<OrthographicCamera>(_scene->getCamera())->setZoom(2);
    }
    Vec3 offset(405, 315, 0);
    _scene->getCamera()->setPosition(_portraits->getPosition(_portraits->getIndex()) + offset);
    _portraits->updateBattery();
    _scene->getCamera()->update();
    //CULog("%f, %f, %f", _scene->getCamera()->getPosition().x, _scene->getCamera()->getPosition().y, _scene->getCamera()->getPosition().z);
    
    
    // Will crash the program because the constructor doesn't set up the model/view yet (delete this comment later)
//    _hunter.update();
    _spirit.update();
    // TODO: update direction index for portraits on spirit control
//    _portraits->updateDirectionIndex(<#Vec3 direction#>, <#int index#>)
}

/**
 * Draws all this scene to the given SpriteBatch.
 *
 * The default implemen3tation of this method simply draws the scene graph
 * to the sprite batch.  By overriding it, you can do custom drawing
 * in its place.
 *
 * @param batch     The SpriteBatch to draw with.
 */
void SGameController::render(std::shared_ptr<cugl::SpriteBatch>& batch) {
    // CULog("Rendering!");
    _scene->render(batch);
    displayBattery(_portraits->getCurBattery(), _portraits->getCurState(), batch);
}

void SGameController::displayBattery(float battery, bool state, std::shared_ptr<cugl::SpriteBatch>& batch){
    CULog("%f", battery);
    if (state){
        batch->setColor(Color4::GREEN);
    } else{
        batch->setColor(Color4::RED);
    }
}

void SGameController::checkLevelLoaded() {
    _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        _levelLoaded = false;
    }

    // Check to see if new level loaded yet
    if (!_levelLoaded && _assets->complete()) {
        _level = nullptr;

        // Access and initialize level
        _level = _assets->get<LevelModel>(LEVEL_ONE_KEY);
        _level->setAssets(_assets);

        CULog("Loading level!");

        // TODO: implement direction and direction limits
        for(int i = 0; i < _level->getPortaits().size(); i++) {
            Vec3 pos(_level->getPortaits()[i].x, _level->getPortaits()[i].y, 0);
            _portraits->addPortrait(i + 1, pos, Vec3(0, 0, -1), Vec2::ZERO, _level->getBattery());
            //CULog("%f, %f", _level->getPortaits()[i].x, _level->getPortaits()[i].y);
        }
        _tilemap->updatePosition(_scene->getSize() / 2);
        std::vector<std::vector<std::string>> tiles = _level->getTileTextures();
        _tilemap->updateDimensions(Vec2(tiles[0].size(), tiles.size()));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(45, 45));
        for (int i = 0; i < tiles.size() * tiles[0].size(); ++i){
            int c = i%tiles[0].size();
            int r = i/tiles[0].size();
            if (tiles[r][c] == "red") {
                _tilemap->addTile(c, r, Color4::RED, true, _assets->get<Texture>("red"));
            } else if (tiles[r][c] == "black") {
                _tilemap->addTile(c, r, Color4::BLACK, false, _assets->get<Texture>("black"));
            } else if (tiles[r][c] == "green") {
                _tilemap->addTile(c, r, Color4::GREEN, true, _assets->get<Texture>("green"));
            } else if (tiles[r][c] == "blue") {
                _tilemap->addTile(c, r, Color4::BLUE, true, _assets->get<Texture>("blue"));
            }
        }

        _levelLoaded = true;
        _portraits->setMaxbattery(_level->getBattery());
    }
}

void SGameController::generateLevel() {
    _tilemap->updateDimensions(_level->getDimensions());
    
}
