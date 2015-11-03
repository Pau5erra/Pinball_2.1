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
	revolute_joint_left = NULL;
	revolute_joint_right = NULL;
}

// Load assets
bool ModulePlayer::Start()
{
	lifes = 3;
	score = 0;
	ball_texture = App->textures->Load("pinball/wheel1.png");
	box_texture = App->textures->Load("pinball/box.png");
	flipper_left_texture = App->textures->Load("pinball/flipper_left.png");
	flipper_right_texture = App->textures->Load("pinball/flipper_right.png");
	
	//Create Ball
	ball = App->physics->CreateCircle(544, 565, 11);
	ball->listener = this;

	//Create Bounce
	BoxUp = App->physics->CreateRectangle(544, 613, 37, 19);
	StaticBox = App->physics->CreateStaticRectangle(544, 500, 37, 19);


	prismatic_joint = App->physics->CreatePrismaticJoint(BoxUp, StaticBox);

	

	int flipper_left[8] = {
		28, 13,
		106, 79,
		97, 91,
		15, 35
	};

	FLeft = App->physics->CreatePolygon(170, 670, flipper_left, 8, b2_dynamicBody, 2.5f);
	StaticPointLeft = App->physics->CreateStaticCircle(181, 696, 5);
	b2Vec2 FLeft_center = FLeft->body->GetLocalCenter();
	FLeft_center.x -= PIXEL_TO_METERS(25);
	FLeft_center.y -= PIXEL_TO_METERS(25);
	revolute_joint_left = App->physics->CreateRevoluteJoin(StaticPointLeft, FLeft, StaticPointLeft->body->GetLocalCenter(), FLeft_center , true, -70, 0, -150, 300);
	

	int flipper_right[8] = {
		92, 23,
		12, 84,
		21, 97,
		104, 49
	};

	FRight = App->physics->CreatePolygon(300, 670, flipper_right, 8, b2_dynamicBody, 2.5f);
	StaticPointRight = App->physics->CreateStaticCircle(380, 696, 5);
	b2Vec2 FRight_center = FRight->body->GetLocalCenter();
	FRight_center.x += PIXEL_TO_METERS(25);
	FRight_center.y -= PIXEL_TO_METERS(25);
	revolute_joint_right = App->physics->CreateRevoluteJoin(StaticPointRight, FRight, StaticPointRight->body->GetLocalCenter(), FRight_center, true, 0, 70, -150, -300);

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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN){

		revolute_joint_left->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP){

		revolute_joint_left->EnableMotor(false);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN){

		revolute_joint_right->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP){

		revolute_joint_right->EnableMotor(false);
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


