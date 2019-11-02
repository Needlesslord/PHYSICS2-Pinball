#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	int numLives;

private:
	bool LoadMap();
public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	

	PhysBody* sensor;
	PhysBody* ball;
	PhysBody* death;
	bool sensed;

	SDL_Texture* background_tex;
	SDL_Texture* ball_tex;
	SDL_Texture* numLives_tex0;
	SDL_Texture* numLives_tex1;
	SDL_Texture* numLives_tex2;
	SDL_Texture* numLives_tex3;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
