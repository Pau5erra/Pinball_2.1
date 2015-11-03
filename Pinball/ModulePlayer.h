#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class b2PrismaticJoint;
struct SDL_Texture;

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
	
	PhysBody* ball;


	// Prismatic Joint
	PhysBody* BoxUp;
	PhysBody* StaticBox;
	b2PrismaticJoint* prismatic_joint;

	int lifes;
};