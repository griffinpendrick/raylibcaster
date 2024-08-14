#include "raylib.h"
#include "raymath.h"
#include "stdint.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef float       f32;
typedef double      f64;

#define PROJECTION_PLANE    960
#define CELL_SIZE           64
#define MAP_WIDTH           15
#define MAP_HEIGHT          15
#define PLAYER_SIZE         10.0f
#define ROTATION_SPEED      0.05f

#define toDegree(x)         x * (180 / PI) 
#define toRadian(x)         x * (PI / 180)
#define worldToGrid(x)      (int)(x / CELL_SIZE)  // Converts World Coordinates (Players Absolute Position) to Grid Coordinates

extern int MAP[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
    f32         speed;
    f32         theta;
    Vector2     position;
    Vector2     direction;
} Player;

Player createPlayer();
void updatePosition(Player *player);

#endif
