#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"


#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* big_ball = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	big_ball->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size/*, int width, int height*/)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	pbody->width = width;
	pbody->height = height;


	return pbody;
}

PhysBody* ModulePhysics::CreateChainSensor(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}


// 
update_status ModulePhysics::PostUpdate()
{
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
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	// TODO 4: If the player releases the mouse button, destroy the joint

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

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


//void ModulePhysics::CreatePinballWalls()
//{
//	int Background[180] = {
//		309, 797,
//		475, 666,
//		475, 518,
//		428, 480,
//		429, 444,
//		400, 393,
//		400, 304,
//		427, 273,
//		425, 270,
//		428, 262,
//		426, 255,
//		424, 249,
//		416, 245,
//		403, 248,
//		398, 245,
//		429, 203,
//		429, 66,
//		417, 54,
//		406, 48,
//		392, 41,
//		373, 60,
//		372, 81,
//		367, 83,
//		366, 73,
//		365, 58,
//		390, 36,
//		403, 42,
//		414, 47,
//		428, 57,
//		434, 65,
//		430, 436,
//		476, 436,
//		476, 62,
//		462, 36,
//		447, 21,
//		428, 10,
//		414, 4,
//		403, 2,
//		312, 2,
//		296, 7,
//		279, 15,
//		267, 23,
//		257, 32,
//		254, 42,
//		253, 78,
//		248, 80,
//		222, 55,
//		222, 33,
//		221, 33,
//		219, 24,
//		212, 16,
//		205, 14,
//		198, 16,
//		191, 21,
//		189, 31,
//		187, 31,
//		186, 58,
//		165, 58,
//		158, 43,
//		149, 30,
//		135, 19,
//		119, 9,
//		100, 2,
//		86, 2,
//		71, 2,
//		48, 10,
//		29, 22,
//		17, 32,
//		11, 40,
//		4, 51,
//		0, 63,
//		0, 320,
//		1, 357,
//		11, 386,
//		14, 397,
//		34, 435,
//		59, 462,
//		59, 476,
//		1, 519,
//		0, 673,
//		0, 737,
//		18, 737,
//		25, 736,
//		30, 731,
//		34, 727,
//		36, 722,
//		36, 717,
//		39, 717,
//		39, 695,
//		170, 797
//	};
//	App->scene_intro->Background = CreateChain(0, 0, Background, 180, b2BodyType::b2_staticBody);
//
//	int BackgroundL[14] = {
//	90, 517,
//	94, 523,
//	154, 651,
//	153, 657,
//	91, 612,
//	85, 603,
//	85, 518
//	};
//	App->scene_intro->BackgroundL = CreateChain(0, 0, BackgroundL, 14, b2BodyType::b2_staticBody);
//
//	int BackgroundR[18] = {
//	318, 655,
//	382, 518,
//	386, 516,
//	390, 519,
//	389, 596,
//	386, 607,
//	379, 615,
//	323, 657,
//	320, 657
//	};
//	App->scene_intro->BackgroundR = CreateChain(0, 0, BackgroundR, 18, b2BodyType::b2_staticBody);
//
//	int ChainL[16] = {
//	48, 538,
//	48, 597,
//	60, 632,
//	154, 703,
//	151, 708,
//	55, 634,
//	44, 601,
//	44, 538
//	};
//	App->scene_intro->ChainL = CreateChain(0, 0, ChainL, 16, b2BodyType::b2_staticBody);
//
//	int ChainR[22] = {
//	430, 538,
//	435, 538,
//	435, 601,
//	424, 632,
//	419, 638,
//	328, 708,
//	326, 707,
//	326, 703,
//	419, 631,
//	430, 598,
//	430, 543
//	};
//	App->scene_intro->ChainR = CreateChain(0, 0, ChainR, 22, b2BodyType::b2_staticBody);
//
//	int BackgroundU[20] = {
//	38, 348,
//	59, 360,
//	100, 285,
//	42, 263,
//	38, 255,
//	36, 244,
//	40, 234,
//	48, 225,
//	44, 221,
//	32, 234
//	};
//	App->scene_intro->BackgroundU = CreateChain(0, 0, BackgroundU, 20, b2BodyType::b2_staticBody);
//
//	int BackgroundLine[8] = {
//	272, 220,
//	315, 263,
//	317, 260,
//	275, 218
//	};
//	App->scene_intro->BackgroundLine = CreateChain(0, 0, BackgroundLine, 8, b2BodyType::b2_staticBody);
//
//	int PivoteUR[42] = {
//		287, 170,
//		292, 170,
//		299, 166,
//		304, 162,
//		306, 157,
//		303, 155,
//		302, 150,
//		309, 144,
//		311, 135,
//		304, 125,
//		294, 120,
//		284, 119,
//		273, 121,
//		267, 126,
//		262, 134,
//		264, 144,
//		270, 149,
//		267, 156,
//		270, 162,
//		276, 168,
//		282, 169
//	};
//
//	App->scene_intro->pivote = CreateChain(0, 0, PivoteUR, 42, b2BodyType::b2_staticBody);
//
//	int PivoteUL[40] = {
//		363, 169,
//		369, 169,
//		380, 161,
//		380, 155,
//		378, 155,
//		377, 152,
//		387, 144,
//		387, 132,
//		377, 122,
//		368, 119,
//		357, 119,
//		346, 123,
//		339, 131,
//		337, 138,
//		338, 143,
//		347, 151,
//		344, 155,
//		346, 164,
//		350, 167,
//		356, 169
//	};
//
//	App->scene_intro->pivote2 = CreateChain(0, 0, PivoteUL, 40, b2BodyType::b2_staticBody);
//
//	int PivoteU[46] = {
//		88, 100,
//		92, 100,
//		97, 99,
//		103, 91,
//		103, 85,
//		100, 84,
//		100, 82,
//		108, 76,
//		110, 70,
//		109, 63,
//		103, 55,
//		93, 50,
//		82, 48,
//		68, 53,
//		61, 60,
//		60, 66,
//		60, 73,
//		69, 81,
//		69, 83,
//		67, 85,
//		67, 92,
//		72, 98,
//		79, 100
//	};
//	App->scene_intro->pivote3 = CreateChain(0, 0, PivoteU, 46, b2BodyType::b2_staticBody);
//
//	int PivoteL[54] = {
//		316, 189,
//		310, 189,
//		305, 191,
//		300, 193,
//		295, 196,
//		291, 200,
//		289, 206,
//		291, 212,
//		294, 216,
//		300, 220,
//		297, 224,
//		297, 229,
//		300, 233,
//		307, 238,
//		312, 239,
//		317, 239,
//		323, 238,
//		328, 235,
//		331, 230,
//		331, 225,
//		328, 219,
//		333, 217,
//		337, 212,
//		338, 205,
//		335, 199,
//		330, 194,
//		321, 190
//	};
//	App->scene_intro->pivote4 = CreateChain(0, 0, PivoteL, 54, b2BodyType::b2_staticBody);
//
//	int BouncyL[8] = {
//		93, 522,
//	96, 523,
//	154, 646,
//	152, 648
//	};
//	App->scene_intro->BouncyL = CreateChain(0, 0, BouncyL, 8, b2BodyType::b2_staticBody);
//
//	int BouncyR[8] = {
//		380, 522,
//	377, 522,
//	319, 646,
//	320, 649
//	};
//	App->scene_intro->BouncyR = CreateChain(0, 0, BouncyR, 8, b2BodyType::b2_staticBody);
//
//	int BarraL[12] = {
//	288, 56,
//	286, 61,
//	286, 81,
//	289, 84,
//	293, 81,
//	293, 60
//	};
//	App->scene_intro->BarraL = CreateChain(0, 0, BarraL, 12, b2BodyType::b2_staticBody);
//
//	int BarraR[12] = {
//	329, 57,
//	326, 58,
//	326, 82,
//	329, 84,
//	332, 81,
//	332, 58
//	};
//	App->scene_intro->BarraR = CreateChain(0, 0, BarraR, 12, b2BodyType::b2_staticBody);
//#ifdef _DEBUG //We don't know why these numbers change when we are in release and debug, so we have diferents colliders in debug/release
//	int Hole1[12] = {
//		195, 17,
//		202, 14,
//		211, 15,
//		217, 21,
//		189, 29,
//		191, 22
//	};
//	App->scene_intro->Hole1 = CreateChain(0, 0, Hole1, 12, b2BodyType::b2_staticBody);
//
//	int Hole2[10] = {
//		400, 55,
//		388, 60,
//		414, 67,
//		412, 62,
//		406, 57
//	};
//	App->scene_intro->Hole2 = CreateChain(0, 0, Hole2, 10, b2BodyType::b2_staticBody);
//
//	int Hole3[14] = {
//		406, 247,
//		427, 266,
//		428, 260,
//		427, 253,
//		424, 249,
//		417, 245,
//		411, 245
//	};
//	App->scene_intro->Hole3 = CreateChain(0, 0, Hole3, 14, b2BodyType::b2_staticBody);
//
//	int Hole4[12] = {
//		36, 719,
//		10, 726,
//		14, 730,
//		21, 733,
//		28, 731,
//		34, 727
//	};
//	App->scene_intro->Hole4 = CreateChain(0, 0, Hole4, 12, b2BodyType::b2_staticBody);
//#else //We don't know why these numbers change when we are in release and debug, so we have diferents colliders in debug/release
//
//	App->scene_intro->Hole1 = CreateRectangle(205, 15, 11, 1);
//	App->scene_intro->Hole2 = CreateRectangle(399, 56, 1, 1);
//	App->scene_intro->Hole3 = CreateRectangle(422, 250, 1, 1);
//	App->scene_intro->Hole4 = CreateRectangle(16, 736, 1, 1);
//
//#endif //We don't know why these numbers change when we are in release and debug, so we have diferents colliders in debug/release
//
//
//
//	int Light1[12] = {
//		168, 62,
//	185, 62,
//	185, 67,
//	182, 71,
//	172, 71,
//	168, 67
//	};
//	App->scene_intro->Light1 = CreateChain(0, 0, Light1, 12, b2BodyType::b2_staticBody);
//
//	int Light2[10] = {
//		68, 355,
//	76, 340,
//	81, 344,
//	81, 353,
//	73, 357
//	};
//	App->scene_intro->Light2 = CreateChain(0, 0, Light2, 10, b2BodyType::b2_staticBody);
//
//	int Light3[10] = {
//		78, 337,
//	87, 322,
//	92, 328,
//	91, 335,
//	86, 339
//	};
//	App->scene_intro->Light3 = CreateChain(0, 0, Light3, 10, b2BodyType::b2_staticBody);
//
//	int Light4[8] = {
//	88, 319,
//	97, 304,
//	103, 313,
//	98, 322
//	};
//	App->scene_intro->Light4 = CreateChain(0, 0, Light4, 8, b2BodyType::b2_staticBody);
//
//	int Light5[12] = {
//		270, 223,
//	282, 236,
//	278, 239,
//	272, 239,
//	266, 233,
//	266, 226
//	};
//	App->scene_intro->Light5 = CreateChain(0, 0, Light5, 12, b2BodyType::b2_staticBody);
//
//	int Light6[12] = {
//		285, 238,
//	297, 251,
//	293, 254,
//	287, 254,
//	281, 247,
//	281, 241
//	};
//	App->scene_intro->Light6 = CreateChain(0, 0, Light6, 12, b2BodyType::b2_staticBody);
//
//	int Light7[12] = {
//		300, 253,
//	313, 266,
//	306, 270,
//	302, 269,
//	296, 263,
//	296, 256
//	};
//	App->scene_intro->Light7 = CreateChain(0, 0, Light7, 12, b2BodyType::b2_staticBody);
//
//	int Light8[10] = {
//		400, 394,
//	407, 410,
//	399, 411,
//	393, 403,
//	394, 396
//	};
//	App->scene_intro->Light8 = CreateChain(0, 0, Light8, 10, b2BodyType::b2_staticBody);
//
//	int Light9[12] = {
//		409, 412,
//	418, 428,
//	412, 429,
//	406, 428,
//	403, 420,
//	405, 415
//	};
//	App->scene_intro->Light9 = CreateChain(0, 0, Light9, 12, b2BodyType::b2_staticBody);
//
//	int Light10[12] = {
//		419, 430,
//	427, 445,
//	422, 448,
//	416, 446,
//	413, 439,
//	414, 432
//	};
//	App->scene_intro->Light10 = CreateChain(0, 0, Light10, 12, b2BodyType::b2_staticBody);
//
//
//
//}
