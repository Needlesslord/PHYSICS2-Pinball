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

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background_tex = App->textures->Load("pinball/Background.png");//TODO 100: LOAD ALL PNGs
	ball_tex = App->textures->Load("pinball/Bola.png");
	leftFlipper_tex = App->textures->Load("pinball/leftFlipper.png");
	rightFlipper_tex = App->textures->Load("pinball/rightFlipper.png");
	numLives_tex0 = App->textures->Load("pinball/Numbers0.png");
	numLives_tex1 = App->textures->Load("pinball/Numbers1.png");
	numLives_tex2 = App->textures->Load("pinball/Numbers2.png");
	numLives_tex3 = App->textures->Load("pinball/Numbers3.png");
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
	App->textures->Unload(rightFlipper_tex);
	App->textures->Unload(numLives_tex0);
	App->textures->Unload(numLives_tex1);
	App->textures->Unload(numLives_tex2);
	App->textures->Unload(numLives_tex3);

	return true;
}

update_status ModuleSceneIntro::PreUpdate() {
	

	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModuleSceneIntro::Update()
{
//STATIC THINGS:--------------------------------------------------------------------------
	//BACKGROUND
	App->renderer->Blit(background_tex, 0, 0, NULL, 1.0f);
	
	//NUMBER OF LIVES
	if		(numLives == 3)	App->renderer->Blit(numLives_tex3, 457, 708, NULL, 1.0f);
	else if (numLives == 2) App->renderer->Blit(numLives_tex2, 457, 708, NULL, 1.0f);
	else if (numLives == 1) App->renderer->Blit(numLives_tex1, 457, 708, NULL, 1.0f);
	else					App->renderer->Blit(numLives_tex0, 457, 708, NULL, 1.0f);


// All draw functions --------------------------------------------------------------------
		//BALL
	if (ball != NULL) {
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ball_tex, x, y, NULL, 1.0f, ball->GetRotation());
	}
		//FLIPPERS
	//LEFT
	if (leftFlipper != NULL) {
		int x, y;
		leftFlipper->GetPosition(x, y);
		App->renderer->Blit(leftFlipper_tex, x, y, NULL, 1.0f, leftFlipper->GetRotation());
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
		}
	 }

}
bool ModuleSceneIntro::LoadMap() {
	//BALL FIRST
	ball = App->physics->CreateCircle(454, 421, 15);
	ball->body->SetBullet(true);
	ball->body->GetFixtureList()->SetFriction(0.4f);
	ball->listener = this;
	numLives = 3;

	//BACKGROUND
	background = App->physics->CreateChain(0, 0, Background, 180);
	background->body->SetType(b2_staticBody);
	backgroundL = App->physics->CreateChain(0, 0, BackgroundL, 14);
	backgroundL->body->SetType(b2_staticBody);

	//FLIPPERS
	//LEFT
	leftFlipper = App->physics->CreateRectangle(195, 705, 85, 13);


	//JOINTS-------------------------------------------------------------
		//FLIPPERS
			//LEFT
	b2RevoluteJointDef leftFlipper_revolute;
	PhysBody* circleLeft;
	circleLeft = App->physics->CreateCircle(155, 705, 5);
	circleLeft->body->SetType(b2_staticBody);
	leftFlipper_revolute.Initialize(leftFlipper->body, circleLeft->body, circleLeft->body->GetWorldCenter());
	leftFlipper_revolute.collideConnected = false;
	leftFlipper_revolute.enableLimit = true;
	leftFlipper_revolute.lowerAngle = -20 * DEGTORAD;
	leftFlipper_revolute.upperAngle = 30 * DEGTORAD;
	leftFlipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&leftFlipper_revolute);
	//RIGHT
	b2RevoluteJointDef rightFlipper_revolute;
	PhysBody* circleRight;


	death = App->physics->CreateRectangleSensor(0, 700, SCREEN_WIDTH * 2, 1);
	return true;
}