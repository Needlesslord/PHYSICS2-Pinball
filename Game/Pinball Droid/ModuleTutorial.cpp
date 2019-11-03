#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTutorial.h"
#include "SDL\include\SDL.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"



ModuleTutorial::ModuleTutorial(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Tutorial.PushBack({ 0, 0, 477, 798 });
	Tutorial.loop = false;
}


ModuleTutorial::~ModuleTutorial()
{}


bool ModuleTutorial::Start()
{
	LOG("Loading space intro");
	tuto = App->textures->Load("pinball/Tutorial.png");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	Tutorial.Reset();

	return true;
}

// UnLoad assets
bool ModuleTutorial::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(tuto);
	Tutorial.Reset();

	return true;
}


update_status ModuleTutorial::Update()
{
	SDL_RenderClear(App->renderer->renderer);
	anim = &Tutorial;
	App->renderer->Blit(tuto, 0, 0, &(anim->GetCurrentFrame()));
	if (((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)))	
	{
		App->physics->Enable();
		App->player->Enable();
		App->scene_intro->Start();
	}
	return UPDATE_CONTINUE;
}