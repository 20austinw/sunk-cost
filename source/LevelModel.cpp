//
// Created by ryupark1321 on 2/23/23.
//

#include "LevelModel.h"
#include "LevelConstants.h"

#pragma mark -
#pragma mark Static Constructors

/**
 * Creates a new, empty level.
 */
LevelModel::LevelModel() : Asset() { _bounds.size.set(1.0f, 1.0f); }

/**
 * Destroys this level, releasing all resources.
 */
LevelModel::~LevelModel(void) { unload(); }

#pragma mark -
#pragma mark Asset Loading
/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene
 * initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel::preload(const std::string& file) {
    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);

    return preload(reader->readJson());
}

/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene
 * initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    if (json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    // Initial geometry
    float w = json->get(WIDTH_FIELD)->asFloat();
    float h = json->get(HEIGHT_FIELD)->asFloat();
    _bounds.size.set(w, h);

    // Set battery
    _battery = json->get(BATTERY_FIELD)->asInt();

    // Get each object in each layer, then decide what to do based off of what
    // type the object is.
    auto objects = json->get("layers")->get(0);
    loadObject(objects);
    for (int i = 1; i < json->get("layers")->size(); i++) {
        // Get the objects per layer
        objects = json->get("layers")->get(i)->get("objects");
        for (int j = 0; j < objects->size(); j++) {
            // For each object, determine what it is and load it
            loadObject(objects->get(j));
        }
    }
    return true;
}

/**
 * Unloads this game level, releasing all sources
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded and
 * unloaded in parallel, not in sequence.  If an asset (like a game level) has
 * references to other assets, then these should be disconnected earlier.
 */
void LevelModel::unload() {
    _bounds = Rect::ZERO;
    _tiles.clear();
    _dimensions = Size::ZERO;
    _tileSize = Size::ZERO;
    _defaultcam = Vec2::ZERO;
    _portraits.clear();
    _player = Vec2::ZERO;
    _mapSize = Size::ZERO;
    _assets = nullptr;
}

#pragma mark -
#pragma mark Individual Loaders

bool LevelModel::loadObject(const std::shared_ptr<JsonValue>& json) {
    auto type = json->get("class")->asString();
    if (type == TILE_FIELD) {
        return loadTiles(json);
    } else if (type == PORTRAIT_FIELD) {
        return loadPortraitSetAndDefault(json);
    } else if (type == PLAYER_FIELD) {
        return loadPlayer(json);
    }
    return false;
}

/**
 * Loads the singular exit door
 *
 * The exit door will will be stored in _goalDoor field and retained.
 * If the exit fails to load, then _goalDoor will be nullptr.
 *
 * @param  reader   a JSON reader with cursor ready to read the exit
 *
 * @return true if the exit door was successfully loaded
 */
bool LevelModel::loadTiles(const std::shared_ptr<JsonValue>& json) {
    auto tiles = json->get("data");
    int width = json->get("width")->asInt();
    int height = json->get("height")->asInt();
    _dimensions = Size(width, height);
    for (int i = 0; i < height; ++i) {
        std::vector<std::string> vec;
        _tiles.push_back(vec);
    }
    bool success = tiles->get(0) != nullptr;
    if (success) {
        for (int i = 0; i < width * height; ++i) {
            int c = i % width;
            int r = i / width;
            int type = tiles->get(c + r * width)->asInt();
            if (type == 1) {
                _tiles[r].push_back("black");
            } else if (type == 3) {
                _tiles[r].push_back("door");
            } else {
                _tiles[r].push_back("green");
            }

            //              if (type > 24 && type < 27) {
            //            _tiles[r].push_back("green");
            //          } else if (type == 29) {
            //            _tiles[r].push_back("blue");
            //          } else if (type == 11) {
            //            _tiles[r].push_back("red");
            //          }
        }
    }
    return success;
}

/**
 * Loads the singular exit door
 *
 * The exit door will will be stored in _goalDoor field and retained.
 * If the exit fails to load, then _goalDoor will be nullptr.
 *
 * @param  reader   a JSON reader with cursor ready to read the exit
 *
 * @return true if the exit door was successfully loaded
 */
bool LevelModel::loadPlayer(const std::shared_ptr<JsonValue>& json) {
    bool success = json->get("x") != nullptr;
    if (success) {
        float x = json->getFloat("x");
        float y = json->getFloat("y");
        _player = Vec2(x, y);
    }
    return success;
}

/**
 * Loads the singular exit door
 *
 * The exit door will will be stored in _goalDoor field and retained.
 * If the exit fails to load, then _goalDoor will be nullptr.
 *
 * @param  reader   a JSON reader with cursor ready to read the exit
 *
 * @return true if the exit door was successfully loaded
 */
bool LevelModel::loadPortraitSetAndDefault(
    const std::shared_ptr<JsonValue>& json) {
    _portraits.push_back(
        std::make_pair(Vec2(json->getFloat("x1"), json->getFloat("y1")),
                       Vec2(json->getFloat("x2"), json->getFloat("y2"))));
    return true;
}
