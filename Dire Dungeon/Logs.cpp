#include "Logs.h"

Logs::Logs(b2Vec2 _position, GameWorld* _world, GLTexture _texture) : LogContainer(_position, _world), logTexture(_texture),
	radius(0.2f)
{
	std::vector<b2BodyDef> logBodyDef;
	std::vector<b2Body*> logBody;
	std::vector<b2CircleShape> log;
	std::vector<b2FixtureDef> logFixDef;

	isOpen = true;
	triggerTime = 0.0f;

	for (int i = 1, j = 1; true; i += 2)
	{
		if (radius * i >= width)
		{
			if (radius * (j + 2) >= width)
			{
				break;
			}
			else
			{
				i = 1;
				j += 2;
			}
		}

		logBodyDef.push_back(b2BodyDef());
		logBodyDef[logBodyDef.size() - 1].type = b2_dynamicBody;
		logBodyDef[logBodyDef.size() - 1].position.Set((x - width / 2.0f) + radius * i, (y - height / 2.0f) + radius * j);
		bodies.push_back(_world->GetB2World()->CreateBody(&logBodyDef[logBodyDef.size() - 1]));

		log.push_back(b2CircleShape());
		log[log.size() - 1].m_radius = radius;

		logFixDef.push_back(b2FixtureDef());
		logFixDef[logFixDef.size() - 1].shape = &log[log.size() - 1];
		logFixDef[logFixDef.size() - 1].density = 10.0f;
		logFixDef[logFixDef.size() - 1].friction = 0.5f;
		bodies[bodies.size() - 1]->CreateFixture(&logFixDef[logFixDef.size() - 1]);
	}
}


Logs::~Logs(void)
{
}

void Logs::OpenDoor()
{
	if(isOpen)
	{
		isOpen = false;
		world->GetB2World()->DestroyJoint(jointOne);

		for (int i = 4; i < bodies.size(); i++) {
			GetBody(i)->ApplyLinearImpulse(b2Vec2(1.0f, 0.0f), bodies[i]->GetWorldCenter(), true);
			GetBody(i)->SetUserData((void*)-6);
		}
	}
}

void Logs::Draw(SpriteBatch& _spriteBatch)
{
	LogContainer::Draw(_spriteBatch);
	for (int i = 4; i < bodies.size(); i++)
	{
		b2Vec2 positionMetres = GetBody(i)->GetPosition();
		float widthMetres = 2.0f * radius, heightMetres = 2.0f * radius;
		// Scale it to 100 pixels per metre
		glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
		glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
		Color color;
		
		_spriteBatch.Draw(destRect, uv, logTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
	}
}