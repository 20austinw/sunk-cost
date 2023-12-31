//
// Created by ryupark1321 on 2/23/23.
//

#ifndef _LEVEL_CONSTANTS_H
#define _LEVEL_CONSTANTS_H

//
//  JSLevelConstants.h
//  JSONDemo
//
//  This file contains all of the details that define the JSON structure of a
//  LevelModel. Created by Walker White on 1/20/16.
//
//  Author of Referenced Files: Walker White
//  Version: 2/23/23
//
#ifndef _LEVEL_CONSTANTS_H__
#define _LEVEL_CONSTANTS_H__

/** The global fields of the level model */
#define WIDTH_FIELD "width"
#define HEIGHT_FIELD "height"
#define TILE_SIZE "tileSize"

#define PORTRAIT_FIELD "portrait"
#define PLAYER_FIELD "player"
#define DOOR_FIELD "doors"

#define UNDER_FIELD "undership"
#define FLOOR_FIELD "floor"
#define AO_FIELD "floor ao"
#define FV_1_FIELD "floorboard variations"
#define FV_2_FIELD "floorboard variations 2"
#define CARPETS_FIELD "carpets"
#define WALLS_FIELD "walls"
#define WALL_UPPER_FIELD "wall upper"
#define WALL_GRIME_FIELD "wall grime"
#define DECOR_FIELD "decor"
#define D_0_FIELD "detail0"
#define D_1_FIELD "detail"
#define D_2_FIELD "detail2"
#define D_3_FIELD "detail3"

#define COLLISION_FIELD "collision"

/** The physics fields for each object */
#define POSITION_FIELD "pos"
#define SIZE_FIELD "size"
#define BODYTYPE_FIELD "bodytype"
#define DENSITY_FIELD "density"
#define ROTATION_FIELD "rotation"
#define STATIC_VALUE "static"
#define BATTERY_FIELD "battery"

#define POSITION_FIELD "pos"
#define SIZE_FIELD "size"
#define BODYTYPE_FIELD "bodytype"
#define DENSITY_FIELD "density"
#define ROTATION_FIELD "rotation"
#define STATIC_VALUE "static"
#define BATTERY_FIELD "battery"
#define TREASURE_FIELD "treasure"
#define TRAP_FIELD "trap"
/** The drawing fields for each object */
#define TEXTURE_FIELD "texture"

/** The wall specific fields */
#define VERTICES_FIELD "polygon"
#define BOUNDARY_FIELD "boundary"

/** The source for our level file */
#define LEVEL_ONE_FILE "json/gppbasic_level.json"
#define LEVEL_TWO_FILE "json/technical_level.json"
#define LEVEL_THREE_FILE "json/large_map.json"
#define LEVEL_FINAL_FILE "json/bigmap.json"

/** The key for our loaded level */
#define LEVEL_ONE_KEY "basic_level"
#define LEVEL_TWO_KEY "technical_level"
#define LEVEL_THREE_KEY "large_map"
#define LEVEL_FINAL_KEY "bigmap"

#endif /* defined(__TD_LEVEL_CONSTANTS_H__) */

#endif //_LEVEL_CONSTANTS_H
