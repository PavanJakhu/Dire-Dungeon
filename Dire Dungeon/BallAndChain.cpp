#include "BallAndChain.h"

//*ORDER OF BODIES*
//0 - Chain
//1 - Ball
//2 - Anchor

BallAndChain::BallAndChain(b2Vec2 _position, GameWorld* _world, GLTexture _ballTexture, GLTexture _chainTexture) : 
	GameObject(_world), ballTexture(_ballTexture), chainTexture(_chainTexture),
	BALL_RADIUS(0.2f), CHAIN_WIDTH(BALL_RADIUS / 2.0f), CHAIN_HEIGHT(1.0f),
	triggered(false), triggerTime(0.0f)
{
	//Chain
	b2BodyDef chainBodyDef;
	chainBodyDef.type = b2_staticBody;
	chainBodyDef.position = _position + b2Vec2(0.0f, CHAIN_HEIGHT / 2.0f);
	bodies.push_back(_world->GetB2World()->CreateBody(&chainBodyDef));
	GetBody(0)->SetUserData((void*)-5);

	b2PolygonShape chainShape;
	chainShape.SetAsBox(CHAIN_WIDTH / 2.0f, CHAIN_HEIGHT / 2.0f);
	GetBody(0)->CreateFixture(&chainShape, 1.0f);

	//Ball
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = _position - b2Vec2(0.0f, CHAIN_HEIGHT);
	bodies.push_back(_world->GetB2World()->CreateBody(&ballBodyDef));
	GetBody(1)->SetUserData((void*)-5);

	b2CircleShape ballShape;
	ballShape.m_radius = BALL_RADIUS;
	GetBody(1)->CreateFixture(&ballShape, 1.0f);

	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = GetBody(0); //Chain
	weldJointDef.bodyB = GetBody(1); //Ball
	weldJointDef.collideConnected = false;
	weldJointDef.localAnchorB.Set(0.0f, CHAIN_HEIGHT / 2.0f);
	joints.push_back((b2WeldJoint*)_world->GetB2World()->CreateJoint(&weldJointDef));

	//Anchor
	b2BodyDef anchorBodyDef;
	anchorBodyDef.type = b2_staticBody;
	anchorBodyDef.position = _position;
	bodies.push_back(_world->GetB2World()->CreateBody(&anchorBodyDef));

	b2PolygonShape anchorShape;
	anchorShape.SetAsBox(0.5f, 0.1f);
	GetBody(2)->CreateFixture(&anchorShape, 0.0f);

	b2RevoluteJointDef revJointDef;
	revJointDef.bodyA = GetBody(0); //Chain
	revJointDef.bodyB = GetBody(2); //Anchor
	revJointDef.collideConnected = false;
	revJointDef.motorSpeed = 10.0f;
	revJointDef.maxMotorTorque = 100.0f;
	revJointDef.enableMotor = false;
	revJointDef.localAnchorA.Set(0.0f, CHAIN_HEIGHT / 2.0f);
	joints.push_back((b2RevoluteJoint*)_world->GetB2World()->CreateJoint(&revJointDef));

	GetBody(0)->SetTransform(_position - b2Vec2(CHAIN_HEIGHT / 2.0f, 0.0f), -1.5f);
	//GetBody(1)->SetTransform(_position + b2Vec2(CHAIN_HEIGHT, 0.0f), 1.5f);
}


BallAndChain::~BallAndChain(void)
{
}

void BallAndChain::Update(GameTime& _gameTime)
{
	if (triggered)
	{
		GetBody(0)->SetType(b2_dynamicBody);

		b2RevoluteJointDef revJointDef;
		revJointDef.bodyA = GetBody(0); //Chain
		revJointDef.bodyB = GetBody(2); //Anchor
		revJointDef.collideConnected = false;
		revJointDef.motorSpeed = -20.0f;
		revJointDef.maxMotorTorque = 10000.0f;
		revJointDef.enableMotor = true;
		revJointDef.localAnchorA.Set(0.0f, CHAIN_HEIGHT / 2.0f);
		joints[joints.size() - 1] = (b2RevoluteJoint*)world->GetB2World()->CreateJoint(&revJointDef);
	}

	if ( !( GetBody(1)->GetLinearVelocity() == b2Vec2_zero) )
		previousVelocity = GetBody(1)->GetLinearVelocity();
}

void BallAndChain::Trigger()
{
	triggered = true;
}

void BallAndChain::Draw(SpriteBatch& _spriteBatch)
{
	Color color;
	glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
	b2Vec2 positionMetres;
	float widthMetres, heightMetres;
	glm::vec4 destRect;

	//Chain
	positionMetres = GetBody(0)->GetPosition();
	widthMetres = CHAIN_WIDTH;
	heightMetres = CHAIN_HEIGHT;
	destRect = glm::vec4(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
	_spriteBatch.Draw(destRect, uv, chainTexture.id, 0.0F, GetBody(0)->GetAngle(), color);

	//Ball
	positionMetres = GetBody(1)->GetPosition();
	widthMetres = 2.0f * BALL_RADIUS; 
	heightMetres = 2.0f * BALL_RADIUS;
	destRect = glm::vec4(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
	_spriteBatch.Draw(destRect, uv, ballTexture.id, 0.0F, GetBody(1)->GetAngle(), color);
}

b2Vec2 BallAndChain::ApplyForce(GameTime _time)
{
	 return 10 * (1 / _time.deltaTime) * GetBody(1)->GetMass() * previousVelocity ;
	//return b2Vec2(3000,40);
}
