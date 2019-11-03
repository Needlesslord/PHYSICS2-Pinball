#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

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
	iPoint initialPosition;
	PhysBody* sensor;
	PhysBody* ball;
	PhysBody* background;
	PhysBody* backgroundL;

	PhysBody* death;
	PhysBody* rightFlipper;
	PhysBody* leftFlipper;

	b2RevoluteJoint* leftFlipper_joint;
	b2RevoluteJoint* rightFlipper_joint;

	bool sensed;

	SDL_Texture* background_tex;
	SDL_Texture* ball_tex;
	SDL_Texture* leftFlipper_tex;
	SDL_Texture* rightFlipper_tex;
	SDL_Texture* numLives_tex0;
	SDL_Texture* numLives_tex1;
	SDL_Texture* numLives_tex2;
	SDL_Texture* numLives_tex3;

	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;
	int life = 0;
	bool alive = true;

};
