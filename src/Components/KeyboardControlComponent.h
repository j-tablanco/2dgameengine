#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Constants.h"
#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"
#include <type_traits>
#include <iostream>

class KeyboardControlComponent : public Component
{
    public:
        std::string upKey;
        std::string downKey;
        std::string leftKey;
        std::string rightKey;
        std::string shootKey;
        TransformComponent *transform;
        SpriteComponent *sprite;

        KeyboardControlComponent(){}

        KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey)
        {
            this->upKey = GetSDLKeyStringCode(upKey);
            this->rightKey = GetSDLKeyStringCode(rightKey);
            this->downKey = GetSDLKeyStringCode(downKey);
            this->leftKey = GetSDLKeyStringCode(leftKey);
            this->shootKey = GetSDLKeyStringCode(shootKey);
        }

        std::string GetSDLKeyStringCode(std:: string key)
        {
            if(key.compare("up") == 0) return "1073741906";
            if(key.compare("down") == 0) return "1073741905";
            if(key.compare("left") == 0) return "1073741904";
            if(key.compare("right") == 0) return "1073741903";
            if(key.compare("space") == 0) return "32";
            return std::to_string(static_cast<int>(key[0]));
        }

        void Initialize() override
        {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
        }

        void Update( float deltaTime) override
        {
            transform -> velocity.x = 0;
            transform -> velocity.y = 0;
            std::string keyCode;
            if( Game::event.type == SDL_KEYDOWN)
            {
                keyCode = std::to_string(Game::event.key.keysym.sym);

                if (keyCode.compare(upKey) == 0)
                {

                    if(transform->position.y >= 0)
                        transform->velocity.y = -90; 
                    sprite->Play("UpAnimation");
                }
                if (keyCode.compare(rightKey) == 0)
                {
                    if( transform->position.x <= WINDOW_WIDTH-32)
                        transform->velocity.x = 250;
                    sprite->Play("RightAnimation");
                }
                if (keyCode.compare(downKey) == 0)
                {
                    if( transform->position.y <= WINDOW_HEIGHT-32)
                        transform->velocity.y = 250; 
                    sprite->Play("DownAnimation");
                }
                if (keyCode.compare(leftKey) == 0)
                {
                    if( transform->position.x >= 0)
                        transform->velocity.x = -90;
                    sprite->Play("LeftAnimation");
                }
                if (keyCode.compare(shootKey) == 0)
                {
                    //TODO 
                    //pew pew
                }
            }
            if (Game::event.type == SDL_KEYUP)
            { 
                keyCode = std::to_string(Game::event.key.keysym.sym);

                if (keyCode.compare(upKey) == 0)
                {
                    transform->velocity.y = 0;
                }
                if (keyCode.compare(rightKey) == 0)
                {
                    transform->velocity.x = 0;
                }
                if (keyCode.compare(downKey) == 0)
                {
                    transform->velocity.y = 0;
                }
                if (keyCode.compare(leftKey) == 0)
                {
                    transform->velocity.x = 0;
                }
            }
} 


        
        
};




#endif
