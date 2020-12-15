#include "Level1.h"
#define LEVEL1_WIDTH 18
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 2
unsigned int level1_data[] =

{
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 3, 3,
 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 3,
 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 3,
 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
 2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 3, 3,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3
};


void Level1::Initialize() {
    state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("newTileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -4, 0);
    state.player->movement = glm::vec3(0);
    //state.player->acceleration = glm::vec3(0, -9.81f, 0);
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
    state.player->jumpPower = 5.8f;



    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID1 = Util::LoadTexture("alien.png");
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID1;
    state.enemies[0].position = glm::vec3(9, -3.25, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    state.enemies[0].isActive = true;

    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID1;
    state.enemies[1].position = glm::vec3(10, -4.75, 0 );
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1;
    state.enemies[1].isActive = true;
}
void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    if (state.player->position.x >= 13) {
        state.nextScene = 2;
    }


    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}