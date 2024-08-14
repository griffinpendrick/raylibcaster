#include "player.h"

Player createPlayer(){
    Player player;
    player.speed        = 3.0f;
    player.theta        = PI / 2;
    player.position     = (Vector2){PROJECTION_PLANE / 2.0f, PROJECTION_PLANE / 1.5f};
    player.direction    = (Vector2){cosf(player.theta), -sinf(player.theta)};
    return player;
}

void updatePosition(Player *player){
    Vector2 newPosition = player->position;

    if (IsKeyDown(KEY_W)){
        newPosition = Vector2Add(player->position, Vector2Scale(player->direction, player->speed));
    }
    if (IsKeyDown(KEY_S)){
        newPosition = Vector2Subtract(player->position, Vector2Scale(player->direction, player->speed));
    }
    if (IsKeyDown(KEY_A)) {
        player->theta += ROTATION_SPEED;
        player->direction = (Vector2){cosf(player->theta), -sinf(player->theta)};
    }
    if (IsKeyDown(KEY_D)) {
        player->theta -= ROTATION_SPEED;
        player->direction = (Vector2){cosf(player->theta), -sinf(player->theta)};
    }

    // Find World Position of Players Current Position
    s8 xIndex = worldToGrid(newPosition.x);
    s8 yIndex = worldToGrid(newPosition.y);

    // Calculate Map Index of New Position including Size of Player
    s8 minXIndex = worldToGrid((f32)(newPosition.x - PLAYER_SIZE));
    s8 maxXIndex = worldToGrid((f32)(newPosition.x + PLAYER_SIZE));

    s8 minYIndex = worldToGrid((f32)(newPosition.y - PLAYER_SIZE));
    s8 maxYIndex = worldToGrid((f32)(newPosition.y + PLAYER_SIZE));

    // Check for Wall Collision
    if (MAP[worldToGrid(player->position.y)][minXIndex] == 0 && MAP[worldToGrid(player->position.y)][maxXIndex] == 0)
        player->position.x = newPosition.x;

    if (MAP[minYIndex][worldToGrid(player->position.x)] == 0 && MAP[maxYIndex][worldToGrid(player->position.x)] == 0)
        player->position.y = newPosition.y;

    // Clamp theta between 0 and 2 * PI
    if (player->theta > 2 * PI)     player->theta = 0.0f;
    if (player->theta < 0.0f)       player->theta = 2 * PI;
}

