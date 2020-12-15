#include "menu.h"
#define MENU_WIDTH 14
#define MENU_HEIGHT 8
#define MENU_ENEMY_COUNT 0
unsigned int menu_data[] =

{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
};


void menu::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("newTileset.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -4, 0);
    state.player->movement = glm::vec3(0);

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


}
   
void menu::Update(float deltaTime) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if ((keys[SDL_SCANCODE_RETURN]) || (keys[SDL_SCANCODE_RETURN2])) { 
        state.nextScene = 1;
    }


}

void menu::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    GLuint fontTextureID = Util::LoadTexture("font1.png");


    Util::DrawText(program, fontTextureID, "SOCIAL DISTANCE", 0.90f, -0.5f, glm::vec3(1.75f, -2.95f, 0));
    Util::DrawText(program, fontTextureID, "You have 3 Lives", 0.85f, -0.5f, glm::vec3(1.75f, -3.95f, 0));
    Util::DrawText(program, fontTextureID, "Avoid All the Infected", 0.85f, -0.5f, glm::vec3(1.75f, -4.95f, 0));
    Util::DrawText(program, fontTextureID, "Press Enter to Start", 0.85f, -0.5f, glm::vec3(1.75f, -5.95f, 0));
}