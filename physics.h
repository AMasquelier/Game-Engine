#pragma once
#include <iostream>
#include <vector>
#include "math.h"
#include "core.h"
#include "basics.h"
#include "graphics.h"
#include "debug.h"

class Rigidbody
{
	public:
		Rigidbody();
		Rigidbody(Point2D p, double mass);
		void update();

		unsigned int get_ID();
		void set_ID(int ID);
		double get_inv_mass();
		Vector get_position();
		void set_position(Vector v);
		Vector get_velocity();
		void set_velocity(Vector v);
		Vector get_acceleration();
		bool gravity();

		void apply_acceleration(Vector v);

		void debug_display();
		Inspector *get_inspector();
		void update_inspector();

	private:
		// Caracteristics
		unsigned int _ID;
		bool _gravity = true;

		// State
		bool _colliding;
		Vector _force;
		Vector _acceleration;
		Vector _velocity;
		Vector _position;

		double _mass = 1;
		double _inv_mass = 1;

		// Debug
		Inspector _inspector;

};

class AABB
{
	public:
		AABB();
		void debug_Display();
		Vector min, max;
};

class BoxCollider
{
	public:
		BoxCollider();
		void update();
		void bind_rigidbody(int ID);

		AABB &getAABB();
		void setAABB(Vector min, Vector max);
		float get_restitution();
		void set_restitution(float e);
		double get_inv_mass();
		Vector get_position();
		void set_position(Vector pos);
		int get_rigidbody();
		Rigidbody *get_rigidbody_ptr();
		float get_static_friction();
		float get_dynamic_friction();

		bool is_colliding(int dir);
		void collide(bool c[4]);
		void collide(bool c, int dir);

		void debug_display();

	private:
		unsigned int _ID;
		float _static_friction = 0.8;
		float _dynamic_friction = 0.8;
		Vector _position = Vec2;
		AABB _aabb;
		float _restitution = 0;
		int _rigidbody = -1;
		Rigidbody *_rb_ptr = nullptr;
		double _inv_mass = 0;

		bool _colliding[4] = { false, false, false, false };

};

typedef struct collision
{
	BoxCollider *a;
	BoxCollider *b;
	Vector normal = Vector(2);
	Vector penetration = Vec2;
	Vector pos_A, pos_B;

} collision;


class Physics_Engine
{
	public:
		Physics_Engine();
		void debug_display();
		void run();

		Component add_rigidbody(Rigidbody rb);
		Rigidbody *get_rigidbody(unsigned int ID);
		Component add_collider(BoxCollider col);
		BoxCollider *get_collider(unsigned int ID);

		float get_dt();

		static Physics_Engine *current_engine;

	private:
		float _dt = 1/60.0;
		Vector _gravity = Vector2(0, 981);
		std::vector<Rigidbody> _rigidbodies;
		std::vector<BoxCollider> _colliders;

		std::vector<collision> _collisions;
};