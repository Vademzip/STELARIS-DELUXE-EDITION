#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:
    void Update();
    Enemy(Model model, Vector3 position, Vector3 speed = { 0.0f,0.0f,0.0f }, int hitpoints = 500);
    void Rotate();
};

