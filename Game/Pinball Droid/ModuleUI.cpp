#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"



ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI");
	font = App->fonts->Load("pinball/fonts.png", "0123456789", 1);
	Aux[0] = 0;
	return true;
}

// Unload assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading Ui");

	return true;
}

// Update: draw background
update_status ModuleUI::Update()
{

	sprintf_s(score, 7, "%1d", Score);
	sprintf_s(highscore, 7, "%1d", HighScore);
	sprintf_s(lastscore, 7, "%1d", LastScore);




	if (App->scene_intro->numLives < 0)
	{
		Aux[cont + 1] = Score;
		if (Score > HighScore)
		{
			HighScore = Score;
		}
		LastScore = Aux[cont];
		cont++;
		//Score[cont] = 0;
	}

	return UPDATE_CONTINUE;
}




#if 0;

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
	if (App->scene_intro->alive==false)
	{
		if (timer)
		{
			time_on_entry = SDL_GetTicks();
			timer = false;
		}
		current_time = SDL_GetTicks() - time_on_entry;
		if (current_time > 1500)
		{
			App->scene_intro->alive = true;
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
		App->scene_intro->alive = false;
		if (score_player > high_score)
		{
			high_score = score_player;
		}
		score_player = 0;
		App->scene_intro->life = 3;
	}

	return UPDATE_CONTINUE;

}


#endif;
