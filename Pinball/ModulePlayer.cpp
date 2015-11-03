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
	ball_texture = NULL;
	box_texture = NULL;
	flipper_left_texture = NULL;
	flipper_right_texture = NULL;

}

ModulePlayer::~ModulePlayer()
{
	prismatic_joint = NULL;
	revolute_joint1 = NULL;
	revolute_joint2 = NULL;
}

// Load assets
bool ModulePlayer::Start()
{
	lifes = 3;
	score = 0;
	ball_texture = App->textures->Load("pinball/wheel1.png");
	box_texture = App->textures->Load("pinball/box.png");

	ball = App->physics->CreateCircle(544, 565, 11);
	ball->listener = this;

	BoxUp = App->physics->CreateRectangle(544, 613, 37, 19);
	StaticBox = App->physics->CreateStaticRectangle(544, 500, 37, 19);


	prismatic_joint = App->physics->CreatePrismaticJoint(BoxUp, StaticBox);

	flipper_left_texture = App->textures->Load("pinball/flipper_left.png");
	flipper_right_texture = App->textures->Load("pinball/flipper_right.png");

	int f1[18] = {
		10, 2,
		22, 1,
		90, 62,
		91, 70,
		87, 74,
		76, 73,
		2, 23,
		2, 11,
		10, 2
	};

	FLeft = App->physics->CreateChain(0, 0, f1, 17);
	StaticPointLeft = App->physics->CreateStaticCircle(50, 40, 5);
	revolute_joint1 = App->physics->CreateRevoluteJoin(62, 12, 300, 400, FLeft, StaticPointLeft, 0, 70, -150, 100);
	

	int f2[18] = {
		66, 3,
		82, 1,
		90, 12,
		90, 21,
		14, 74,
		4, 74,
		0, 68,
		0, 61,
		66, 3
	};

	FRight = App->physics->CreateChain(0, 0, f2, 17);
	StaticPointRight = App->physics->CreateStaticCircle(200, 600, 5);
	revolute_joint2 = App->physics->CreateRevoluteJoin(62, 12, 300, 400, FLeft, StaticPointLeft, 0, 70, -150, 100);

	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ball_texture);
	App->textures->Unload(box_texture);
	App->textures->Unload(flipper_left_texture);
	App->textures->Unload(flipper_right_texture);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	App->renderer->Blit(ball_texture, ball_x, ball_y);
	int box_x, box_y;
	BoxUp->GetPosition(box_x, box_y);
	App->renderer->Blit(box_texture, box_x, box_y);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		prismatic_joint->EnableMotor(false);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP){

		prismatic_joint->EnableMotor(true);
	}
	

	//Llista de boles quan apretes 1
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball_texture, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	return UPDATE_CONTINUE;
}


