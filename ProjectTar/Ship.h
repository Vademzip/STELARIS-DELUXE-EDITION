#pragma once
#include "Entity.h"
#include "GenericShader.h"

class Ship : public Entity
{
public:
    void Update();
    void Draw();
    Ship(Model model, Vector3 position, Vector3 speed = { 0.0f,0.0f,0.0f }, int hitpoints = 500);
    //void shader();
    BoundingBox boundingBox();
};

