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
	life_font = App->fonts->Load("pinball/life_font.png", "0123", 1);

	return true;
}

// Unload assets
bool ModuleUI::CleanUp() {
	LOG("Unloading UI");
	App->fonts->UnLoad(score_font);
	App->fonts->UnLoad(life_font);

	return true;
}

// Update: draw background
update_status ModuleUI::Update() {
	//Print Score
	sprintf_s(score_string, 10, "%1d", score_player);
	App->fonts->BlitText(155, 773, score_font, score_string);
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate() {
	return UPDATE_CONTINUE;
}

