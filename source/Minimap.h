//
//  Minimap.h
//  Sunk
//
//  Created by Austin Wu on 4/19/23.
//

#ifndef Minimap_h
#define Minimap_h
#include <cugl/cugl.h>

using namespace cugl;

class Minimap {
#pragma mark Internal References
  private:
    std::shared_ptr<scene2::PolygonNode> _node;

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

    Minimap(const std::shared_ptr<cugl::AssetManager>& assets, Vec2 position,
            Size size, int color){};

    /** Deletes this HunterView */
    ~Minimap() {}

#pragma mark Scene Methods
  public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {}

    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {}

#endif /* Minimap_h */
