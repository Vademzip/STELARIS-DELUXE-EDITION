#include "Enemy.h"
#include <raymath.h>

void Enemy::Update()
{
	Rotate();
	Entity::Update();
}

Enemy::Enemy(Model mod, Vector3 spos, Vector3 speed, int hitpoints) : Entity(mod, spos, speed, hitpoints) 
{
	switch (GetRandomValue(1, 4))
	{
	case 1:
		tint = ORANGE; // цвет закраски астероида
		break;
	case 2:
		tint = MAROON;
		break;
	case 3:
		tint = DARKGREEN;
		break;
	default:
		tint = RED;
		break;
	}

	if (Vector3Equals(speed, Vector3Zero())) spd.z = 0.05f + GetRandomValue(1, 5) / 100; // если скорость не была передана, то она задаётся случайно
	hp += GetRandomValue(-50, 50);
	rotatoSpd += GetRandomValue(5, 9) / 100;

}

void Enemy::Rotate()
{
	rotato += rotatoSpd;
	if (rotato > 6.28) rotato = 0;  //6,28 - 2пи
	model.transform = MatrixRotateZ(rotato);
	
}
