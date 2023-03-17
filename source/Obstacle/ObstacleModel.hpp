//
//  ObstacleModel.hpp
//  Sunk
//
//  Created by 任清扬 on 13/3/23.
//

#ifndef ObstacleModel_hpp
#define ObstacleModel_hpp

#include <cugl/cugl.h>
using namespace cugl;

class ObstacleModel : public cugl::physics2::PolygonObstacle {
public:
  //  MARK: - Enumerations

  /** The different type of obstacles you can make */
  enum Type {
    /** A trap */
    TRAP,
    /** A treasure */
    TREASURE,

  };
  //  MARK: - Constants

/** Keys for obstacle textures */
#define TRAP_TEXTURE "trap"
#define TREASURE_TEXTURE "treasure"

/** The sizes of obstacles in Box2D units */
#define TRAP_WIDTH 10
#define TREASURE_WIDTH 10

protected:
  //  MARK: - Properties

  /** The type of obstacle this model represents */
  Type _type;

public:
  //  MARK: - Constructors

  /**
   * Constructs an Obstacle Model
   */
  ObstacleModel(){};

  /**
   * Destructs an Obstacle Model
   */
  ~ObstacleModel() { dispose(); }

  /**
   * Disposes of all resources in this instance of Obstacle Model
   */
  void dispose(){};

  /**
   * Initializes an Obstacle Model
   */

  bool init(float scale, std::shared_ptr<Texture> &texture, Type type);
};

#endif /* ObstacleModel_hpp */
