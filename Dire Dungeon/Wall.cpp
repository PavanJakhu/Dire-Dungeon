#include "Wall.h"

Wall::Wall(b2Vec2 _position, GameWorld* _world, GLTexture _texture, float _width, float _height) : GameObject(_world), texture(_texture)
{
	{
		//SOLID BLOCK ONE
		b2BodyDef bd1;
		if(_height > _width)
		{
			bd1.position.Set(_position.x,_position.y);
		}
		else if (_width > _height)
		{
			bd1.position.Set(_position.x,_position.y);
		}
		bd1.type = b2_staticBody;

		bodies.push_back(world->GetB2World()->CreateBody(&bd1));

		b2PolygonShape shape1;
		if(_height > _width)
		{
			shape1.SetAsBox(_width * 2 / 5, _height);
		}
		else if (_width > _height)
		{
			shape1.SetAsBox(_width, _height * 2 / 5);
		}
		GetBody(0)->CreateFixture(&shape1,1.0f);
	}
	{
		//COLLIDABLE BLOCK
		b2BodyDef bd2;
		if(_height > _width)
		{
			bd2.position.Set(_position.x - (_width * 2 / 5),_position.y);
		}
		else if (_width > _height)
		{
			bd2.position.Set(_position.x,_position.y - (_height * 2 / 5));
		}
		bd2.type = b2_dynamicBody;

		bodies.push_back(world->GetB2World()->CreateBody(&bd2));

		b2PolygonShape shape2;
		if(_height > _width)
		{
			shape2.SetAsBox(_width * 1 / 5, _height);
		}
		else if (_width > _height)
		{
			shape2.SetAsBox(_width, _height * 1 / 5);
		}
		GetBody(1)->SetGravityScale(0);
		GetBody(1)->CreateFixture(&shape2,9999.0f);
		GetBody(1)->SetUserData((void*)-3);
	}

	{
		//SOLID BLOCK TWO
		b2BodyDef bd3;
		if(_height > _width)
		{
			bd3.position.Set(_position.x - (_width * 3 / 5),_position.y);
		}
		else if (_width > _height)
		{
			bd3.position.Set(_position.x,_position.y - (_height * 3 / 5));
		}
		bd3.type = b2_staticBody;

		bodies.push_back(world->GetB2World()->CreateBody(&bd3));

		b2PolygonShape shape3;
		if(_height > _width)
		{
			shape3.SetAsBox(_width * 3 / 5, _height);
		}
		else if (_width > _height)
		{
			shape3.SetAsBox(_width, _height * 3 / 5);
		}
		GetBody(2)->CreateFixture(&shape3,1.0f);
	}	
}

void Wall::Draw(SpriteBatch& _spriteBatch) {
	for (int i = 1; i < bodies.size()-1; i++) {
		if(GetBody(i)-> GetFixtureList()->GetShape()->GetType() == b2Shape::e_polygon)
		{
			b2Vec2 positionMetres = GetBody(i)->GetPosition();
			b2PolygonShape* shape = (b2PolygonShape*)GetBody(i)->GetFixtureList()->GetShape();

			float widthMetres = shape->GetVertex(0).x * 2;
			float heightMetres = shape->GetVertex(0).y * 2;

			glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);
			glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
			Color color;

			_spriteBatch.Draw(destRect, uv, texture.id, 0.0F, GetBody(i)->GetAngle(), color);
		}
	}
}