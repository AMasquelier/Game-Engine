#include "physics.h"

// Rigidbody
Rigidbody::Rigidbody()
{
	_force.reserve(2);
	_acceleration.reserve(2);
	_velocity.reserve(2);
	_position.reserve(2);

}

unsigned int Rigidbody::get_ID()
{
	return _ID;
}

void Rigidbody::set_ID(unsigned int ID)
{
	_ID = ID;
}

void Rigidbody::debug_display()
{
	b2Vec2 points[4];
	for (int i = 0; i < 4; i++)
		points[i] = ((b2PolygonShape *)_body->GetFixtureList()->GetShape())->m_vertices[i];

	glColor4f(1, 1, 0, 1);
	glPushMatrix();
	glTranslatef(_body->GetPosition().x, _body->GetPosition().y, 0);
	glRotatef(_body->GetAngle()*180.0 / PI, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++)
		glVertex3f(points[i].x, points[i].y, 0);

	glEnd();
	glPopMatrix();

	Draw::Circle(_body->GetPosition(), 0.2, 10, rgb_color(1, 1, 0));
	Draw::Arrow(_body->GetPosition(), _body->GetPosition() + 0.1 * _body->GetLinearVelocity(), rgb_color(1, 1, 0));
}

void Rigidbody::update()
{
}

void Rigidbody::set_box_body(b2World * world, double x, double y, double w, double h)
{
	b2BodyDef bd;
	bd.position.Set(x, y);
	b2Body *body = world->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(w / 2, h / 2);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0;
	body->CreateFixture(&fd);

	_body = body;
}

void Rigidbody::set_dynamic()
{
	_body->SetType(b2_dynamicBody);
}



// Collider
BoxCollider::BoxCollider()
{
}

void BoxCollider::update()
{
}

void BoxCollider::set_ID(unsigned int ID)
{
	_ID = ID;
}

unsigned int BoxCollider::get_ID()
{
	return _ID;
}

void BoxCollider::load(b2World *world, double x, double y, double w, double h)
{
	b2BodyDef bd;
	bd.position.Set(x, y);
	b2Body *body = world->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(w / 2, h / 2);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0;
	body->CreateFixture(&fd);

	_collider = body;
}

void BoxCollider::debug_display()
{
	b2Vec2 points[4];
	for (int i = 0; i < 4; i++)
		points[i] = ((b2PolygonShape *)_collider->GetFixtureList()->GetShape())->m_vertices[i];

	glColor4f(1, 0, 0, 1);
	glPushMatrix();
	glTranslatef(_collider->GetPosition().x, _collider->GetPosition().y, 0);
	glRotatef(_collider->GetAngle()*180.0 / PI, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++)
		glVertex3f(points[i].x, points[i].y, 0);

	glEnd();
	glPopMatrix();

}



////////////////////////////
//     Physics Engine     //
////////////////////////////

Physics_Engine *Physics_Engine::current_engine = nullptr;
Physics_Engine::Physics_Engine()
{
	current_engine = this;
	_free_rb_id.push(0);
	_free_bc_id.push(0);
	_world = new b2World(b2Vec2(0.0, 90.0));
}

void Physics_Engine::debug_display()
{
	for (int i = 0; i < _colliders.size(); i++)
		_colliders[i].debug_display();

	
	for (int i = 0; i < _rigidbodies.size(); i++)
		_rigidbodies[i].debug_display();

}

void Physics_Engine::run()
{
	Physics_Engine::current_engine = this;
	
	_world->Step(_dt, 5, 500);
}

Component Physics_Engine::add_rigidbody(Rigidbody rb)
{
	_rigidbodies.push_back(rb);
	
	unsigned int ID = get_rb_id();
	_rigidbodies[_rigidbodies.size()-1].set_ID(ID);
	return Component(Component::RIGIDBODY_E, ID);
}

Component Physics_Engine::add_box_rigidbody(double x, double y, double w, double h)
{
	Rigidbody rb;
	rb.set_box_body(_world, x, y, w, h);
	rb.set_dynamic();
	_rigidbodies.push_back(rb);

	unsigned int ID = get_rb_id();
	_rigidbodies[_rigidbodies.size() - 1].set_ID(ID);
	return Component(Component::RIGIDBODY_E, ID);
}

Rigidbody *Physics_Engine::get_rigidbody(unsigned int ID)
{
	for (int i = 0; i < _rigidbodies.size(); i++)
	{
		if (_rigidbodies[i].get_ID() == ID) return &_rigidbodies[i];
	}
	return nullptr;
}

Component Physics_Engine::add_collider(BoxCollider col)
{
	_colliders.push_back(col);

	unsigned int ID = get_bc_id();
	_colliders[_colliders.size()-1].set_ID(ID);
	return Component(Component::COLLIDER_E, ID);
}

Component Physics_Engine::add_collider(double x, double y, double w, double h)
{
	BoxCollider col;
	
	col.load(_world, x, y, w, h);
	_colliders.push_back(col);

	unsigned int ID = get_bc_id();
	_colliders[_colliders.size() - 1].set_ID(ID);
	return Component(Component::COLLIDER_E, ID);
}

BoxCollider * Physics_Engine::get_collider(unsigned int ID)
{
	for (int i = 0; i < _colliders.size(); i++)
	{
		if (_colliders[i].get_ID() == ID) return &_colliders[i];
	}
	return nullptr;
}

void Physics_Engine::set_dt(float dt)
{
	_dt = dt;
}

float Physics_Engine::get_dt()
{
	return _dt;
}

unsigned int Physics_Engine::get_rb_id()
{
	unsigned int id = _free_rb_id.top();
	_free_rb_id.pop();

	if (_free_rb_id.size() == 0) _free_rb_id.push(id + 1);
	return id;
}

unsigned int Physics_Engine::get_bc_id()
{
	unsigned int id = _free_bc_id.top();
	_free_bc_id.pop();

	if (_free_bc_id.size() == 0) _free_bc_id.push(id + 1);
	return id;
}


