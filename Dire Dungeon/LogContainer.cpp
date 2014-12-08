#include "LogContainer.h"
#include <Direngine\ResourceManager.h>

LogContainer::LogContainer(b2Vec2 _position, GameWorld* _world) : GameObject(_world),
	x(_position.x), y(_position.y), width(1.0f), height(0.1f)
{
	containerTexture = ResourceManager::GetInstance()->Load<GLTexture>("Textures/Traps/rope.png");

	b2BodyDef boxBodyDef[4];
	b2PolygonShape box[4];

	boxBodyDef[0].position.Set(x - height / 2.0f, y);
	box[0].SetAsBox(width / 2.0f, height / 2.0f);

	boxBodyDef[1].position.Set(x - width / 2.0f, y + width / 2.0f - height / 2.0f);
	boxBodyDef[1].angle = 3.14f / 2.0f;
	box[1].SetAsBox(width / 2.0f, height / 2.0f);

	boxBodyDef[2].position.Set(x + width / 4.0f - height / 2.0f, y + width - height);
	box[2].SetAsBox(3.0f * width / 4.0f, height / 2.0f);

	boxBodyDef[3].position.Set(x + 3.0f * width / 4.0f - height, y + width / 2.0f - height);
	boxBodyDef[3].angle = tanf(width / (width / 4.0f));;
	boxBodyDef[3].type = b2_dynamicBody;
	float widthOfDoor = sqrtf((width / 2.0f) * (width / 2.0f) + (width / 4.0f) * (width / 4.0f));
	box[3].SetAsBox(widthOfDoor - height, height / 2.0f);

	for (int i = 0; i < 4; i++)
	{
		bodies.push_back(_world->GetB2World()->CreateBody(&boxBodyDef[i]));
		bodies[i]->CreateFixture(&box[i], 1.0f);
	}

	b2RevoluteJointDef revJointDefOne;
	revJointDefOne.bodyA = bodies[2];
	revJointDefOne.bodyB = bodies[3];
	revJointDefOne.collideConnected = false;
	revJointDefOne.localAnchorA.Set(3.0f * width / 4.0f, -height / 2.0f);
	revJointDefOne.localAnchorB.Set(widthOfDoor - height, 0.0f);
	jointOne = (b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&revJointDefOne);

	b2RevoluteJointDef revJointDefTwo;
	revJointDefTwo.bodyA = bodies[0];
	revJointDefTwo.bodyB = bodies[3];
	revJointDefTwo.collideConnected = false;
	revJointDefTwo.localAnchorA.Set(width / 2.0f, 0.0f);
	revJointDefTwo.localAnchorB.Set(-widthOfDoor + height, 0.0f);
	revJointDefTwo.enableMotor = true;
	revJointDefTwo.motorSpeed = -360.0f * 3.14f / 180.0f;
	revJointDefTwo.maxMotorTorque = 20.0f;
	b2Joint* jointTwo = (b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&revJointDefTwo);
}


LogContainer::~LogContainer(void)
{
}

void LogContainer::Draw(SpriteBatch& _spriteBatch)
{
	for (int i = 0; i < bodies.size() - 4; i++)
	{
		b2Vec2 positionMetres = GetBody(i)->GetPosition();
		glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, width * 100, height * 100);
		if (i == 2) { destRect.z = (width + width / 2.0f) * 100; }
		glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
		Color color;

		_spriteBatch.Draw(destRect, uv, containerTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
	}
}