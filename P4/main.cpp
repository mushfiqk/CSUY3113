#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>
#include "map.h"
#include "Util.h"


#include "Entity.h"
#define ENEMY_COUNT 3

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 5
unsigned int level1_data[] =
{
 3, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 1, 1, 0, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1,
 3, 2, 2, 1, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

struct GameState {
    Map *map;
    Entity *player;
    Entity *enemies;

};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music *music;


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO |SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();

    state.player->entityType = PLAYER;

    state.player->position = glm::vec3(0,10,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.8f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f; 
    state.player->width = 0.8f;
    state.player->jumpPower = 7;

   
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);

    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID1 = Util::LoadTexture("ctg.png");
    GLuint enemyTextureID2 = Util::LoadTexture("ctg.png");
    GLuint enemyTextureID3 = Util::LoadTexture("spike.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID1;
    state.enemies[0].position = glm::vec3(5, -2.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].width = 0.8f;
    state.enemies[0].height = 0.8f;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID2;
    state.enemies[1].position = glm::vec3(3, 0.25f, 0);
    state.enemies[1].speed = 0.5;
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].width = 0.8f;
    state.enemies[1].height = 0.8f;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID1;
    state.enemies[2].position = glm::vec3(7, 1.25, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = CHASER;
    state.enemies[2].aiState = CHASE;
    state.enemies[2].width = 0.8f;
    state.enemies[2].height = 0.8f;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                if (state.player->collidedBottom) { 
                    state.player->jump = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }


    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        
        state.player->Update(FIXED_TIMESTEP, state.player, state.enemies, ENEMY_COUNT, state.map);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player,state.enemies, ENEMY_COUNT, state.map);

        } 
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-state.player->position.x, 0, 0));
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    program.SetViewMatrix(viewMatrix); 
    state.map->Render(&program);
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    state.player->Render(&program);
    
    if (!(state.player->gameOver)) {
       
        Util::DrawText(&program, fontTextureID, "You Win!", 1, -0.5f, glm::vec3(-4.2f, 2.5, 0));
    }
    else {
        Util::DrawText(&program, fontTextureID, "You Lose!", 1, -0.5f, glm::vec3(-4.2f, 2.5, 0));
    }
    

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}