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
	void gameOver();

private:
	bool LoadMap();
public:
	iPoint initialPosition;
	PhysBody* sensor;
	PhysBody* ball;
	PhysBody* background;
	PhysBody* triangleLeft;
	PhysBody* triangleRight;
	PhysBody* curvePostTriangleLeft;
	PhysBody* curvePostTriangleRight;
	PhysBody* topFlipperBase;
	PhysBody* threeButtonStick;
	PhysBody* bouncerTriangleLeft;
	PhysBody* bouncerTriangleRight;
	PhysBody* bouncerTopLeft;
	PhysBody* bouncerTriangleBot;
	PhysBody* triangleLeftBouncer;
	PhysBody* triangleRightBouncer;
	PhysBody* doubleBonusLeftBar;
	PhysBody* doubleBonusRightBar;
	PhysBody* x5multiplierTop;
	PhysBody* x5multiplierTopRight;
	PhysBody* x10multiplier;
	PhysBody* x20multiplier;
	PhysBody* x20multiplierLight;
	PhysBody* leftBotLight;
	PhysBody* LeftMidLight;
	PhysBody* LeftTopLight;
	PhysBody* midTopLight;
	PhysBody* midMidLight;
	PhysBody* midBotLight;
	PhysBody* rightTopLight;
	PhysBody* rightMidLight;
	PhysBody* rightBotLight;
	PhysBody* death;
	PhysBody* standby_sensor;
	PhysBody* rightFlipper;
	PhysBody* leftFlipper;
	PhysBody* leftupFlipper;

	b2RevoluteJoint* leftFlipper_joint;
	b2RevoluteJoint* leftupFlipper_joint;
	b2RevoluteJoint* rightFlipper_joint;

	bool sensed;

	SDL_Texture* background_tex;
	SDL_Texture* ball_tex;
	SDL_Texture* leftFlipper_tex;
	SDL_Texture* leftupFlipper_tex;
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

	//death to initial position
	bool isDead = false;

	//PLUNGER
	bool standby = true;
	int intensity = 0;

	//To activate combos
	uint light_fx;
	uint enterhole;
	uint exxithole;
	SDL_Texture* lightUp;
	SDL_Texture* lightLeft;
	SDL_Texture* lightU;
	SDL_Texture* lightRight;
	bool OnLight1 = false;
	bool OnLight2 = false;
	bool OnLight3 = false;
	bool OnLight4 = false;
	bool OnLight5 = false;
	bool OnLight6 = false;
	bool OnLight7 = false;
	bool OnLight8 = false;
	bool OnLight9 = false;
	bool OnLight10 = false;
	//sensors x5, x10, x20
	PhysBody* sensor_x5up;
	PhysBody* sensor_x5right;
	PhysBody* sensor_x10;
	PhysBody* sensor_x20;
	//sensors arrows
	PhysBody* sensor_arrows_upright;
	PhysBody* sensor_arrows_uprleft;
	PhysBody* sensor_arrows_left;





};
