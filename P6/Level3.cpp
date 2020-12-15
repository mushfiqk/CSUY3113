#include "Level3.h"
#define LEVEL3_WIDTH 18
#define LEVEL3_HEIGHT 8
#define LEVEL3_ENEMY_COUNT 4
unsigned int level3_data[] =

{
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,
 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 3, 3,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3
};


void Level3::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("newTileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("queenresized.png");

    state.player->animRight = new int[4]{ 11, 10, 9, 8 };
    state.player->animLeft = new int[4]{ 15, 14, 13, 12 };
    state.player->animUp = new int[4]{ 7, 6, 5, 4 };
    state.player->animDown = new int[4]{ 3, 2, 1, 0 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    



    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID1 = Util::LoadTexture("alien.png");
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID1;
    state.enemies[0].position = glm::vec3(4, -2.25, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    state.enemies[0].isActive = true;

    GLuint enemyTextureID2 = Util::LoadTexture("alien.png");
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID2;
    state.enemies[1].position = glm::vec3(10, -3.95, 0);
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1;
    state.enemies[1].isActive = true;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID2;
    state.enemies[2].position = glm::vec3(8, -5.95, 0);
    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 1;
    state.enemies[2].isActive = true;

    state.enemies[3].entityType = ENEMY;
    state.enemies[3].textureID = enemyTextureID2;
    state.enemies[3].position = glm::vec3(8, 0.00, 0);
    state.enemies[3].aiType = WAITANDGO;
    state.enemies[3].aiState = IDLE;
    state.enemies[3].speed = 1;
    state.enemies[3].isActive = true;



}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    if (state.player->position.x >= 14) {
        state.player->goal = true;
    }
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }
}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    GLuint fontTextureID = Util::LoadTexture("font1.png");

    if (state.player->goal == true) {
        

        Util::DrawText(program, fontTextureID, "You Win!", 1, -0.5f, glm::vec3(state.player->position.x, state.player->position.y, 0));
        state.player->speed = 0.0f;
        state.player->acceleration = glm::vec3(0, 0, 0);
    }
    
}