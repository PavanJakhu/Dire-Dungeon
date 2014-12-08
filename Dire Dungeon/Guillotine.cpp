#include "Guillotine.h"
#include <Direngine\ResourceManager.h>
#include <vector>

//*ORDER OF BODIES*
//0 is anchor
//1 to NUM_SEGMENTS - 1 are rope pieces
//NUM_SEGMENTS is blade
//NUM_SEGMENTS + 1 to NUM_SEGMENTS + 4 are guards

Guillotine::Guillotine(b2Vec2 _position, GameWorld* _world, GLTexture _ropeTexture, GLTexture _bladeTexture)
	: GameObject(_world), ropeTexture(_ropeTexture), bladeTexture(_bladeTexture),
	NUM_SEGMENTS(4), NUM_JOINTS(NUM_SEGMENTS - 1), NUM_ROPE_JOINTS(NUM_SEGMENTS - 1),
	ROPE_WIDTH(0.1f), ROPE_HEIGHT(0.5f), BLADE_WIDTH(1.0f), BLADE_HEIGHT(0.5f),
	anchorPos(_position)
{
	ResourceManager* resources = ResourceManager::GetInstance(); 
	guillotineSound = resources->Load<Sound>("Sounds/SFX/guillotine.wav");
	guardTexture = resources->Load<GLTexture>("Textures/Character/WHITE-BOX.png");
	bodies.resize(NUM_SEGMENTS);
	joints.resize(NUM_JOINTS);
	ropeJoints.resize(NUM_ROPE_JOINTS);
	hasFallen = false;
	triggerTime = 0.0f;

	//Anchor
	b2BodyDef anchorBodyDef;
	anchorBodyDef.type = b2_staticBody;
	anchorBodyDef.position = anchorPos;

	b2PolygonShape anchorShape;
	anchorShape.SetAsBox(0.5f, 0.1f);

	bodies[0] = _world->GetB2World()->CreateBody(&anchorBodyDef);
	bodies[0]->CreateFixture(&anchorShape, 2.0f);

	ChangeFilters(bodies[0], 1);

	//Rope
	b2BodyDef ropeBodyDef;
	ropeBodyDef.type = b2_dynamicBody;

	b2PolygonShape ropeShape;
	ropeShape.SetAsBox(ROPE_WIDTH / 2, ROPE_HEIGHT / 2);

	for (int i = 1; i < NUM_SEGMENTS; i++)
	{
		ropeBodyDef.position.Set(GetBody(i - 1)->GetPosition().x, GetBody(i - 1)->GetPosition().y - ROPE_HEIGHT / 2.0f);
		bodies[i] = _world->GetB2World()->CreateBody(&ropeBodyDef);
		GetBody(i)->CreateFixture(&ropeShape, 0.1f);
		GetBody(i)->SetUserData((void*)-4);
	}

	//Joints
	b2RevoluteJointDef jointDef;

	jointDef.localAnchorA.y = -0.1f;
	jointDef.localAnchorB.y = ROPE_HEIGHT / 2;
	jointDef.bodyA = bodies[0];
	jointDef.bodyB = bodies[1];
	joints[0] = (b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&jointDef);

	jointDef.localAnchorA.y = -ROPE_HEIGHT / 2;
	jointDef.localAnchorB.y = ROPE_HEIGHT / 2;
	jointDef.collideConnected = false;

	for (int i = 1; i < NUM_JOINTS; i++)
	{
		jointDef.bodyA = bodies[i];
		jointDef.bodyB = bodies[i + 1];
		joints[i] = (b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&jointDef);
	}

	b2RopeJointDef ropeDef;

	ropeDef.localAnchorA.Set(0.0f, -0.1f);
	ropeDef.localAnchorB.Set(0.0f, ROPE_HEIGHT / 2);
	ropeDef.maxLength = ROPE_HEIGHT;
	ropeDef.collideConnected = false;
	ropeDef.bodyA = bodies[0];
	ropeDef.bodyB = bodies[1];
	ropeJoints[0] = (b2RopeJoint*)_world->GetB2World()->CreateJoint(&ropeDef);

	ropeDef.localAnchorA.Set(0.0f, -ROPE_HEIGHT / 2);
	ropeDef.localAnchorB.Set(0.0f, ROPE_HEIGHT / 2);
	ropeDef.maxLength = ROPE_HEIGHT;
	ropeDef.collideConnected = false;

	for (int i = 1; i < NUM_ROPE_JOINTS; i++)
	{
		ropeDef.bodyA = bodies[i];
		ropeDef.bodyB = bodies[i + 1];
		ropeJoints[i] = (b2RopeJoint*)_world->GetB2World()->CreateJoint(&ropeDef);
	}

	//Blade
	b2BodyDef bladeBodyDef;
	bladeBodyDef.position.Set(bodies[bodies.size() - 1]->GetPosition().x, bodies[bodies.size() - 1]->GetPosition().y - ROPE_HEIGHT);
	bladeBodyDef.type = b2_dynamicBody;
	bodies.push_back(_world->GetB2World()->CreateBody(&bladeBodyDef));
	GetBody(bodies.size() - 1)->SetUserData((void*)-4);

	b2Vec2 vertices[4];
	vertices[0].Set(-BLADE_WIDTH / 2.0f, BLADE_HEIGHT / 2.0f);
	vertices[1].Set(-BLADE_WIDTH / 2.0f, -BLADE_HEIGHT / 2.0f);
	vertices[2].Set(BLADE_WIDTH / 2.0f, 0.0f);
	vertices[3].Set(BLADE_WIDTH / 2.0f, BLADE_HEIGHT / 2.0f);

	b2PolygonShape bladeShape;
	bladeShape.Set(vertices, 4);
	GetBody()->SetUserData((void*)-4);
	bodies[bodies.size() - 1]->CreateFixture(&bladeShape, 0.05f);

	b2RevoluteJointDef jointDefBlade;
	jointDefBlade.bodyA = bodies[NUM_SEGMENTS - 1]; //Last rope.
	jointDefBlade.bodyB = bodies[NUM_SEGMENTS]; //Blade
	jointDefBlade.collideConnected = false;
	jointDefBlade.localAnchorA.y = -ROPE_HEIGHT / 2;
	jointDefBlade.localAnchorB.y = BLADE_HEIGHT / 2.0f;
	b2RevoluteJoint* joint = (b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&jointDefBlade);

	
	//Guards
	/*b2BodyDef boxGuardBodyDef;
	b2PolygonShape boxGuardShape;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			boxGuardBodyDef.position.Set((-ROPE_WIDTH - 0.3f) + 0.7f * j, (anchorPos.y - 0.5f) - 0.8f * i);
			bodies.push_back(_world->GetB2World()->CreateBody(&boxGuardBodyDef));
			boxGuardShape.SetAsBox(0.1f, 0.05f);
			bodies[bodies.size() - 1]->CreateFixture(&boxGuardShape, 0.0f);
			ChangeFilters(bodies[bodies.size() - 1], 1);
		}
	}*/
}


Guillotine::~Guillotine()
{
}

void Guillotine::Draw(SpriteBatch& _spriteBatch)
{
	Color color;
	glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
	b2Vec2 positionMetres;
	float widthMetres, heightMetres;

	//Anchor
	positionMetres = GetBody(0)->GetPosition();
	widthMetres = 1.0f;
	heightMetres = 0.2f;
	glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
	_spriteBatch.Draw(destRect, uv, guardTexture.id, 0.0F, GetBody(0)->GetAngle(), color);

	//Rope
	widthMetres = ROPE_WIDTH; 
	heightMetres = ROPE_HEIGHT;
	for (int i = 1; i < NUM_SEGMENTS; i++)
	{
		positionMetres = GetBody(i)->GetPosition();
		glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
		_spriteBatch.Draw(destRect, uv, ropeTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
	}

	//Blade
	positionMetres = GetBody(NUM_SEGMENTS)->GetPosition();
	widthMetres = BLADE_WIDTH;
	heightMetres = BLADE_HEIGHT;
	destRect.x = positionMetres.x * 100;
	destRect.y = positionMetres.y * 100;
	destRect.z = widthMetres * 100; 
	destRect.w = heightMetres * 100;
	
	_spriteBatch.Draw(destRect, uv, bladeTexture.id, 0.0f, GetBody(NUM_SEGMENTS)->GetAngle(), color);

	//Guards
	/*widthMetres = 0.4f;
	heightMetres = 0.2f;
	for (int i = NUM_SEGMENTS + 1; i < bodies.size(); i++)
	{
		positionMetres = GetBody(i)->GetPosition();
		glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);

		_spriteBatch.Draw(destRect, uv, guardTexture.id, 0.0f, GetBody(i)->GetAngle(), color);
	}*/
}

void Guillotine::ChangeFilters(b2Body* body, int index)
{
	b2Fixture *fix = body->GetFixtureList();
	b2Filter filter = fix->GetFilterData();
	filter.groupIndex = index;
	fix->SetFilterData(filter);
}

void Guillotine::Fall()
{
	if(!hasFallen)
	{		
		hasFallen = true;
		AudioManager::PlaySound(guillotineSound, 0);
		world->GetB2World()->DestroyJoint(GetJoint(0));
		world->GetB2World()->DestroyJoint(ropeJoints[0]);
	}
}

b2Vec2 Guillotine::ApplyForce(GameTime _time)
{
	return (1 / _time.deltaTime) * GetBody(NUM_SEGMENTS)->GetMass() * GetBody(NUM_SEGMENTS)->GetLinearVelocity();
}