#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	virtual ~ModuleUI();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	SDL_Texture* game_over = nullptr;

	int score_font = 0;
	int life_font = 0;
	int score_player = 0;
	int high_score = 0;
	char score_string[10];
	char high_score_string[10];
	char life_string[4];

	bool timer = true;
	int current_time = 0, time_on_entry = 0;

};