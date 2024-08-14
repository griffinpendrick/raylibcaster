#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "player.h"
#include "stdlib.h"
#include "stdio.h"
#include "float.h"

#define NUM_RAYS            960
#define MAP_SCALAR          0.25f
#define SIDEBAR_WIDTH       240         // PROJECTION_PLANE * MAP_SCALAR
#define WINDOW_WIDTH        1200        // PROJECTION_PLANE + SIDEBAR_WIDTH
#define WINDOW_HEIGHT       960

Rectangle PANEL_SIZE = {PROJECTION_PLANE, SIDEBAR_WIDTH, SIDEBAR_WIDTH, PROJECTION_PLANE - SIDEBAR_WIDTH};

int MAP[MAP_HEIGHT][MAP_WIDTH] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,};

void LoadTextures(Texture2D textures[]){
    textures[1]  = LoadTexture("assets/textures/purplestone.png");
    textures[2]  = LoadTexture("assets/textures/bluestone.png");
    textures[3]  = LoadTexture("assets/textures/colorstone.png");
    textures[4]  = LoadTexture("assets/textures/eagle.png");
    textures[5]  = LoadTexture("assets/textures/greenlight.png");
    textures[6]  = LoadTexture("assets/textures/greystone.png");
    textures[7]  = LoadTexture("assets/textures/mossy.png");
    textures[8]  = LoadTexture("assets/textures/pillar.png");
    textures[9]  = LoadTexture("assets/textures/barrel.png");
    textures[10] = LoadTexture("assets/textures/redbrick.png");
    textures[11] = LoadTexture("assets/textures/wood.png");
}

void UnloadTextures(Texture2D textures[]){
    UnloadTexture(textures[1]);
    UnloadTexture(textures[2]);
    UnloadTexture(textures[3]);
    UnloadTexture(textures[4]);
    UnloadTexture(textures[5]);
    UnloadTexture(textures[6]);
    UnloadTexture(textures[7]);
    UnloadTexture(textures[8]);
    UnloadTexture(textures[9]);
    UnloadTexture(textures[10]);
    UnloadTexture(textures[11]);
}

void drawMap(Player player, Texture2D textures[]){
    
    Vector2 scaledPosition      = Vector2Scale(player.position, MAP_SCALAR);
    scaledPosition.x            += PROJECTION_PLANE + 1;

    // Draw Map Background
    DrawRectangle(PROJECTION_PLANE + 1, 0, SIDEBAR_WIDTH, SIDEBAR_WIDTH, DARKGRAY);

    // Fill Grid
    for (u8 y = 0; y < MAP_HEIGHT; ++y){
        for (u8 x = 0; x < MAP_WIDTH; ++x){
            if (MAP[y][x])
                DrawTextureEx(textures[MAP[y][x]], (Vector2){(x * CELL_SIZE * MAP_SCALAR) + PROJECTION_PLANE + 1, y * CELL_SIZE * MAP_SCALAR}, 0.0f, MAP_SCALAR, WHITE);
        }
    }

    // Draw Vertical Grid Lines
    for (u8 x = 0; x <= MAP_HEIGHT; ++x)
        DrawLine((x * CELL_SIZE * MAP_SCALAR) + PROJECTION_PLANE + 1, 0, (x * CELL_SIZE * MAP_SCALAR) + PROJECTION_PLANE + 1, SIDEBAR_WIDTH, WHITE);
    DrawLine(WINDOW_WIDTH, 0, WINDOW_WIDTH, SIDEBAR_WIDTH, WHITE);

    // Draw Horizontal Grid Lines
    for (u8 y = 0; y <= MAP_WIDTH; ++y)
        DrawLine(PROJECTION_PLANE + 1, (y * CELL_SIZE * MAP_SCALAR), WINDOW_WIDTH, (y * CELL_SIZE * MAP_SCALAR), WHITE);
    DrawLine(PROJECTION_PLANE + 1, 1, WINDOW_WIDTH, 1, WHITE);

    // Draw Player
    DrawCircleV(scaledPosition, 4.0f, WHITE);
}

void updateMap(Player player, int selectedTileIndex){
    // Undo Map Scale
    Vector2 mousePosition = GetMousePosition();
    mousePosition.x       -= 960;
    mousePosition         = Vector2Scale(mousePosition, 4);

    s8 xIndex = worldToGrid(mousePosition.x);
    s8 yIndex = worldToGrid(mousePosition.y);

    // Place and Remove Walls using Minimap
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        if(!MAP[yIndex][xIndex] && MAP[yIndex][xIndex] != 1)
            MAP[yIndex][xIndex] = selectedTileIndex;
        else if(MAP[yIndex][xIndex] && MAP[yIndex][xIndex] != 1)
            MAP[yIndex][xIndex] = 0;
    }

    // Makes Sure User can not Place Wall on Self
    MAP[worldToGrid(player.position.y)][worldToGrid(player.position.x)] = 0;
}

void drawMapRay(Player player, Vector2 intersection){

    Vector2 scaledPosition = Vector2Scale(player.position, MAP_SCALAR);
    scaledPosition.x += 960;

    Vector2 scaledIntersection = Vector2Scale(intersection, MAP_SCALAR);
    scaledIntersection.x += 960;

    DrawLineEx(scaledPosition, scaledIntersection, 1.0f, WHITE);
}

void castRays(Player player, Texture2D textures[]){

    Vector2 hIntersection;
    Vector2 hStep;
    f32     hDistance;

    Vector2 vIntersection;
    Vector2 vStep;
    f32     vDistance;

    f32 castArc;
    castArc = player.theta + toRadian(30);

    for (u16 column = 0; column < NUM_RAYS; column++){
    
        if(castArc > 0 && castArc < PI){
            // Casting Ray Up
            hIntersection.y = (int)(player.position.y / CELL_SIZE) * CELL_SIZE;
            hIntersection.x = player.position.x + (player.position.y - hIntersection.y) / tan(castArc);
            hStep.y = -CELL_SIZE;
            hStep.x = (CELL_SIZE / tan(castArc));
            hIntersection.y -= 1;
        }
        else {
            // Casting Ray Down
            hIntersection.y = (int)(player.position.y / CELL_SIZE) * CELL_SIZE + CELL_SIZE;
            hIntersection.x = player.position.x + (player.position.y - hIntersection.y) / tan(castArc);
            hStep.y = CELL_SIZE;
            hStep.x = -(CELL_SIZE / tan(castArc));
        }

        if(castArc < (PI / 2) || castArc > (3 * PI / 2)){
            // Casting Ray Right
            vIntersection.x = (int)(player.position.x / CELL_SIZE) * CELL_SIZE + CELL_SIZE;
            vIntersection.y = player.position.y + (player.position.x - vIntersection.x) * tan(castArc);
            vStep.x = CELL_SIZE;
            vStep.y = -(CELL_SIZE * tan(castArc));
        }
        else {
            // Casting Ray Left
            vIntersection.x = (int)(player.position.x / CELL_SIZE) * CELL_SIZE;
            vIntersection.y = player.position.y + (player.position.x - vIntersection.x) * tan(castArc);
            vStep.x = -CELL_SIZE;
            vStep.y = (CELL_SIZE * tan(castArc));
            vIntersection.x -= 1;
        }
        
        // Grid Coords of Ray Intersections
        s8 xIndex;
        s8 yIndex;

        // Looking Left or Right
        if (castArc == 0 || castArc == PI || castArc == 2 * PI){
            hDistance = FLT_MAX;
        }
        else{
            // Move Ray Until it Hits a Horizontal Wall
            for(;;){
                xIndex = worldToGrid(hIntersection.x);
                yIndex = worldToGrid(hIntersection.y);
                
                // Check if We Are Outside Map Bounds
                if (xIndex >= MAP_WIDTH || yIndex > MAP_HEIGHT || xIndex < 0 || yIndex < 0){
                    hDistance = FLT_MAX;
                    break;
                }
                // Check if Ray Hits Wall
                else if (MAP[yIndex][xIndex]){
                    hDistance = Vector2Distance(player.position, hIntersection);
                    break;
                }
                // Extend Ray if Ray Does not Hit a Wall
                else{
                    hIntersection = Vector2Add(hIntersection, hStep);
                }
            }
        }
        
        // Looking Up and Down
        if (castArc == (PI / 2) || castArc == (3 * PI / 2)){
            vDistance = FLT_MAX;
        }
        else{
            // Move Ray Until it Hits a Vertical Wall
            for(;;){
                xIndex = worldToGrid(vIntersection.x);
                yIndex = worldToGrid(vIntersection.y);
                
                // Check if We Are Outside Map Bounds
                if (xIndex >= MAP_WIDTH || yIndex >= MAP_HEIGHT || xIndex < 0 || yIndex < 0){
                    vDistance = FLT_MAX;
                    break;
                }
                // Check if Ray Hits Wall
                else if (MAP[yIndex][xIndex]){
                    vDistance = Vector2Distance(player.position, vIntersection);
                    break;
                }
                // Extend Ray if Ray Does not Hit a Wall
                else{
                    vIntersection = Vector2Add(vIntersection, vStep);
                }
            }
        }
        
        f32 distance;
        Vector2 wallIntersection;
        int textureOffset;

        if (hDistance < vDistance){
            distance = hDistance;

            wallIntersection = hIntersection;
            textureOffset = (int)wallIntersection.x % CELL_SIZE;
        }
        else{
            distance = vDistance;

            wallIntersection = vIntersection;
            textureOffset = (int)wallIntersection.y % CELL_SIZE;
        }

        drawMapRay(player, wallIntersection);

        distance *= cosf(player.theta - castArc);
        
        f32 projectedWallHeight = CELL_SIZE * PROJECTION_PLANE / distance;
        f32 bottomWall          = (PROJECTION_PLANE / 2) + (projectedWallHeight / 2);
        f32 topWall             = (PROJECTION_PLANE / 2) - (projectedWallHeight / 2);
        f32 wallLength          = (bottomWall - topWall) + 1;

        yIndex = worldToGrid(wallIntersection.y);
        xIndex = worldToGrid(wallIntersection.x);

        Rectangle source = {(float)textureOffset, 0.0f, 1.0f, CELL_SIZE};
        Rectangle dest   = {column, topWall, 1.0f, wallLength};
        Vector2   origin = {0.0f, 0.0f};
        DrawTexturePro(textures[MAP[yIndex][xIndex]], source, dest, origin, 0.0f, WHITE);

        // Clamp castArc Between 0-360 and Update castArc
        if (castArc > toRadian(360.0f)) castArc -= toRadian(360.0f);
        castArc -= toRadian(60.0f / NUM_RAYS);
    }
}

void drawGui(Player *player){

    GuiPanel(PANEL_SIZE, "SETTINGS");

    GuiLabel((Rectangle){965, 270, 100, 20}, "Game Controls:");
    GuiLabel((Rectangle){965, 290, 100, 20}, "Move Forward: W");
    GuiLabel((Rectangle){965, 310, 100, 20}, "Move Left: A");
    GuiLabel((Rectangle){965, 330, 100, 20}, "Move Back: S");
    GuiLabel((Rectangle){965, 350, 100, 20}, "Move Right: D");
    GuiLabel((Rectangle){965, 370, 200, 20}, "Change Current Tile: SPACE");

    GuiLabel((Rectangle){965, 400, 100, 20}, "Player Speed:");
    GuiSlider((Rectangle){1010, 420, 100, 20}, "1", "20", &player->speed, 1.0f, 20.0f);
}

int main(){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "");
    GuiLoadStyle("assets/styles/dark/style_dark.rgs");
    SetTargetFPS(62);

    Player player = createPlayer();

    Texture2D textures[12];
    LoadTextures(textures);

    int selectedTileIndex = 2;

    while(!WindowShouldClose()){

        // Draws GUI Panel
        drawGui(&player);

        // Update Player Position and Map
        updatePosition(&player);
        updateMap(player, selectedTileIndex);

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Temp Ceiling and Floor
        DrawRectangle(0, 0, PROJECTION_PLANE, PROJECTION_PLANE / 2, GRAY);
        DrawRectangle(0, PROJECTION_PLANE / 2, PROJECTION_PLANE, PROJECTION_PLANE, DARKGRAY);

        // Draw Minimap
        drawMap(player, textures);
        DrawFPS(1120, 935);

        // Raycaster
        castRays(player, textures);

        EndDrawing();

        if(IsKeyPressed(KEY_SPACE)) 
            selectedTileIndex++;
        else if(selectedTileIndex == 11)
            selectedTileIndex = 2;
    }

    UnloadTextures(textures);
    CloseWindow();
    return 0;
}
