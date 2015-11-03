#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class b2PrismaticJoint;
struct SDL_Texture;
class b2RevoluteJoint;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	p2List<PhysBody*> circles;


	SDL_Texture* ball_texture;
	SDL_Texture* box_texture;

	SDL_Texture* flipper_left_texture;
	SDL_Texture* flipper_right_texture;
	
	PhysBody* ball;


	// Prismatic Joint
	PhysBody* BoxUp;
	PhysBody* StaticBox;
	b2PrismaticJoint* prismatic_joint;

	//Revolute Joint
	PhysBody* FLeft;
	PhysBody* StaticPointLeft;
	b2RevoluteJoint* revolute_joint_left;
	

	PhysBody* FRight;
	PhysBody* StaticPointRight;
	b2RevoluteJoint* revolute_joint_right;

	int lifes;
	int score;
};