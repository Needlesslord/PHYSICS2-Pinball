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
	rightFlipper_tex = App->textures->Load("pinball/rightFlipper.png");
	
	numLives_tex0 = App->textures->Load("pinball/Numbers0.png");
	numLives_tex1 = App->textures->Load("pinball/Numbers1.png");
	numLives_tex2 = App->textures->Load("pinball/Numbers2.png");
	numLives_tex3 = App->textures->Load("pinball/Numbers3.png");

	flipper_fx = App->audio->LoadFx("pinball/Flipper.wav");
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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		leftFlipper->body->ApplyAngularImpulse(-2.0F, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		rightFlipper->body->ApplyAngularImpulse(2.0F, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
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
	if		(numLives == 3)	App->renderer->Blit(numLives_tex3, 457, 708, NULL, 1.0f);
	else if (numLives == 2) App->renderer->Blit(numLives_tex2, 457, 708, NULL, 1.0f);
	else if (numLives == 1) App->renderer->Blit(numLives_tex1, 457, 708, NULL, 1.0f);
	else					App->renderer->Blit(numLives_tex0, 457, 708, NULL, 1.0f);


// All draw functions --------------------------------------------------------------------
		//BALL
	if (ball != NULL) {
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ball_tex, x, y, NULL, 1.0f);
	}
		//FLIPPERS
	//LEFT
	if (leftFlipper != NULL) {
		int x, y;
		leftFlipper->GetPosition(x, y);
		App->renderer->Blit(leftFlipper_tex, x, y - 10, NULL, 1.0f, leftFlipper->GetRotation());
	}
	if (rightFlipper != NULL) {
		int x, y;
		rightFlipper->GetPosition(x, y);
		App->renderer->Blit(rightFlipper_tex, x, y - 10, NULL, 1.0f, rightFlipper->GetRotation());
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
			/*//DOESN'T WORK FOR SOME REASON
			ball->body->GetWorld()->DestroyBody(ball->body);
			ball = App->physics->CreateCircle(initialPosition.x, initialPosition.y, 15);
			ball->listener = this;
			*/
		}
	 }

}
bool ModuleSceneIntro::LoadMap() {
	//BALL FIRST
	ball = App->physics->CreateCircle(initialPosition.x, initialPosition.y, 15);
	ball->body->SetBullet(true);
	ball->body->GetFixtureList()->SetFriction(0.4f);
	ball->listener = this;
	numLives = 3;

	//BACKGROUND
	background = App->physics->CreateChain(0, 0, Background, 180);
	background->body->SetType(b2_staticBody);
	backgroundL = App->physics->CreateChain(0, 0, BackgroundL, 14);
	backgroundL->body->SetType(b2_staticBody);
	backgroundR = App->physics->CreateChain(0, 0, BackgroundR, 18);
	backgroundR->body->SetType(b2_staticBody);
	chainL = App->physics->CreateChain(0, 0, ChainL, 16);
	chainL->body->SetType(b2_staticBody);
	chainR = App->physics->CreateChain(0, 0, ChainR, 22);
	chainR->body->SetType(b2_staticBody);
	backgroundU = App->physics->CreateChain(0, 0, BackgroundU, 20);
	backgroundU->body->SetType(b2_staticBody);
	backgroundLine = App->physics->CreateChain(0, 0, BackgroundLine, 8);
	backgroundLine->body->SetType(b2_staticBody);
	pivoteUR = App->physics->CreateChain(0, 0, PivoteUR, 42);
	pivoteUR->body->SetType(b2_staticBody);
	pivoteUL = App->physics->CreateChain(0, 0, PivoteUL, 40);
	pivoteUL->body->SetType(b2_staticBody);
	pivoteU = App->physics->CreateChain(0, 0, PivoteU, 46);
	pivoteU->body->SetType(b2_staticBody);
	pivoteL = App->physics->CreateChain(0, 0, PivoteL, 54);
	pivoteL->body->SetType(b2_staticBody);
	bouncyL = App->physics->CreateChain(0, 0, BouncyL, 8);
	bouncyL->body->SetType(b2_staticBody);
	bouncyR = App->physics->CreateChain(0, 0, BouncyR, 8);
	bouncyR->body->SetType(b2_staticBody);
	barraL = App->physics->CreateChain(0, 0, BarraL, 12);
	barraL->body->SetType(b2_staticBody);
	barraR = App->physics->CreateChain(0, 0, BarraR, 12);
	barraR->body->SetType(b2_staticBody);
	hole1 = App->physics->CreateChain(0, 0, Hole1, 12);
	hole1->body->SetType(b2_staticBody);
	hole2 = App->physics->CreateChain(0, 0, Hole2, 10);
	hole2->body->SetType(b2_staticBody);
	hole3 = App->physics->CreateChain(0, 0, Hole3, 14);
	hole3->body->SetType(b2_staticBody);
	hole4 = App->physics->CreateChain(0, 0, Hole4, 12);
	hole4->body->SetType(b2_staticBody);
	light1 = App->physics->CreateChain(0, 0, Light1, 12);
	light1->body->SetType(b2_staticBody);
	light2 = App->physics->CreateChain(0, 0, Light2, 10);
	light2->body->SetType(b2_staticBody);
	light3 = App->physics->CreateChain(0, 0, Light3, 10);
	light3->body->SetType(b2_staticBody);
	light4 = App->physics->CreateChain(0, 0, Light4, 8);
	light4->body->SetType(b2_staticBody);
	light5 = App->physics->CreateChain(0, 0, Light5, 12);
	light5->body->SetType(b2_staticBody);
	light6 = App->physics->CreateChain(0, 0, Light6, 12);
	light6->body->SetType(b2_staticBody);
	light7 = App->physics->CreateChain(0, 0, Light7, 12);
	light7->body->SetType(b2_staticBody);
	light8 = App->physics->CreateChain(0, 0, Light8, 10);
	light8->body->SetType(b2_staticBody);
	light9 = App->physics->CreateChain(0, 0, Light9, 12);
	light9->body->SetType(b2_staticBody);
	light10 = App->physics->CreateChain(0, 0, Light10, 12);
	light10->body->SetType(b2_staticBody);




	//FLIPPERS
	//LEFT
	leftFlipper = App->physics->CreateRectangle(182, 727, 80, 20);
	rightFlipper = App->physics->CreateRectangle(296, 727, 80, 20);


	//JOINTS-------------------------------------------------------------
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

	death = App->physics->CreateRectangleSensor(0, 790, SCREEN_WIDTH * 2, 1);
	return true;
}