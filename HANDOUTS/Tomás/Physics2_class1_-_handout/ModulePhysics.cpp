#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <cstdlib>
#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef circleDef;
	circleDef.type = b2_staticBody;
	circleDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* circleBody = world->CreateBody(&circleDef);

	b2CircleShape circleShape;
	circleShape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef circleFixture;
	circleFixture.shape = &circleShape;
	circleBody->CreateFixture(&circleFixture);



	b2BodyDef boxDef;
	boxDef.type = b2_staticBody;
	boxDef.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(SCREEN_HEIGHT * 19 / 20));// X & Y

	b2Body* boxBody = world->CreateBody(&boxDef);

	b2PolygonShape boxShape;
	boxShape.m_vertices->Set(PIXEL_TO_METERS(SCREEN_WIDTH), PIXEL_TO_METERS(SCREEN_HEIGHT * 1 / 20));// W & H

	b2FixtureDef boxFixture;
	boxFixture.shape = &boxShape;
	boxBody->CreateFixture(&boxFixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		int x = App->input->GetMouseX();
		int y = App->input->GetMouseY();
		int v1 = rand() % 100;
		float radius = PIXEL_TO_METERS(v1);	//THE RADIUS IS A RANDOMLY GENERATED VALUE (ASKED IN HOMEWORK)
		
		b2BodyDef body;
		body.type = b2_dynamicBody;
		body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
		b2Body* b = world->CreateBody(&body);
		
		b2CircleShape shape;
		shape.m_radius = radius;

		b2FixtureDef fixture;
		fixture.shape = &shape;
		b->CreateFixture(&fixture);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawQuad({ METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_vertices->x), METERS_TO_PIXELS(shape->m_vertices->y) }, 255, 255, 255);
				}
				break;
				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;
	return true;
}
