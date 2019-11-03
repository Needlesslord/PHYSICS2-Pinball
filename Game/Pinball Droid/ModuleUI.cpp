#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI");
	score_font = App->fonts->Load("pinball/score_font.png" ,"0123456789", 1);
	life_font = App->fonts->Load("pinball/life_font.png", "01234", 1);

	game_over = App->textures->Load("pinball/game_over.png");
	return true;
}

// Unload assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI");
	App->fonts->UnLoad(score_font);
	App->fonts->UnLoad(life_font);

	return true;
}

// Update: draw background
update_status ModuleUI::Update()
{
	//Print Score
	sprintf_s(score_string, 10, "%1d", score_player);

	App->fonts->BlitText(157, 12, score_font, score_string);

	//Print High Score
	sprintf_s(high_score_string, 10, "%1d", high_score);

	App->fonts->BlitText(320, 36, score_font, high_score_string);

	//Print Life
	sprintf_s(life_string, 4, "%1d", App->scene_intro->life);

	App->fonts->BlitText(394, 362, life_font, life_string);
	if (App->scene_intro->death)
	{
		if (timer)
		{
			time_on_entry = SDL_GetTicks();
			timer = false;
		}
		current_time = SDL_GetTicks() - time_on_entry;
		if (current_time > 1500)
		{
			App->scene_intro->death = false;
			timer = true;
		}
		SDL_Rect rect = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		App->renderer->DrawQuad(rect, 255, 255, 255, 100);
		App->renderer->Blit(game_over, 65, 200, NULL);
	}

	

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate()
{
	if (App->scene_intro->life == 0)
	{
		App->scene_intro->death = true;
		if (score_player > high_score)
		{
			high_score = score_player;
		}
		score_player = 0;
		App->scene_intro->life = 3;
	}

	return UPDATE_CONTINUE;

}



