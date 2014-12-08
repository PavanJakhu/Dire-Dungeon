#include "RagdollCharacter.h"

RagdollCharacter::RagdollCharacter(float scale, float _speed, b2Vec2 _position, GameWorld* _world, ResourceManager* _resources) : GameObject(_world){
	//Load the textures
	boxTexture = _resources->Load<GLTexture>("Textures/Character/WHITE-BOX.png");
	headHappyTexture = _resources->Load<GLTexture>("Textures/Character/umer.png");
	headSadTexture = _resources->Load<GLTexture>("Textures/Character/surprise.png");
	headTexture = _resources->Load<GLTexture>("Textures/Character/headCentered.png");

	onHitScream = _resources->Load<Sound>("Sounds/SFX/grunt.ogg");
	
	startTime = 0.0F;
	isContactRagdoll = 0;
	speed = _speed;
	recentCollision = false;

	//Create Bodies
	//Head
	b2BodyDef bd1;
	bd1.position.Set(0.0f, 0.65f);
	bd1.type = b2_staticBody;

	bodies.push_back(world->GetB2World()->CreateBody(&bd1));

	b2CircleShape shape;
	shape.m_radius = 0.15f;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	GetBody(0)->CreateFixture(&fd);
	GetBody(0)->SetUserData( this );
	//torso
	CreateBody(1, 0.0f, 0.5f/scale, 0.07f/scale, 0.55f/scale);
	GetBody(1)->SetUserData( this );
	//Right Leg
	CreateBody(2, 0.25f/scale, -0.4f/scale, 0.07f/scale, 0.45f/scale, 0.6);
	GetBody(2)->SetUserData( this );
	//Left Leg
	CreateBody(3, -0.25f/scale, -0.4f/scale, 0.07f/scale, 0.45f/scale, -0.6);
	GetBody(3)->SetUserData( this );
	//Left Arm
	CreateBody(4, -0.25f/scale, 0.8f/scale, 0.40f/scale, 0.07f/scale);
	GetBody(4)->SetUserData( this );
	//Right Arm
	CreateBody(5, 0.25f/scale, 0.8f/scale, 0.40f/scale, 0.07f/scale);
	GetBody(5)->SetUserData( this );
	//Create Joints
	//torso and right arm
	CreateJoint(GetBody(1), GetBody(5), 0.07f/scale, 0.30f/scale, -0.40f/scale, 0.0f, true, -1.3f, 0.7f);
	////torso and left arm
	CreateJoint(GetBody(1), GetBody(4), -0.07f/scale, 0.30f/scale, 0.40f/scale, 0.0f, true, -0.7f, 1.3f);
	//torso and right leg
	CreateJoint(GetBody(1), GetBody(2), 0.07f/scale, -0.55f/scale, 0.0f, 0.45f/scale, false);
	//torso and left leg
	CreateJoint(GetBody(1), GetBody(3), -0.07f/scale, -0.55f/scale, 0.0f, 0.45f/scale, false);
	//torso and head
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = GetBody(1);
	weldJointDef.bodyB = GetBody(0);
	weldJointDef.localAnchorA.Set(0.0f, 0.65f/scale);
	weldJointDef.localAnchorB.Set(0.0f,-0.05f);
	world->GetB2World()->CreateJoint( &weldJointDef );

	for (int i = 0; i < bodies.size(); i++)
	{
		float initialx = GetBody(i)->GetPosition().x;
		GetBody(i)->SetTransform(GetBody(i)->GetPosition() + _position,GetBody(i)->GetAngle());
		bodyInitialPos.push_back(b2Vec2(initialx, GetBody(i)->GetPosition().y));
	}

	audioPlayed = false;
}

RagdollCharacter::~RagdollCharacter()
{
}

void RagdollCharacter::Update(GameTime& _gameTime) {

	startTime += _gameTime.deltaTime;
	if (isContactRagdoll > 0)
	{
		recentCollision = true;
		startTime = 0;
		for (int i = 0; i < bodies.size(); i++)
		{
			GetBody(i)->SetType(b2_dynamicBody);
		}
	}

	if (isContactRagdoll < 1 && startTime > 3 && recentCollision)
	{
		recentCollision = false;
		startTime = 2;
		for (int i = 0; i < bodies.size(); i++)
		{
			GetBody(i)->SetType(b2_staticBody);
		}

		GetBody(1)->SetTransform(b2Vec2(bodyInitialPos[1].x + GetBody(0)->GetPosition().x, bodyInitialPos[1].y), 0);
		GetBody(2)->SetTransform(b2Vec2(bodyInitialPos[2].x + GetBody(0)->GetPosition().x, bodyInitialPos[2].y), 0.6);
		GetBody(3)->SetTransform(b2Vec2(bodyInitialPos[3].x + GetBody(0)->GetPosition().x, bodyInitialPos[3].y), -0.6);
		GetBody(4)->SetTransform(b2Vec2(bodyInitialPos[4].x + GetBody(0)->GetPosition().x, bodyInitialPos[4].y), 0);
		GetBody(5)->SetTransform(b2Vec2(bodyInitialPos[5].x + GetBody(0)->GetPosition().x, bodyInitialPos[5].y), 0);
		GetBody(0)->SetTransform(b2Vec2(bodyInitialPos[0].x + GetBody(0)->GetPosition().x, bodyInitialPos[0].y), 0);
	}

	moveCharacter(speed);
}

void RagdollCharacter::Draw(SpriteBatch& _spriteBatch) {
	// Loop through and draw the bodies
	for (int i = 0; i < bodies.size(); i++) {
		b2Vec2 positionMetres = GetBody(i)->GetPosition();
		float widthMetres = 0.5F, heightMetres = 0.5F;
		// Scale it to 100 pixels per metre
		if (GetBody(i)->GetFixtureList()->GetShape()->GetType() == b2Shape::e_circle)
		{
			widthMetres = GetBody(i)->GetFixtureList()->GetShape()->m_radius * 2;
			heightMetres = GetBody(i)->GetFixtureList()->GetShape()->m_radius * 2;

			glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
			glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
			Color color;

			_spriteBatch.Draw(destRect, uv, headTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
			if(recentCollision)
			{
				_spriteBatch.Draw(destRect, uv, headSadTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
			}
			else
			{
				_spriteBatch.Draw(destRect, uv, headHappyTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
			}
		}

		else if(GetBody(i)-> GetFixtureList()->GetShape()->GetType() == b2Shape::e_polygon)
		{
			b2PolygonShape* shape = (b2PolygonShape*)GetBody(i)->GetFixtureList()->GetShape();

			widthMetres = shape->GetVertex(0).x * 2;
			heightMetres = shape->GetVertex(0).y * 2;

			glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
			glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
			Color color;

			_spriteBatch.Draw(destRect, uv, boxTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
		}
	}
}

void RagdollCharacter::CreateBody(int bodyCount, float x, float y, float w, float h, float angle)
{
	b2BodyDef bd;
	bd.position.Set(x, y);
	bd.angle = angle;
	bd.type = b2_staticBody;

	bodies.push_back(world->GetB2World()->CreateBody(&bd));

	b2PolygonShape shape;
	shape.SetAsBox(w, h);
	GetBody(bodyCount)->CreateFixture(&shape,10.0f);
}

void RagdollCharacter::CreateJoint(b2Body* bodyA, b2Body* bodyB, float anchorAX, float anchorAY, float anchorBX, float anchorBY, bool limited, float lowerAngle, float upperAngle)
{
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = bodyA;
	revoluteJointDef.bodyB = bodyB;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(anchorAX ,anchorAY);
	revoluteJointDef.localAnchorB.Set(anchorBX ,anchorBY);
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.enableLimit = limited;
	if (limited)
	{
		revoluteJointDef.lowerAngle = lowerAngle;
		revoluteJointDef.upperAngle = upperAngle;
	}
	world->GetB2World()->CreateJoint(&revoluteJointDef);
}

float RagdollCharacter::playerVelocity()
{
	if(GetBody(0)->GetType() == b2_staticBody)
	{
		return speed * 100;
	}
	else if(GetBody(0)->GetType() == b2_dynamicBody)
	{
		return GetBody(0)->GetLinearVelocity().x;
	}
}
void RagdollCharacter::FlipSpeed()
{

	if(GetBody(0)->GetType() == b2_staticBody)
	{
		speed = -speed;
	}
	else if(GetBody(0)->GetType() == b2_dynamicBody)
	{
		GetBody(0)->SetLinearVelocity(b2Vec2(-(GetBody(0)->GetLinearVelocity().x),(GetBody(0)->GetLinearVelocity().y)));
		speed = -speed;
	}
}


b2Vec2 RagdollCharacter::playerPosition()
{
	return GetBody(0)->GetPosition();
}

void RagdollCharacter::startContact()
{
	isContactRagdoll++;
	if (!audioPlayed) {
		AudioManager::PlaySound(onHitScream, 0);
		audioPlayed = true;
	}
}
void RagdollCharacter::endContact()
{
	isContactRagdoll--;
	audioPlayed = false;
}

void RagdollCharacter::moveCharacter(float speed)
{
	if(GetBody(0)->GetType() == b2_staticBody)
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			GetBody(i)->SetTransform(b2Vec2(GetBody(i)->GetPosition().x + speed, GetBody(i)->GetPosition().y),GetBody(i)->GetAngle());
		}
	}
}