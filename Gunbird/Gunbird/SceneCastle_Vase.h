#pragma once
#ifndef __SceneCastle_Vase_H__
#define __SceneCastle_Vase_H__

#include "Enemy.h"
#include "ModuleParticles.h"


class SceneCastle_Vase : public Enemy
{
private:

	Animation vase;
	Animation vaseRed;
	Animation vaseWhite;
	iPoint original_pos;

	uint lastShot;

public:

	SceneCastle_Vase(int x, int y);
	void OnCollision(Collider* collider);
	void Move();
	uint getLives();

};

#endif