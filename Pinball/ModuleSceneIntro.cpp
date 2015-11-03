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
	ball_scored = false;
	ball_best_score = false;
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

	//Sensor for ground
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, 881, SCREEN_WIDTH, 50, 0.0f);
	
	sensor->listener = this;

	//Sensor for structure triangles
	SensorLeftTriangle = App->physics->CreateRectangleSensor(161, 580, 110, 16, -90.0f);
	SensorLeftTriangle->listener = this;

	SensorRightTriangle = App->physics->CreateRectangleSensor(404, 580, 110, 16, 90.0f);
	SensorRightTriangle->listener = this;
	
	//Score Sensors
	ScoreSensor1 = App->physics->CreateRectangleSensor(154, 242, 35, 10, 0.0f);
	ScoreSensor1->listener = this;
	ScoreSensor2 = App->physics->CreateRectangleSensor(204, 242, 35, 10, 0.0f);
	ScoreSensor1->listener = this;
	ScoreSensor3 = App->physics->CreateRectangleSensor(368, 242, 35, 10, 0.0f);
	ScoreSensor1->listener = this;
	ScoreSensor4 = App->physics->CreateRectangleSensor(417, 242, 35, 10, 0.0f);
	ScoreSensor1->listener = this;

	ScoreBestSensor = App->physics->CreateRectangleSensor(284, 222, 45, 10, 0.0f);
	ScoreBestSensor->listener = this;

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
		App->renderer->Blit(App->player->flipper_left_texture, 150, 670, NULL, 1.0f, App->player->FLeft->GetRotation(), 32, 30);
		App->renderer->Blit(App->player->flipper_right_texture, 290, 660, NULL, 1.0f, App->player->FRight->GetRotation(), 91, 41);
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
			App->player->score = 0;
		}
		ball_fall = false;
	}

	if (ball_up){

		App->player->ball->getNewVelocity();
		App->player->ball->setGravity();
		App->player->ball->setPush(0.0f, -10000.0f);
		ball_up = false;
	}

	if (ball_scored){
		App->player->score += 50;
		ball_scored = false;
	}

	if (ball_best_score){
		App->player->score += 200;
		ball_best_score = false;
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
	//Check if collision to score
	if (bodyA == ScoreSensor1 || bodyB == ScoreSensor1){
		ball_scored = true;
	}
	if (bodyA == ScoreSensor2 || bodyB == ScoreSensor2){
		ball_scored = true;
	}
	if (bodyA == ScoreSensor3 || bodyB == ScoreSensor3){
		ball_scored = true;
	}
	if (bodyA == ScoreSensor4 || bodyB == ScoreSensor4){
		ball_scored = true;
	}

	if (bodyA == ScoreBestSensor || bodyB == ScoreBestSensor){
		ball_best_score = true;
	}
}
