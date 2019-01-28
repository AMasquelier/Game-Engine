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

void Rigidbody::set_ID(int ID)
{
	_ID = ID;
}

double Rigidbody::get_inv_mass()
{
	return _inv_mass;
}

Vector Rigidbody::get_position()
{
	return _position;
}

void Rigidbody::set_position(Vector v)
{
	_position = v;
}

Vector Rigidbody::get_velocity()
{
	return _velocity;
}

void Rigidbody::set_velocity(Vector v)
{
	
	_velocity = v;
}

Vector Rigidbody::get_acceleration()
{
	return _acceleration;
}

bool Rigidbody::gravity()
{
	return _gravity;
}

void Rigidbody::apply_acceleration(Vector v)
{
	_acceleration += v;
}

void Rigidbody::debug_display()
{
	Draw::Circle(_position, 3, 12, rgb_color(0.3, 0.7, 0.8));
	Draw::Arrow(_position, _position + _velocity * 0.1, rgb_color(0.3, 0.7, 0.8));
	Draw::Debug_Text(_position + Vector2(8, -20), "ID = " + std::to_string(_ID), 14, rgb_color(0.3, 0.7, 0.8));

	
}

void Rigidbody::update_inspector()
{
	/*_inspector.clear();
	_inspector.set_name("rigidbody");
	_inspector.add_element(TYPENAME::INT, "ID", &_ID);
	_inspector.add_element(TYPENAME::DOUBLE, "mass", &_mass);
	_inspector.add_element(TYPENAME::DOUBLE, "position.x", &_position[0]);
	_inspector.add_element(TYPENAME::DOUBLE, "position.y", &_position[1]);
	_inspector.add_element(TYPENAME::DOUBLE, "velocity.x", &_velocity[0]);
	_inspector.add_element(TYPENAME::DOUBLE, "velocity.y", &_velocity[1]);
	_inspector.add_element(TYPENAME::DOUBLE, "acceleration.x", &_acceleration[0]);
	_inspector.add_element(TYPENAME::DOUBLE, "acceleration.y", &_acceleration[1]);
	_inspector.add_element(TYPENAME::DOUBLE, "force.x", &_force[0]);
	_inspector.add_element(TYPENAME::DOUBLE, "force.y", &_force[1]);*/
}

Inspector *Rigidbody::get_inspector()
{
	return &_inspector;
}

void Rigidbody::update()
{
	Physics_Engine * current_engine = Physics_Engine::current_engine;
	float dt = current_engine->get_dt();

	_acceleration	+= _force * dt;
	_velocity		+= _acceleration * dt;
	_position		+= _velocity * dt;


	_force = Vec2;
	_acceleration = Vec2;
}


// AABB
AABB::AABB()
{
	min.reserve(2);
	max.reserve(2);
}

void AABB::debug_Display()
{
	Draw::Rectangle(min, max, rgb_color(0.8, 0.1, 0.1));
	Draw::Filled_Rectangle(min, max, rgba_color(0.8, 0.1, 0.1, 0.1));
}


// Collider
BoxCollider::BoxCollider()
{
}

void BoxCollider::update()
{
	if (_rigidbody != -1)
	{
		Physics_Engine * current_engine = Physics_Engine::current_engine;
		_rb_ptr = current_engine->get_rigidbody(_rigidbody);
		if (_rb_ptr != nullptr)
		{
			
			Vector dp = _rb_ptr->get_position() - _position;
			_aabb.max += dp;
			_aabb.min += dp;
			_position += dp;
		}
	}
}

AABB &BoxCollider::getAABB()
{
	return _aabb;
}

void BoxCollider::setAABB(Vector min, Vector max)
{
	_aabb.min = min;
	_aabb.max = max;
	_position = (max + min) * 0.5;
}

void BoxCollider::bind_rigidbody(int ID)
{
	_rigidbody = ID;
	Rigidbody *rb;
	if ((rb = Physics_Engine::current_engine->get_rigidbody(ID)) != nullptr)
	{
		_inv_mass = rb->get_inv_mass();
	}
	
}

float BoxCollider::get_restitution()
{
	return _restitution;
}

void BoxCollider::set_restitution(float e)
{
	_restitution = e;
}

double BoxCollider::get_inv_mass()
{
	return _inv_mass;
}

Vector BoxCollider::get_position()
{
	return _position;
}

void BoxCollider::set_position(Vector pos)
{
	_position = pos;
}

int BoxCollider::get_rigidbody()
{
	return _rigidbody;
}

Rigidbody * BoxCollider::get_rigidbody_ptr()
{
	return _rb_ptr;
}

float BoxCollider::get_static_friction()
{
	return _static_friction;
}

float BoxCollider::get_dynamic_friction()
{
	return _dynamic_friction;
}

void BoxCollider::debug_display()
{
	_aabb.debug_Display();
}

bool BoxCollider::is_colliding(int dir)
{
	return _colliding[dir];
}

void BoxCollider::collide(bool c[4])
{
	for (int i = 0; i < 4; i++)
		_colliding[i] = c[i];
}

void BoxCollider::collide(bool c, int dir)
{
	_colliding[dir] = c;
}


bool AABB_intersect(AABB a, AABB b)
{
	if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) return false;
	if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) return false;
	return true;
}

void check_collision(collision *col)
{
	// Only works with aabb shapes
	BoxCollider *A = col->a;
	BoxCollider *B = col->b;

	AABB Abox = A->getAABB();
	AABB Bbox = B->getAABB();

	Vector n = B->get_position() - A->get_position();

	col->pos_A = A->get_position();
	col->pos_B = B->get_position();

	double a_extent = (Abox.max[0] - Abox.min[0]) / 2;
	double b_extent = (Bbox.max[0] - Bbox.min[0]) / 2;

	double x_overlap = a_extent + b_extent - fabs(n[0]);
	col->penetration = Vec2;
	if (x_overlap > 0)
	{
		a_extent = (Abox.max[1] - Abox.min[1]) / 2;
		b_extent = (Bbox.max[1] - Bbox.min[1]) / 2;

		double y_overlap = a_extent + b_extent - abs(n[1]);

		if (y_overlap > 0)
		{
			if (y_overlap > x_overlap)
			{
				if (n[0] < 0)
				{
					col->normal = Vector2(-1, 0);
					col->penetration[0] = -x_overlap;
					A->collide(true, 0);
					B->collide(true, 1);
				}
				else
				{
					col->normal = Vector2(1, 0);
					col->penetration[0] = x_overlap;
					A->collide(true, 1);
					B->collide(true, 0);
				}

			}
			else
			{
				if (n[1] < 0)
				{
					col->normal = Vector2(0, -1);
					col->penetration[1] = -y_overlap;
					A->collide(true, 2);
					B->collide(true, 3);
				}
				else
				{
					col->normal = Vector2(0, 1);
					col->penetration[1] = y_overlap;
					A->collide(true, 3);
					B->collide(true, 2);
				}
			}
		}


	}
}

void resolve_collision(collision *col)
{
	BoxCollider *A = col->a;
	BoxCollider *B = col->b;
	Physics_Engine * current_engine = Physics_Engine::current_engine;
	Rigidbody *rb_a = A->get_rigidbody_ptr(), *rb_b = B->get_rigidbody_ptr();

	Vector va(2), vb(2);
	if (rb_a != nullptr) va = rb_a->get_velocity();
	if (rb_b != nullptr) vb = rb_b->get_velocity();

	Vector dv = vb - va;
	Vector normal = col->normal;
	double vn = dot_product(dv, normal);
	Vector tangent;

	if (dv[0] * normal[1] - dv[1] * normal[0] < 0)
		tangent = Vector2(-normal[1], normal[0]);
	else
		tangent = Vector2(normal[1], -normal[0]);

	float e = fmin(A->get_restitution(), B->get_restitution());
	float j = -(1 + e) * vn * (1 / (A->get_inv_mass() + B->get_inv_mass()));
	float tj = dot_product(dv, tangent) * (1 / (A->get_inv_mass() + B->get_inv_mass())) * 0.2;



	if (rb_a != nullptr)
	{
		rb_a->set_velocity(rb_a->get_velocity() - (comp_product(tangent, Vector2(1, 0.2)) * (-tj) + col->normal * j) * A->get_inv_mass());
	}
}

void position_correction(collision *col)
{
	BoxCollider *A = col->a;
	BoxCollider *B = col->b;
	Physics_Engine * current_engine = Physics_Engine::current_engine;

	Rigidbody *rb_a = A->get_rigidbody_ptr(), *rb_b = B->get_rigidbody_ptr();

	if (rb_a != nullptr) col->penetration -= col->pos_A - rb_a->get_position();
	if (rb_b != nullptr) col->penetration -= col->pos_B - rb_b->get_position();

	if (rb_a != nullptr)
	{
		Vector a = rb_a->get_position();
		rb_a->set_position(a - col->penetration * A->get_inv_mass());
	}
	if (rb_b != nullptr)
	{
		Vector b = rb_b->get_position();
		rb_b->set_position(b + col->penetration * B->get_inv_mass());
	}
}



////////////////////////////
//     Physics Engine     //
////////////////////////////

Physics_Engine *Physics_Engine::current_engine = nullptr;
Physics_Engine::Physics_Engine()
{
	current_engine = this;
}

void Physics_Engine::debug_display()
{
	for (int i = 0; i < _colliders.size(); i++)
		_colliders[i].debug_display();

	for (int i = 0; i < _rigidbodies.size(); i++)
		_rigidbodies[i].debug_display();

	for (int i = 0; i < _collisions.size(); i++)
	{
		Vector p = (_collisions[i].a->get_position() + _collisions[i].a->get_position()) * 0.5;
		Draw::Arrow(p, p + _collisions[i].normal * 80, rgb_color(0.2, 0.9, 0.2));
	}
}

void Physics_Engine::run()
{
	Physics_Engine::current_engine = this;
	
	for (int i = 0; i < _rigidbodies.size(); i++)
	{
		if (_rigidbodies[i].gravity()) _rigidbodies[i].apply_acceleration(_gravity);
		_rigidbodies[i].update();
	}
	_collisions.clear();
	for (int i = 0; i < _colliders.size(); i++)
	{
		bool colliding_reset[4] = { false, false, false, false };
		_colliders[i].collide(colliding_reset);

		_colliders[i].update();
		if (_colliders[i].get_rigidbody() != -1)
		{
			for (int j = 0; j < _colliders.size(); j++)
			{
				if (j != i)
				{
					if (AABB_intersect(_colliders[i].getAABB(), _colliders[j].getAABB()))
					{
						collision col;
						col.a = &_colliders[i];
						col.b = &_colliders[j];
						_collisions.push_back(col);
					}
				}
			}
		}
	}

	for (int i = 0; i < _collisions.size(); i++)
	{
		check_collision(&_collisions[i]);
		resolve_collision(&_collisions[i]);
		position_correction(&_collisions[i]);
	}
	
	for (int i = 0; i < _colliders.size(); i++)
		_colliders[i].update();
}

Component Physics_Engine::add_rigidbody(Rigidbody rb)
{
	_rigidbodies.push_back(rb);
	
	unsigned int ID = _rigidbodies.size() - 1;
	_rigidbodies[ID].set_ID(ID);
	return Component(Component::RIGIDBODY_E, ID);
}

Rigidbody *Physics_Engine::get_rigidbody(unsigned int ID)
{
	if (ID < _rigidbodies.size())
		return &_rigidbodies[ID];
	return nullptr;
}

Component Physics_Engine::add_collider(BoxCollider col)
{
	_colliders.push_back(col);

	unsigned int ID = _colliders.size() - 1;
	return Component(Component::COLLIDER_E, ID);
}

BoxCollider * Physics_Engine::get_collider(unsigned int ID)
{
	if (ID < _colliders.size())
		return &_colliders[ID];
	return nullptr;
}

float Physics_Engine::get_dt()
{
	return _dt;
}


