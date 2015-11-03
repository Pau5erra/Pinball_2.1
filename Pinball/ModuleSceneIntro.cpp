#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_fall = false;
	ball_up = false;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	

	
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	pinball_empty = App->textures->Load("pinball/pinball_empty.png");
	//flippers = App->textures->Load("pinball/flippers.png");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, 881, SCREEN_WIDTH, 50, 0.0f);
	
	sensor->listener = this;

	SensorLeftTriangle = App->physics->CreateRectangleSensor(161, 580, 110, 16, -90.0f);
	SensorLeftTriangle->listener = this;

	SensorRightTriangle = App->physics->CreateRectangleSensor(404, 580, 110, 16, 90.0f);
	SensorRightTriangle->listener = this;
	

	/*
	//definim una caixa qualsevol
	caixa = App->physics->CreateRectangle(300,400,125,25);
	//ara crear revolute join -> enganxar en cordenades de body que parteix des del centre i posem posicio a la esquina que es la meitat de width dividit per la meitat de heihgt
	//segon dir a quin punt de lescenari en el anchor nomes si estas utilitzant una chain pk en aquest cas les chain ocupen tot l'escenari
	App->physics->CreateRevoluteJoin(62, 12, 300, 400, caixa, App->physics->ground1);
	*/
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
		App->renderer->Blit(pinball_empty, 0, 0);
		App->renderer->Blit(flippers,0,0);
	/*if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		caixa->Force(caixa->body, 32 * DEGTORAD);
		LOG("aplicant força");
	}
	*/

	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);


	//Control of Lifes
	if (ball_fall){

		App->player->ball->SetPosition(544, 565, 0.0f);
		App->player->lifes -= 1;
		if (App->player->lifes == 2){
			pinball_empty = App->textures->Load("pinball/pinball_empty_2life.png");
		}
		else if (App->player->lifes == 1){
			pinball_empty = App->textures->Load("pinball/pinball_empty_1life.png");
		}
		else if (App->player->lifes == 0){
			pinball_empty = App->textures->Load("pinball/pinball_empty_0life.png");
		}
		else if (App->player->lifes == -1){
			pinball_empty = App->textures->Load("pinball/pinball_empty.png");
			App->player->lifes = 3;
		}
		ball_fall = false;
	}

	if (ball_up){

		App->player->ball->getNewVelocity();
		App->player->ball->setGravity();
		App->player->ball->setPush(0.0f, -10000.0f);
		ball_up = false;
	}

	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == sensor || bodyB == sensor){
	
		ball_fall = true;
	}

	if (bodyA == SensorLeftTriangle || bodyB == SensorLeftTriangle){
		ball_up = true;
	}

	if (bodyA == SensorRightTriangle || bodyB == SensorRightTriangle){
		ball_up = true;
	}
}
