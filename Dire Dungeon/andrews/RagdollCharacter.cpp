#include "RagdollCharacter.h"

RagdollCharacter::RagdollCharacter(float scale, b2Vec2 _position, GameWorld* _world, ResourceManager* _resources) : GameObject(_world) {
	//Load the textures
	boxTexture = _resources->LoadTexture("Textures/Character/WHITE-BOX.png");
	headHappyTexture = _resources->LoadTexture("Textures/Character/happy.png");
	headSadTexture = _resources->LoadTexture("Textures/Character/surprise.png");
	headTexture = _resources->LoadTexture("Textures/Character/headCentered.png");

	startTime = 0.0F;
	contacting = false;

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
	fd.density = 10.0f;
	GetBody(0)->CreateFixture(&fd);
	GetBody(0)->SetUserData( this );
	//torso
	CreateBody(1, 0.0f + _position.x, 0.5f/scale + _position.y, 0.07f/scale, 0.55f/scale);
	GetBody(1)->SetUserData( this );
	//Right Leg
	CreateBody(2, 0.25f/scale + _position.x, -0.4f/scale + _position.y, 0.07f/scale, 0.45f/scale, 0.6);
	//Left Leg
	CreateBody(3, -0.25f/scale + _position.x, -0.4f/scale + _position.y, 0.07f/scale, 0.45f/scale, -0.6);
	//Left Arm
	CreateBody(4, -0.25f/scale + _position.x, 0.8f/scale + _position.y, 0.40f/scale, 0.07f/scale);
	//Right Arm
	CreateBody(5, 0.25f/scale + _position.x, 0.8f/scale + _position.y, 0.40f/scale, 0.07f/scale);

	//Create Joints
	//torso and right arm
	CreateJoint(GetBody(1), GetBody(5), 0.07f/scale + _position.x, 0.30f/scale + _position.y, -0.40f/scale + _position.x, 0.0f + _position.y, true, -1.3f, 0.7f);
	////torso and left arm
	CreateJoint(GetBody(1), GetBody(4), -0.07f/scale + _position.x, 0.30f/scale + _position.y, 0.40f/scale + _position.x, 0.0f + _position.y, true, -0.7f, 1.3f);
	//torso and right leg
	CreateJoint(GetBody(1), GetBody(2), 0.07f/scale + _position.x, -0.55f/scale + _position.y, 0.0f + _position.x, 0.45f/scale + _position.y, false);
	//torso and left leg
	CreateJoint(GetBody(1), GetBody(3), -0.07f/scale + _position.x, -0.55f/scale + _position.y, 0.0f + _position.x, 0.45f/scale + _position.y, false);
	//torso and head
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = GetBody(1);
	weldJointDef.bodyB = GetBody(0);
	weldJointDef.localAnchorA.Set(0.0f + _position.x, 0.65f/scale + _position.y);
	weldJointDef.localAnchorB.Set(0.0f + _position.x,-0.05f + _position.y);
	world->GetB2World()->CreateJoint( &weldJointDef );
}

void RagdollCharacter::Update(GameTime& _gameTime) {
	if (contacting)
		startTime = _gameTime.time;

	if (_gameTime.time > startTime + 5.0F) {
		for (int i = 0; i < bodies.size(); i++) {
			GetBody(i)->SetType(b2_dynamicBody);
		}
	}
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
			_spriteBatch.Draw(destRect, uv, headHappyTexture.id, 0.0F, GetBody(i)->GetAngle(), color);
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
	return GetBody(0)->GetLinearVelocity().x;
}

void RagdollCharacter::beginContact()
{
	contacting = true;
}
void RagdollCharacter::endContact()
{
	contacting = false;
}