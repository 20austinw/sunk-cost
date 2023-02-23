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
#include "GameController.h"

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
GameController::GameController(const Size displaySize,
                               const std::shared_ptr<std::mt19937>& randoms):
_scene(cugl::Scene2::alloc(displaySize)) {
    _randoms = randoms;
    
    /// Initialize the tilemap and add it to the scene
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
    _template = 0;
    
    /// Initialize generator class with a boolean to seed random generator
    _generator = std::make_unique<GeneratorController>(true,_randoms);
}

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void GameController::update(float dt) {
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    /// Pre-made templates
    if (inputController->isKeyPressed(KeyCode::NUM_1)) {
        generateTemplate(1);
    } else if (inputController->isKeyPressed(KeyCode::NUM_2)) {
        generateTemplate(2);
    } else if (inputController->isKeyPressed(KeyCode::NUM_3)) {
        generateTemplate(3);
    } else if (inputController->isKeyPressed(KeyCode::NUM_4)) {
        generateTemplate(4);
    } else if (inputController->isKeyPressed(KeyCode::NUM_5)) {
        generateTemplate(5);
    }
    
    /// Tile size modifier with `-` and `=`
    if (inputController->isKeyPressed(KeyCode::EQUALS)) { _tilemap->modifyTileSize(2, 2); }
    else if (inputController->isKeyPressed(KeyCode::MINUS)) { _tilemap->modifyTileSize(0.5, 0.5); }
    
    /// Dimension modifier with `[` and `]` for modifying columns
    /// and `;` and `'` for modifying rows
    if (inputController->isKeyPressed(KeyCode::LEFT_BRACKET)) { 
      _tilemap->modifyDimensions(-1, 0);  
    } else if (inputController->isKeyPressed(KeyCode::RIGHT_BRACKET)) { 
      _tilemap->modifyDimensions(1, 0);  
    } else if (inputController->isKeyPressed(KeyCode::SEMICOLON)) { 
      _tilemap->modifyDimensions(0, -1);
    } else if (inputController->isKeyPressed(KeyCode::QUOTE)) { 
      _tilemap->modifyDimensions(0, 1);
    }
    
    /// Color inversion with `\`
    if (inputController->isKeyPressed(KeyCode::BACKSLASH)) {
        _tilemap->invertColor();
    }
    
    // Regenerating tile map with a new (random) seed using 'S'
    if (inputController->isKeyPressed(KeyCode::S)) {
        unsigned seed = static_cast<unsigned>(time(0));
        _randoms->seed(seed);
        CULog("Seed = %d", seed);
        _generator = std::make_unique<GeneratorController>(true,_randoms);
        generateTemplate(_template);
    }
}


#pragma mark -
#pragma mark Generation Helpers

/** Generates a yellow smiley face with a black smile. */
void GameController::generateSmileyFace() {
    _tilemap->updateDimensions(Vec2(20, 20));
    _tilemap->updateColor(Color4::YELLOW);
    _tilemap->updateTileSize(Size(10, 10));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    Color4 tileColor = Color4::BLACK;
    
    // Eyes
    _tilemap->addTile(5, 15, tileColor);
    _tilemap->addTile(15, 15, tileColor);
    
    // Nose
    _tilemap->addTile(10, 8, tileColor);
    
    // Smile
    _tilemap->addTile(3, 5, tileColor);
    _tilemap->addTile(4, 4, tileColor);
    for(int i = 5; i <= 15; i++) {
        _tilemap->addTile(i, 3, tileColor);
    }
    _tilemap->addTile(16, 4, tileColor);
    _tilemap->addTile(17, 5, tileColor);
}


#pragma mark -
#pragma mark Helpers

