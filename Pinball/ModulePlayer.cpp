#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	box = NULL;
}

ModulePlayer::~ModulePlayer()
{
	prismatic_joint = NULL;
}

// Load assets
bool ModulePlayer::Start()
{
	box = App->textures->Load("pinball/box.png");

	BoxUp = App->physics->CreateRectangle(544, 613, 37, 19);
	StaticBox = App->physics->CreateStaticRectangle(544, 500, 37, 19);


	prismatic_joint = App->physics->CreatePrismaticJoint(BoxUp, StaticBox);

	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int box_x, box_y;
	BoxUp->GetPosition(box_x, box_y);
	App->renderer->Blit(box, box_x, box_y);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		prismatic_joint->EnableMotor(false);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP){

		prismatic_joint->EnableMotor(true);
	}

	return UPDATE_CONTINUE;
}


