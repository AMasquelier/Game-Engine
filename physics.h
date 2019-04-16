#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "math.h"
#include "core.h"
#include "basics.h"
#include "graphics.h"
#include "debug.h"
#include "Box2D/Box2D.h"

class Rigidbody
{
	public:
		Rigidbody();
		void update();
		void set_box_body(b2World *world, double x, double y, double w, double h);
		void set_dynamic();
		void apply_force(double speed);

		unsigned int get_ID();
		void set_ID(unsigned int ID);

		void debug_display();

	private:
		unsigned int _ID;
		bool _gravity = true;

		b2Body *_body;
};

class BoxCollider
{
	public:
		BoxCollider();
		void update();
		void set_ID(unsigned int ID);
		unsigned int get_ID();
		void load(b2World *world, double x, double y, double w, double h);

		void debug_display();

	private:
		unsigned int _ID;
		b2Body *_collider;

};


class Physics_Engine
{
	public:
		Physics_Engine();
		void debug_display();
		void run();

		Component add_rigidbody(Rigidbody rb);
		Component add_box_rigidbody(double x, double y, double w, double h);
		Rigidbody *get_rigidbody(unsigned int ID);
		Component add_collider(BoxCollider col);
		Component add_collider(double x, double y, double w, double h);
		BoxCollider *get_collider(unsigned int ID);
		
		void set_dt(float dt);
		float get_dt();

		static Physics_Engine *current_engine;

	private:
		b2World *_world;
		float _dt = 1/60.0;
		std::vector<Rigidbody> _rigidbodies;
		std::vector<BoxCollider> _colliders;
		std::stack<unsigned int> _free_rb_id;
		std::stack<unsigned int> _free_bc_id;

		unsigned int get_rb_id();
		unsigned int get_bc_id();

};