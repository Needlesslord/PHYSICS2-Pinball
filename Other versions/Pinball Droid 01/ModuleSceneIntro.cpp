#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ChainListElements.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Handouts -----------------
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//----------------------------------

	//The Game --------------
	assets = App->textures->Load("pinball/assets.png");

	//plunger
	plungerlight.PushBack({ 1010, 731, 57, 63 });					//Needs coords
	plunger_fx = App->audio->LoadFx("pinball/plunger.wav");
	//flippers
	flipper_fx = App->audio->LoadFx("pinball/flipper.wav");



	App->physics->CreatePinballWalls();

	//--------------------------

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	App->textures->Unload(assets);

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
	if (!death)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) {
			left_flipper->body->ApplyAngularImpulse(-2.0F, true);
			left_up_flipper->body->ApplyAngularImpulse(-2.0F, true);

		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			App->audio->PlayFx(flipper_fx);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)) {

			right_flipper->body->ApplyAngularImpulse(2.0F, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) == KEY_REPEAT)
		{
			App->renderer->Blit(assets, 253, 256, &(plungerlight.GetCurrentFrame()));
			plunger_joint->SetMotorSpeed((0.0F, 1.0F));
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			plunger_joint->SetMotorSpeed((0.0F, -40.0F));
			App->audio->PlayFx(plunger_fx);
		}
	}

	return UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	//Handouts --------------------------------------------------
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	
		//------------------------

		//The Game --------------------------

		if (plunger != NULL)
		{
			int x, y;
			plunger->GetPosition(x, y);
			SDL_Rect rect = { 96,83,19,80 };
			App->renderer->Blit(assets, x + 412, y + 651, &rect, 1.0f);			//Need coords
		}




		//--------------------------
	
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

bool ModuleSceneIntro::LoadMap()
{
	//BALL


	//-----------------------

	//MAP--------------------

	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(0.0f, 1.0f);

	//plunger
	plunger = App->physics->CreateChain(0, 0, plungerchain, 10);
	jointDef.Initialize(App->physics->ground, plunger->body, { 422, 742 }, worldAxis);
	plunger_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&jointDef);




	//------------------------------
}

