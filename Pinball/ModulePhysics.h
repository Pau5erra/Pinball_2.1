#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	void SetPosition(int x, int y , float angle);
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void Force(b2Body* bodyA, int force);

	void setPush(float x, float y);
	void setGravity();
	b2Vec2 getNewVelocity();
	void setNewVelocity( const b2Vec2& new_velocity);


public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateStaticCircle(int x, int y, int radius);
	PhysBody* CreateStaticRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float angle);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreateChainStatic(int x, int y, int* points, int size);
	PhysBody* CreateChainRestitution(int x, int y, int* points, int size);
	
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType bodyType, float density = 1.0f, bool sensor = false);

	//JOINT
	b2RevoluteJoint* CreateRevoluteJoin(PhysBody* bodyA, PhysBody* bodyB,const  b2Vec2& center_1, const b2Vec2 center_2, bool limit, const int lowAngle, const int upAngle, const int motorSpeed, const int maxTorque);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//MOUSE JOINT
	b2Vec2 mouse_position;
	b2Body* body_clicked;

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

public:
	PhysBody* ground1;
};