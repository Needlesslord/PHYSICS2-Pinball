#ifndef __MODULETUTORIAL_H__
#define __MODULETUTORIAL_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2Animation.h"

struct SDL_Texture;

class ModuleTutorial : public Module
{
public:
	ModuleTutorial(Application* app, bool start_enabled = true);
	~ModuleTutorial();
	p2Animation* anim = nullptr;
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * tuto = nullptr;
	p2Animation Tutorial;

};

#endif
