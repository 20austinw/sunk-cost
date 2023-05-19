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
    float w = json->get(WIDTH_FIELD)->asInt();
    float h = json->get(HEIGHT_FIELD)->asInt();
    _bounds.size.set(w, h);

    int t = json->get(TILE_SIZE)->asInt();
    _tileSize = t;
    _dimensions = Vec2(w, h);

    // Set battery
    _battery = json->get(BATTERY_FIELD)->asFloat();

    // Get each object in each layer, then decide what to do based off of what
    // type the object is.
    auto objects = json->get("layers")->get(0);
    loadObject(objects);

    for (int i = 1; i < 16; i++) {
        objects = json->get("layers")->get(i);
        loadObject(objects);
    }

    for (int i = 16; i < json->get("layers")->size(); i++) {
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
    _tileSize = 0;
    _defaultcam = Vec2::ZERO;
    _portraits.clear();
    _player = Vec2::ZERO;
    _mapSize = Size::ZERO;
    _assets = nullptr;
    _boarder.clear();
}

#pragma mark -
#pragma mark Individual Loaders

bool LevelModel::loadObject(const std::shared_ptr<JsonValue>& json) {
    auto type = json->get("class")->asString();
    if (type == FLOOR_FIELD) {
        return loadTiles(json);
    } else if (type == PORTRAIT_FIELD) {
        return loadPortraitSetAndDefault(json);
    } else if (type == PLAYER_FIELD) {
        return loadPlayer(json);
    } else if (type == DOOR_FIELD) {
        return loadDoors(json);
    } else if (type == UNDER_FIELD) {
        return loadDetails(json);
    } else if (type == AO_FIELD) {
        return loadDetails(json);
    } else if (type == FV_1_FIELD) {
        return loadDetails(json);
    } else if (type == FV_2_FIELD) {
        return loadDetails(json);
    } else if (type == CARPETS_FIELD) {
        return loadDetails(json);
    } else if (type == WALLS_FIELD) {
        return loadDetails(json);
    } else if (type == WALL_UPPER_FIELD) {
        return loadDetails(json);
    } else if (type == WALL_GRIME_FIELD) {
        return loadDetails(json);
    } else if (type == DECOR_FIELD) {
        return loadDetails(json);
    } else if (type == D_0_FIELD) {
        return loadDetails(json);
    } else if (type == D_1_FIELD) {
        return loadDetails(json);
    } else if (type == D_2_FIELD) {
        return loadDetails(json);
    } else if (type == D_3_FIELD) {
        return loadDetails(json);
    } else if (type == COLLISION_FIELD) {
        return loadCollision(json);
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

    bool success = tiles->get(0) != nullptr;
    if (success) {
        for (int i = 0; i < tiles->size(); i++) {
            _tiles.emplace_back(tiles->get(i)->asInt());
        }
    }
    return success;
}

bool LevelModel::loadDetails(const std::shared_ptr<JsonValue>& json) {
    auto tiles = json->get("data");

    bool success = tiles->get(0) != nullptr;
    if (success) {
        std::vector<int> list;
        for (int i = 0; i < tiles->size(); i++) {
            list.emplace_back(tiles->get(i)->asInt());
        }
        _details.emplace_back(list);
    }
    return success;
}

bool LevelModel::loadCollision(const std::shared_ptr<JsonValue>& json) {
    float xOffset = json->get("x_offset")->asFloat();
    float yOffset = json->get("y_offset")->asFloat();
    int x = json->get("x")->asInt();
    int y = json->get("y")->asInt();
    auto boarder = json->get("boarder");
    bool success = boarder->get(0) != nullptr;
    for (int i = 0; i < boarder->size(); i += 2) {
        float xw = boarder->get(i)->asFloat();
        xw = (xOffset + x * xw) * 128;
        float yw = boarder->get(i + 1)->asFloat();
        yw = (yOffset + y * yw) * 128;
        _boarder.emplace_back(Vec2(xw, yw));
    }
    auto walls = json->get("walls");
    for (int i = 0; i < walls->size(); i++) {
        auto obs = walls->get(i)->get("data");
        std::vector<Vec2> v;
        for (int n = 0; n < obs->size(); n += 2) {
            float xw = obs->get(n)->asFloat();
            xw = (xOffset + x * xw) * 128;
            float yw = obs->get(n + 1)->asFloat();
            yw = (yOffset + y * yw) * 128;
            v.emplace_back(Vec2(xw, yw));
        }
        _collision.emplace_back(v);
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

bool LevelModel::loadDoors(const std::shared_ptr<JsonValue>& json) {
    // TODO
    auto doors = json->get("objects");
    bool success = doors->get(0) != nullptr;
    if (success) {
        for (int i = 0; i < doors->size(); i++) {
            auto door = doors->get(i);
            _doors.push_back(
                std::make_pair(Vec2(door->getFloat("x"), door->getFloat("y")),
                               door->getInt("type")));
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
bool LevelModel::loadPortraitSetAndDefault(
    const std::shared_ptr<JsonValue>& json) {
    _portraits.push_back(
        std::vector({Vec2(json->getFloat("x1"), json->getFloat("y1")),
                     Vec2(json->getFloat("x2"), json->getFloat("y2")),
                     Vec2(json->getFloat("x3"), json->getFloat("y3"))}));
    return true;
}
