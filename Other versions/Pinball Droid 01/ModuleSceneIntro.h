#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "p2Animation.h"


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
	bool LoadMap();


public:

	//Handouts -----------------
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	//-----------------------------

	//The Game -------------------
	SDL_Texture* assets;

	//plunger
	p2Animation			plungerlight;
	PhysBody*			plunger;
	b2PrismaticJoint*   plunger_joint;
	uint				plunger_fx;
	//flippers
	PhysBody*			right_flipper;
	PhysBody*			left_flipper;
	PhysBody*			left_up_flipper;
	b2RevoluteJoint*    right_joint;
	b2RevoluteJoint*    left_joint;
	b2RevoluteJoint*    left_up_joint;
	uint				flipper_fx;




	//Variables
	bool death = false;


	//---------------------------------

};
