#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"


#include "../lib/glm/glm.hpp"


EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
Map* map;
SDL_Renderer* Game::renderer;
SDL_Event Game::event;


Game::Game()
{
    this -> isRunning = false;
    
}

Game::~Game()
{


}

bool Game::IsRunning() const
{
    return this -> isRunning;
}

void Game::Initialize(int width, int height)
{
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    window = SDL_CreateWindow(
       NULL,
       SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED,
       WINDOW_WIDTH,
       WINDOW_HEIGHT,
       SDL_WINDOW_BORDERLESS
    ); 
    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
}
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber)
{
    /*Start including new assets to the asset manager list*/
    assetManager -> AddTexture("tank-image", std::string("./assets/images/tank-big-left.png").c_str());
    assetManager -> AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager -> AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    /*Start including entities and components to them*/
    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));

    Entity& chopperEntity(manager.AddEntity("chopper", PLAYER_LAYER));
    chopperEntity.AddComponent<TransformComponent>(40,60, 0, 0, 32, 32, 1);
    chopperEntity.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    chopperEntity.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
    
}

void Game::ProcessInput()
{
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT: 
            {
                isRunning = false;
                break;
            }
        case SDL_KEYDOWN:
            {
                if( event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
            }
        default:
            {
                break;
            }
    }
}

void Game::Update()
{
    //Sleep the execution til we reach the target frame time in ms
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    //Only call delay if we are too fast to process the frame
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) SDL_Delay(timeToWait);

    //deltaTime is the difference in ticks (?) from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    //Clamp deltaTime to a max value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    //new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();
    
    manager.Update(deltaTime);
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) return;

    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
