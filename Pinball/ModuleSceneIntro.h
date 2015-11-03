#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;
class b2PrismaticJoint;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	
	p2List<PhysBody*> boxes;

	
	//creem un punter body que apuntara al objecte caixa
	PhysBody* caixa;
	PhysBody* BoxUp;
	PhysBody* StaticBox;
	

	PhysBody* sensor;
	//Two sensors of the two triangles
	PhysBody* SensorRightTriangle;
	PhysBody* SensorLeftTriangle;

	//Sensors of score
	PhysBody* ScoreSensor1;
	PhysBody* ScoreSensor2;
	PhysBody* ScoreSensor3;
	PhysBody* ScoreSensor4;

	PhysBody* ScoreBestSensor;

	//To check collisions
	bool sensed;
	bool ball_fall;
	bool ball_up;
	bool ball_scored;
	bool ball_best_score;
	
	SDL_Texture* pinball_empty;
	SDL_Texture* flippers;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
