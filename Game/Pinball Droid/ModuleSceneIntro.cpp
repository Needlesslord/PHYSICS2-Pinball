#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Chain.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	initialPosition.x = 454;
	initialPosition.y = 421;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background_tex = App->textures->Load("pinball/Background.png");//TODO 100: LOAD ALL PNGs
	ball_tex = App->textures->Load("pinball/Bola.png");
	leftFlipper_tex = App->textures->Load("pinball/leftFlipper.png");
	leftupFlipper_tex = App->textures->Load("pinball/leftFlipper.png");
	rightFlipper_tex = App->textures->Load("pinball/rightFlipper.png");
	rightUpArrows_tex = App->textures->Load("pinball/left.png");
	leftUpArrows_tex = App->textures->Load("pinball/right.png");
	leftArrows_tex = App->textures->Load("pinball/Up.png");
	leftlight_tex = App->textures->Load("pinball/L_Light.png");
	
	numLives_tex0 = App->textures->Load("pinball/Numbers0.png");
	numLives_tex1 = App->textures->Load("pinball/Numbers1.png");
	numLives_tex2 = App->textures->Load("pinball/Numbers2.png");
	numLives_tex3 = App->textures->Load("pinball/Numbers3.png");

	flipper_fx = App->audio->LoadFx("pinball/Flipper.wav");
	App->audio->LoadFx("pinball/New_ball.wav");
	//Activate combos
	light_fx = App->audio->LoadFx("pinball/Light.wav");

	LoadMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background_tex);//TODO 101: UNLOAD EVERYTHING
	App->textures->Unload(ball_tex);
	App->textures->Unload(leftFlipper_tex);
	App->textures->Unload(leftupFlipper_tex);
	App->textures->Unload(rightFlipper_tex);
	App->textures->Unload(numLives_tex0);
	App->textures->Unload(numLives_tex1);
	App->textures->Unload(numLives_tex2);
	App->textures->Unload(numLives_tex3);
	return true;
}

update_status ModuleSceneIntro::PreUpdate() {

	//move
	if (standby) {
		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT))
		{
			intensity++;
			if (intensity > 200) intensity = 200;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
			ball->body->ApplyLinearImpulse({ 0, (-0.05f * intensity) }, { 0,0 }, true);
			intensity = 0;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		leftFlipper->body->ApplyAngularImpulse(-4.0F, true);
		leftupFlipper->body->ApplyAngularImpulse(-4.0F, true);

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		rightFlipper->body->ApplyAngularImpulse(4.0F, true);
	}
	
	//fx
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipper_fx);
	}

	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//STATIC THINGS:--------------------------------------------------------------------------
		//BACKGROUND
	App->renderer->Blit(background_tex, 0, 0, NULL, 1.0f);

	//NUMBER OF LIVES
	if (numLives == 3)	App->renderer->Blit(numLives_tex3, 457, 708, NULL, 1.0f);
	else if (numLives == 2) App->renderer->Blit(numLives_tex2, 457, 708, NULL, 1.0f);
	else if (numLives == 1) App->renderer->Blit(numLives_tex1, 457, 708, NULL, 1.0f);
	else					App->renderer->Blit(numLives_tex0, 457, 708, NULL, 1.0f);


	// All draw functions --------------------------------------------------------------------

	//FLIPPERS
		//LEFT
	if (leftFlipper != NULL) {
		int x, y;
		leftFlipper->GetPosition(x, y);
		App->renderer->Blit(leftFlipper_tex, x, y - 10, NULL, 1.0f, leftFlipper->GetRotation());
	}
	//LEFT-UP
	if (leftupFlipper != NULL) {
		int x, y;
		leftupFlipper->GetPosition(x, y);
		App->renderer->Blit(leftupFlipper_tex, x, y - 10, NULL, 1.0f, leftFlipper->GetRotation());
	}
	//RIGHT
	if (rightFlipper != NULL) {
		int x, y;
		rightFlipper->GetPosition(x, y);
		App->renderer->Blit(rightFlipper_tex, x, y - 10, NULL, 1.0f, rightFlipper->GetRotation());
	}
	//LIGHTS
	if ((sensor_arrows_upright != NULL) && (sensor_arrows_upright_b == true)) {
		App->renderer->Blit(rightUpArrows_tex, 96, 19);
	}
	if ((sensor_arrows_upleft != NULL) && (sensor_arrows_upleft_b == true)) {
		App->renderer->Blit(leftUpArrows_tex, 5, 19);
	}
	if ((sensor_arrows_left != NULL) && (sensor_arrows_left_b == true)) {
		App->renderer->Blit(leftArrows_tex, 9, 233);
	}
	if ((LeftTopLight != NULL) && (left_top_light_b == true)) {
		App->renderer->Blit(leftlight_tex, 90, 307);
	}
	if ((LeftMidLight != NULL) && (left_mid_light_b == true)) {
		App->renderer->Blit(leftlight_tex, 80, 325);
	}
	if ((LeftBotLight != NULL) && (left_bot_light_b == true)) {
		App->renderer->Blit(leftlight_tex, 70, 343);
	}
	if (isDead)
	{
		ball->body->GetWorld()->DestroyBody(ball->body);
		ball = App->physics->CreateCircle(initialPosition.x, initialPosition.y, 15);
		ball->listener = this;
		isDead = false;
		standby = true;
		if (numLives < 0) gameOver();
	}

	//BALL
	if (ball != NULL) {
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ball_tex, x, y, NULL, 1.0f);
	}
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate() {
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		int x = App->input->GetMouseX();
		int y = App->input->GetMouseY();
		ball->body->GetWorld()->DestroyBody(ball->body);
		ball = App->physics->CreateCircle(x, y, 15);
		ball->listener = this;
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

	if (bodyA == ball) {
		if (bodyB == death) {
			numLives--;
			isDead = true;
			sensor_arrows_upright_b = false;
			sensor_arrows_upleft_b = false;
			sensor_arrows_left_b = false;
			
		}
		if (bodyB == standby_sensor) {
			standby = false;
		}
		if (bodyB == sensor_arrows_upright && ball->body->GetLinearVelocity().y > 0) {
			sensor_arrows_upright_b = true;
		}
		if (bodyB == sensor_arrows_upleft && ball->body->GetLinearVelocity().y > 0) {
			sensor_arrows_upleft_b = true;
		}
		if (bodyB == sensor_arrows_left && ball->body->GetLinearVelocity().y < 0) {
			sensor_arrows_left_b = true;
		}
		if (bodyB == bouncerTriangleLeft) {

		}
		if (bodyB == LeftTopLight) {
			left_top_light_b = true;
		}
		if (bodyB == LeftMidLight){
			left_mid_light_b = true;
		}
		if (bodyB == LeftBotLight) {
			left_bot_light_b = true;
		}
	}
}
bool ModuleSceneIntro::LoadMap() {
	//BALL FIRST
	ball = App->physics->CreateCircle(initialPosition.x, initialPosition.y, 15);
	ball->body->SetBullet(true);
	ball->body->GetFixtureList()->SetFriction(0.4f);
	ball->listener = this;
	numLives = 2;

	//BACKGROUND
	background				= App->physics->CreateChain(0, 0, Background, 180);
	background->body->SetType(b2_staticBody);
	triangleLeft			= App->physics->CreateChain(0, 0, BackgroundL, 14);//TRIANGLE LEFT
	triangleLeft->body->SetType(b2_staticBody);
	triangleRight			= App->physics->CreateChain(0, 0, BackgroundR, 18);//TRIANGLE RIGHT
	triangleRight->body->SetType(b2_staticBody);
	curvePostTriangleLeft	= App->physics->CreateChain(0, 0, ChainL, 16);//CURVE POST TRIANGLE LEFT
	curvePostTriangleLeft->body->SetType(b2_staticBody);
	curvePostTriangleRight	= App->physics->CreateChain(0, 0, ChainR, 22);//CURVE POST TRIANGLE RIGHT
	curvePostTriangleRight->body->SetType(b2_staticBody);
	topFlipperBase			= App->physics->CreateChain(0, 0, BackgroundU, 20);//TOP FLIPPER BASE
	topFlipperBase->body->SetType(b2_staticBody);
	threeButtonStick		= App->physics->CreateChain(0, 0, BackgroundLine, 8);//THREE BUTTONS STICK
	threeButtonStick->body->SetType(b2_staticBody);
	
	bouncerTriangleLeft		= App->physics->CreateChain(0, 0, PivoteUR, 42);//BOUNCER TOP - MID
	bouncerTriangleLeft->body->SetType(b2_staticBody);
	bouncerTriangleLeft->body->GetFixtureList()->SetRestitution(1.2F);
	bouncerTriangleLeft->body->GetFixtureList()->SetFriction(0.3F);
	
	bouncerTriangleRight	= App->physics->CreateChain(0, 0, PivoteUL, 40);//BOUNCER TOP-RIGHT
	bouncerTriangleRight->body->SetType(b2_staticBody);
	bouncerTriangleRight->body->GetFixtureList()->SetRestitution(1.2F);
	bouncerTriangleRight->body->GetFixtureList()->SetFriction(0.3F);

	bouncerTopLeft			= App->physics->CreateChain(0, 0, PivoteU, 46);//BOUNCER TOP - LEFT / LONE BOUNCER
	bouncerTopLeft->body->SetType(b2_staticBody);
	bouncerTopLeft->body->GetFixtureList()->SetRestitution(1.2F);
	bouncerTopLeft->body->GetFixtureList()->SetFriction(0.3F);

	bouncerTriangleBot		= App->physics->CreateChain(0, 0, PivoteL, 54);//BOUNCER BOT
	bouncerTriangleBot->body->SetType(b2_staticBody);
	bouncerTriangleBot->body->GetFixtureList()->SetRestitution(1.2F);
	bouncerTriangleBot->body->GetFixtureList()->SetFriction(0.3F);

	triangleLeftBouncer		= App->physics->CreateChain(0, 0, BouncyL, 8);
	triangleLeftBouncer->body->SetType(b2_staticBody);
	triangleLeftBouncer->body->GetFixtureList()->SetRestitution(1.2f);
	triangleLeftBouncer->body->GetFixtureList()->SetFriction(0.3f);

	triangleRightBouncer	= App->physics->CreateChain(0, 0, BouncyR, 8);
	triangleRightBouncer->body->SetType(b2_staticBody);
	triangleRightBouncer->body->GetFixtureList()->SetRestitution(1.2f);
	triangleRightBouncer->body->GetFixtureList()->SetFriction(0.3f);

	doubleBonusLeftBar		= App->physics->CreateChain(0, 0, BarraL, 12);
	doubleBonusLeftBar->body->SetType(b2_staticBody);
	doubleBonusRightBar		= App->physics->CreateChain(0, 0, BarraR, 12);
	doubleBonusRightBar->body->SetType(b2_staticBody);
	x5multiplierTop			= App->physics->CreateChain(0, 0, Hole1, 12);
	x5multiplierTop->body->SetType(b2_staticBody);
	x5multiplierTopRight	= App->physics->CreateChain(0, 0, Hole2, 10);
	x5multiplierTopRight->body->SetType(b2_staticBody);	
	x10multiplier			= App->physics->CreateChain(0, 0, Hole3, 14);
	x10multiplier->body->SetType(b2_staticBody);
	x20multiplier			= App->physics->CreateChain(0, 0, Hole4, 12);
	x20multiplier->body->SetType(b2_staticBody);	
	x20multiplierLight		= App->physics->CreateChain(0, 0, Light1, 12);
	x20multiplierLight->body->SetType(b2_staticBody);

	LeftBotLight			= App->physics->CreateChain(0, 0, Light2, 10);
	LeftBotLight->body->SetType(b2_staticBody);

	LeftMidLight			= App->physics->CreateChain(0, 0, Light3, 10);
	LeftMidLight->body->SetType(b2_staticBody);
	LeftTopLight			= App->physics->CreateChain(0, 0, Light4, 8);
	LeftTopLight->body->SetType(b2_staticBody);
	midTopLight				= App->physics->CreateChain(0, 0, Light5, 12);
	midTopLight->body->SetType(b2_staticBody);
	midMidLight				= App->physics->CreateChain(0, 0, Light6, 12);
	midMidLight->body->SetType(b2_staticBody);
	midBotLight				= App->physics->CreateChain(0, 0, Light7, 12);
	midBotLight->body->SetType(b2_staticBody);
	rightTopLight			= App->physics->CreateChain(0, 0, Light8, 10);
	rightTopLight->body->SetType(b2_staticBody);
	rightMidLight			= App->physics->CreateChain(0, 0, Light9, 12);
	rightMidLight->body->SetType(b2_staticBody);
	rightBotLight			= App->physics->CreateChain(0, 0, Light10, 12);
	rightBotLight->body->SetType(b2_staticBody);

	//FLIPPERS
	//LEFT
	leftFlipper = App->physics->CreateRectangle(182, 727, 80, 20);
	leftupFlipper = App->physics->CreateRectangle(85, 247, 80, 20);
	rightFlipper = App->physics->CreateRectangle(296, 727, 80, 20);


	//JOINTS------------------------------------------------------------------------------------------------------------
		//FLIPPERS
			//LEFT
	b2RevoluteJointDef leftFlipper_revolute;
	PhysBody* circleLeft;
	circleLeft = App->physics->CreateCircle(153, 727, 10);
	circleLeft->body->SetType(b2_staticBody);
	leftFlipper_revolute.Initialize(leftFlipper->body, circleLeft->body, circleLeft->body->GetWorldCenter());
	leftFlipper_revolute.collideConnected = false;
	leftFlipper_revolute.enableLimit = true;
	leftFlipper_revolute.lowerAngle = -30 * DEGTORAD;
	leftFlipper_revolute.upperAngle = 30 * DEGTORAD;
	leftFlipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&leftFlipper_revolute);

			//LEFT-UP
	b2RevoluteJointDef leftupFlipper_revolute;
	PhysBody* circleLeftup;
	circleLeftup = App->physics->CreateCircle(56, 247, 10);
	circleLeftup->body->SetType(b2_staticBody);
	leftupFlipper_revolute.Initialize(leftupFlipper->body, circleLeftup->body, circleLeftup->body->GetWorldCenter());
	leftupFlipper_revolute.collideConnected = false;
	leftupFlipper_revolute.enableLimit = true;
	leftupFlipper_revolute.lowerAngle = -30 * DEGTORAD;
	leftupFlipper_revolute.upperAngle = 30 * DEGTORAD;
	leftupFlipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&leftupFlipper_revolute);

			//RIGHT
	b2RevoluteJointDef rightFlipper_revolute;
	PhysBody* circleRight;
	circleRight = App->physics->CreateCircle(325, 727, 10);
	circleRight->body->SetType(b2_staticBody);
	rightFlipper_revolute.Initialize(rightFlipper->body, circleRight->body, circleRight->body->GetWorldCenter());
	rightFlipper_revolute.collideConnected = false;
	rightFlipper_revolute.enableLimit = true;
	rightFlipper_revolute.lowerAngle = -30 * DEGTORAD;
	rightFlipper_revolute.upperAngle = 30 * DEGTORAD;
	rightFlipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&rightFlipper_revolute);


	//SENSORS
		//DEATH
	death = App->physics->CreateRectangleSensor(0, 790, SCREEN_WIDTH * 2, 1);
		//STANDBY
	standby_sensor = App->physics->CreateRectangleSensor(390, 20, 1, 25);
		//x5, x10, x20
	sensor_x5up = App->physics->CreateRectangleSensor(202, 32, 5, 5);
	sensor_x5right = App->physics->CreateRectangleSensor(400, 65, 5, 5);
	sensor_x10 = App->physics->CreateRectangleSensor(415, 256, 5, 5);
	sensor_x20 = App->physics->CreateRectangleSensor(25, 720, 5, 5);
		//ARROWS
	sensor_arrows_upright = App->physics->CreateRectangleSensor(15, 75, 5, 5);
	sensor_arrows_upleft = App->physics->CreateRectangleSensor(150, 70, 5, 5);
	sensor_arrows_left = App->physics->CreateRectangleSensor(15, 245, 5, 5);

	return true;
}
void ModuleSceneIntro::gameOver() {
	numLives = 3;
	LeftTopLight = false;
	LeftMidLight = false;
	LeftBotLight = false;
}