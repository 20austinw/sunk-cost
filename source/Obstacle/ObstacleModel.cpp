//
//  ObstacleModel.cpp
//  Sunk
//
//  Created by 任清扬 on 13/3/23.
//

#include "ObstacleModel.hpp"
using namespace cugl;
using namespace std;

//  MARK: - Constants

/** Keys for obstacle textures */
#define TRAP_TEXTURE "trap"
#define TREASURE_TEXTURE "treasure"

/** The sizes of obstacles in Box2D units */
#define TRAP_WIDTH 10
#define TREASURE_WIDTH 10

//  MARK: - Constructors

/**
 * Initializes an Obstacle Model
 */
bool ObstacleModel::init(float scale, shared_ptr<Texture> &texture, Type type) {
  PolyFactory pf;
  switch (type) {
  case TRAP:
    PolygonObstacle::init(pf.makeRect(0, 0, TRAP_WIDTH, TRAP_WIDTH / 2));
    break;
  case TREASURE:
    PolygonObstacle::init(
        pf.makeRect(0, 0, TREASURE_WIDTH, TREASURE_WIDTH / 4));
    break;
  }
  setDebugColor(Color4::RED);

  return true;
}
