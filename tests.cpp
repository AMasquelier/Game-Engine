#include "tests.h"

Character::Character()
{
	Physics_Engine *act_engine = Physics_Engine::current_engine;

	BoxCollider bc; 
	bc.setAABB(_pos - Vector2(32, 32), _pos + Vector2(32, 32));
	bc.set_restitution(0.0);
	_components.push_back(act_engine->add_collider(bc));

	Rigidbody rb; rb.set_position(_pos);
	_components.push_back(act_engine->add_rigidbody(rb));

}

Character::Character(Vector pos)
{
	_pos = pos;
	act_engine = Physics_Engine::current_engine;

	Rigidbody rb; rb.set_position(_pos);
	_components.push_back(act_engine->add_rigidbody(rb));

	BoxCollider bc;
	bc.setAABB(_pos - Vector2(32, 32), _pos + Vector2(32, 32));
	bc.set_restitution(0.0);
	bc.bind_rigidbody(get_component(Component::RIGIDBODY_E).get_ID());
	_components.push_back(act_engine->add_collider(bc));
}

void Character::load(Vector pos)
{
	Physics_Engine *act_engine = Physics_Engine::current_engine;

	act_engine->get_collider(get_component(Component::COLLIDER_E).get_ID())->set_position(pos);
	act_engine->get_rigidbody(get_component(Component::RIGIDBODY_E).get_ID())->set_position(pos);


}

void Character::update()
{
	BoxCollider *bc = act_engine->get_collider(get_component(Component::COLLIDER_E).get_ID());
	for (int i = 0; i < 4; i++)
		_colliding[i] = bc->is_colliding(i);

	_pos = act_engine->get_rigidbody(get_component(Component::RIGIDBODY_E).get_ID())->get_position();
}

void Character::display()
{
	
	Draw::Rectangle(_pos - Vector2(32, 32), _pos + Vector2(32, 32), rgb_color(1, 1, 1));
}

void Character::go_left(float c)
{
	Rigidbody *rigidbody = act_engine->get_rigidbody(get_component(Component::RIGIDBODY_E).get_ID());
	if (rigidbody->get_velocity()[0] > -300)
		rigidbody->apply_acceleration(Vector2(-8000 * c, 0));
}

void Character::go_right(float c)
{
	Rigidbody *rigidbody = act_engine->get_rigidbody(get_component(Component::RIGIDBODY_E).get_ID());
	if (rigidbody->get_velocity()[0] < 300)
		rigidbody->apply_acceleration(Vector2(8000 * c, 0));
}

void Character::jump(float c)
{
	Rigidbody *rigidbody = act_engine->get_rigidbody(get_component(Component::RIGIDBODY_E).get_ID());
	if (_colliding[3] && rigidbody->get_velocity()(1) > -100)
	{
		rigidbody->apply_acceleration(Vector2(0, -60000 * c));
	}
}
