#include "Pendulum.h"
#include <Direngine\ResourceManager.h>
#include <iostream>

Pendulum::Pendulum(b2Vec2 _position, GameWorld* _world, GLTexture _pendulumTexture) : 
	GameObject(_world), pendulumTexture(_pendulumTexture),
	BLADE_WIDTH(1.0f), BLADE_HEIGHT(0.5f),
	ROD_WIDTH(BLADE_WIDTH / 16.0f), ROD_HEIGHT(1.1f)
{

	previousVelocity = b2Vec2(0,0);
	//ResourceManager* resources = ResourceManager::GetInstance();
	//anchorTexture = resources->Load<GLTexture>("Textures/Character/WHITE-BOX.png");
	triggered = false;
	triggerTime = 0.0f;

	//Anchor
	b2BodyDef anchorBodyDef;
	anchorBodyDef.type = b2_staticBody;
	anchorBodyDef.position = _position;
	bodies.push_back(_world->GetB2World()->CreateBody(&anchorBodyDef));

	b2PolygonShape anchorShape;
	anchorShape.SetAsBox(0.5f, 0.1f);
	GetBody(bodies.size() - 1)->CreateFixture(&anchorShape, 0.0f);

	//Pendulum
	b2BodyDef pendulumBodyDef;
	pendulumBodyDef.type = b2_staticBody;
	//pendulumBodyDef.position = anchorBody->GetPosition() - b2Vec2(0.0f, bladeHeight / 2.0f + rodHeight);
	pendulumBodyDef.angle = 0.0f;
	bodies.push_back(_world->GetB2World()->CreateBody(&pendulumBodyDef));
	GetBody(bodies.size() - 1)->SetUserData((void*)-3);

	b2Vec2 bladeVertices[3][4];

	bladeVertices[0][0] = b2Vec2(-BLADE_WIDTH / 2.0f, BLADE_HEIGHT / 2.0f);
	bladeVertices[0][1] = b2Vec2(-BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 8.0f);
	bladeVertices[0][2] = b2Vec2(-BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 2.0f);
	bladeVertices[0][3] = b2Vec2(-3.0f * BLADE_WIDTH / 8.0f, -BLADE_HEIGHT / 4.0f);
	b2PolygonShape bladeShape1;
	bladeShape1.Set(bladeVertices[0], 4);

	bladeVertices[1][0] = b2Vec2(-BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 8.0f);
	bladeVertices[1][1] = b2Vec2(BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 8.0f);
	bladeVertices[1][2] = b2Vec2(BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 2.0f);
	bladeVertices[1][3] = b2Vec2(-BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 2.0f);
	b2PolygonShape bladeShape2;
	bladeShape2.Set(bladeVertices[1], 4);

	bladeVertices[2][0] = b2Vec2(BLADE_WIDTH / 2.0f, BLADE_HEIGHT / 2.0f);
	bladeVertices[2][1] = b2Vec2(BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 8.0f);
	bladeVertices[2][2] = b2Vec2(BLADE_WIDTH / 6.0f, -BLADE_HEIGHT / 2.0f);
	bladeVertices[2][3] = b2Vec2(3.0f * BLADE_WIDTH / 8.0f, -BLADE_HEIGHT / 4.0f);
	b2PolygonShape bladeShape3;
	bladeShape3.Set(bladeVertices[2], 4);

	b2Vec2 rodVertices[4];
	rodVertices[0] = b2Vec2(-ROD_WIDTH, -BLADE_HEIGHT / 8.0f);
	rodVertices[1] = b2Vec2(ROD_WIDTH, -BLADE_HEIGHT / 8.0f);
	rodVertices[2] = b2Vec2(ROD_WIDTH, BLADE_HEIGHT / 2.0f + ROD_HEIGHT);
	rodVertices[3] = b2Vec2(-ROD_WIDTH, BLADE_HEIGHT / 2.0f + ROD_HEIGHT);
	b2PolygonShape rodShape;
	rodShape.Set(rodVertices, 4);

	b2FixtureDef pendulumFixDef;
	pendulumFixDef.density = 5.0f;
	pendulumFixDef.shape = &bladeShape1;
	GetBody(bodies.size() - 1)->CreateFixture(&pendulumFixDef);
	pendulumFixDef.shape = &bladeShape2;
	GetBody(bodies.size() - 1)->CreateFixture(&pendulumFixDef);
	pendulumFixDef.shape = &bladeShape3;
	GetBody(bodies.size() - 1)->CreateFixture(&pendulumFixDef);
	pendulumFixDef.shape = &rodShape;
	GetBody(bodies.size() - 1)->CreateFixture(&pendulumFixDef);

	//Anchor and Pendulum Joint
	b2RevoluteJointDef revJointDef;
	revJointDef.bodyA = GetBody(1);
	revJointDef.bodyB = GetBody(0);
	revJointDef.collideConnected = false;
	revJointDef.localAnchorA.Set(0.0f, ROD_HEIGHT + (BLADE_HEIGHT / 2.0f));
	revJointDef.lowerAngle = -3.14f / 2.0f;
	revJointDef.upperAngle = 3.14f / 2.0f;
	revJointDef.enableLimit = true;
	joints.push_back((b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&revJointDef));

	GetBody(1)->SetTransform(b2Vec2(_position.x + ROD_HEIGHT, _position.y), 1.5f);
}


Pendulum::~Pendulum(void)
{
}

void Pendulum::Update(GameTime& _gameTime)
{
	if(triggered)
	{
		for (int i = 1; i < bodies.size(); i++)
		{
			GetBody(i)->SetType(b2_dynamicBody);
		}

		if (GetBody(1)->GetLinearVelocity().x == 0.0f && GetBody(1)->GetLinearVelocity().y == 0.0f && floorf(GetBody(1)->GetAngle()) == 0.0f)
		{
			GetBody(1)->ApplyLinearImpulse(b2Vec2(50.0f, 0.0f), GetBody(1)->GetWorldCenter(), true);
		}

		if (GetBody(1)->GetAngle() >= 1.5f)
		{
			GetBody(1)->ApplyLinearImpulse(b2Vec2(0.0f, -20.0f), GetBody(1)->GetWorldCenter(), true);
		}
		else if (GetBody(1)->GetAngle() <= -1.5f)
		{
			GetBody(1)->ApplyLinearImpulse(b2Vec2(0.0f, -20.0f), GetBody(1)->GetWorldCenter(), true);
		}
	}
	if ( !( GetBody(1)->GetLinearVelocity() == b2Vec2_zero) )
		previousVelocity = GetBody(1)->GetLinearVelocity();

}

void Pendulum::Trigger()
{
	triggered = true;
}

void Pendulum::Draw(SpriteBatch& _spriteBatch)
{
	Color color;
	glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
	b2Vec2 positionMetres;
	float widthMetres, heightMetres;

	//Anchor
	/*positionMetres = GetBody(0)->GetPosition();
	widthMetres = 1.0f;
	heightMetres = 0.2f;
	glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
	_spriteBatch.Draw(destRect, uv, anchorTexture.id, 0.0F, GetBody(0)->GetAngle(), color);*/

	//Pendulum
	positionMetres = GetBody(1)->GetPosition() + b2Vec2(0.0f, (BLADE_HEIGHT + (ROD_HEIGHT - (BLADE_HEIGHT / 2.0f) - (BLADE_HEIGHT / 8.0f))) / 2.0f - (BLADE_HEIGHT / 2.0f));
	widthMetres = BLADE_WIDTH; 
	heightMetres = BLADE_HEIGHT + (ROD_HEIGHT - (BLADE_HEIGHT / 2.0f)/* - (BLADE_HEIGHT / 8.0f)*/);
	glm::vec4 destRect;
	destRect.x = positionMetres.x * 100;
	destRect.y = positionMetres.y * 100;
	destRect.z = widthMetres * 100;
	destRect.w = heightMetres * 100;
	_spriteBatch.Draw(destRect, uv, pendulumTexture.id, 0.0F, GetBody(1)->GetAngle(), color);
}

b2Vec2 Pendulum::ApplyForce(GameTime _time)
{
	 return 10 * (1 / _time.deltaTime) * GetBody(1)->GetMass() * previousVelocity ;
	//return b2Vec2(3000,40);
}