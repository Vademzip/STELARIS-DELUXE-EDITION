#include "Ship.h"
#include "LightingShader.H"
#include "raymath.h"
//#include "Entity.h"

Ship::Ship(Model mod, Vector3 spos, Vector3 speed, int hitpoints) : Entity(mod, spos, speed, hitpoints) 
{
}

void Ship::Update()
{
	if (IsKeyDown(KEY_A)) pos.x -= 0.05; //нажатие клавиш
	if (IsKeyDown(KEY_D)) pos.x += 0.05;
	if (IsKeyDown(KEY_W)) pos.y += 0.05;
	if (IsKeyDown(KEY_S)) pos.y -= 0.05;

	if (pos.x < -10) pos.x = -10;
	if (pos.y < -6) pos.y = -6;
	if (pos.x > 10) pos.x = 10;
	if (pos.y > 6) pos.y = 6;
}

void Ship::Draw()
{
	Entity::Draw();
	
}

BoundingBox Ship::boundingBox() {
	return BoundingBox{ Vector3{ pos.x - 2.5f, pos.y - 2.5f,pos.z - 6 },
						Vector3{ pos.x + 2.5f, pos.y + 2.5f,pos.z + 5 }
	};
}
