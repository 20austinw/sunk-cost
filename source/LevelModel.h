//
// Created by ryupark1321 on 2/23/23.
//

#ifndef _LEVEL_MODEL_H
#define _LEVEL_MODEL_H

#include <cugl/cugl.h>
#include <vector>
#include <cugl/assets/CUAsset.h>
#include <cugl/io/CUJsonReader.h>
#include "HunterModel.h"
#include "SpiritModel.h"
#include "PortraitSet.h"
#include "TileModel.h"

using namespace cugl;

#pragma mark -
#pragma mark Level Model
class LevelModel : public Asset {
    /** The bounds of this level in physics coordinates */
    Rect _bounds;
    
    /** Vector of textures for tiles */
    std::vector<std::vector<std::string>> _tiles;
    
    /** Dimenisons of this tilemap */
    Size _dimensions;
    
    /** Tile Size of this tilemap */
    Size _tileSize;
    
    /** Position of the default camera */
    Vec2 _defaultcam;
    
    /** Vector of positions of portraits */
    std::vector<Vec2> _portraits;
    
    /** Position data of player */
    Vec2 _player;
    
    /** Reference to vector of tiles */
    Size _mapSize;
    
    /** The AssetManager for the game mode */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** The battery limit for the protraits */
    int _battery;
    
public:
#pragma mark Static Constructors
  /**
   * Creates a new game level with no source file.
   *
   * The source file can be set at any time via the setFile() method. This method
   * does NOT load the asset.  You must call the load() method to do that.
   *
   * @return  an autoreleased level file
   */
  static std::shared_ptr<LevelModel> alloc() {
    std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
    return (result->init("") ? result : nullptr);
  }

  /**
   * Creates a new game level with the given source file.
   *
   * This method does NOT load the level. You must call the load() method to do that.
   * This method returns false if file does not exist.
   *
   * @return  an autoreleased level file
   */
  static std::shared_ptr<LevelModel> alloc(std::string file) {
    std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
    return (result->init(file) ? result : nullptr);
  }
    
#pragma mark Getters
    
    std::vector<std::vector<std::string>> getTileTextures() {
        return _tiles;
    }
    
    Size getDimensions() {
        return _dimensions;
    }
    
    Size getTileSize() {
        return _tileSize;
    }
    
    Vec2 getDefaultCamPosition() {
        return _defaultcam;
    }
    
    std::vector<Vec2> getPortaits() {
        return _portraits;
    }
    
    Vec2 getPlayerPosition() {
        return _player;
    }
    
    Size getMapSize() {
        return _mapSize;
    }
    
    int getBattery(){
        return _battery;
    }
    
#pragma mark Physics Attributes
  /**
   * Returns the bounds of this level in physics coordinates
   *
   * @return the bounds of this level in physics coordinates
   */
  const Rect& getBounds() const   { return _bounds; }

  /**
   * Sets the loaded assets for this game level
   *
   * @param assets the loaded assets for this game level
   */
  void setAssets(const std::shared_ptr<AssetManager>& assets) { _assets = assets; }

#pragma mark -
#pragma mark Asset Loading
  /**
   * Loads this game level from the source file
   *
   * This load method should NEVER access the AssetManager.  Assets are loaded in
   * parallel, not in sequence.  If an asset (like a game level) has references to
   * other assets, then these should be connected later, during scene initialization.
   *
   * @param file the name of the source file to load from
   *
   * @return true if successfully loaded the asset from a file
   */
  virtual bool preload(const std::string& file);


  /**
   * Loads this game level from a JsonValue containing all data from a source Json file.
   *
   * This load method should NEVER access the AssetManager.  Assets are loaded in
   * parallel, not in sequence.  If an asset (like a game level) has references to
   * other assets, then these should be connected later, during scene initialization.
   *
   * @param json the json loaded from the source file to use when loading this game level
   *
   * @return true if successfully loaded the asset from the input JsonValue
   */
  virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

  /**
   * Unloads this game level, releasing all sources
   *
   * This load method should NEVER access the AssetManager.  Assets are loaded and
   * unloaded in parallel, not in sequence.  If an asset (like a game level) has
   * references to other assets, then these should be disconnected earlier.
   */
  void unload();

  //#pragma mark -
  //#pragma mark Initializers
  /**
   * Creates a new, empty level.
   */
  LevelModel(void);

  /**
   * Destroys this level, releasing all resources.
   */
  virtual ~LevelModel(void);

#pragma mark -
#pragma mark Individual Loaders
  /**
    * Determines which object is currently being parsed,
    * and then sends the object over
    * to the respective load function
    *
    * @param reader a JSON reader with cursor ready to read the object
    *
    * @return true if the object was successfully loaded
    */
  bool loadObject(const std::shared_ptr<JsonValue>& json);

  /**
    * Parses JSON and extracts portrait/default camera positions from it
    *
    * @param reader a JSON reader with cursor ready to read the portraitset
    *
    * @return true if the object was successfully loaded
    */
  bool loadPortraitSetAndDefault(const std::shared_ptr<JsonValue>& json);

  /**
    * Parses JSON and extracts player positions from it
    *
    * @param reader a JSON reader with cursor ready to read the player
    *
    * @return true if the object was successfully loaded
    */
  bool loadPlayer(const std::shared_ptr<JsonValue>& json);

  /**
    * Parses JSON and extracts tile positions and textures from it
    *
    * @param reader a JSON reader with cursor ready to read the tiles
    *
    * @return true if the object was successfully loaded
    */
  bool loadTiles(const std::shared_ptr<JsonValue>& json);
};


#endif //_LEVEL_MODEL_H
