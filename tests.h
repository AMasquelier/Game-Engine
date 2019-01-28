#pragma once
#include "basics.h"
#include "physics.h"
#include "graphics.h"

class Character : public Game_Object
{
	public:
		Character();
		Character(Vector pos);
		void load(Vector pos);
		void update();
		void display();

		void go_left(float c);
		void go_right(float c);
		void jump(float c);


	private:
		Vector _pos = Vec2;
		bool _colliding[4] = { false, false, false, false };
		Physics_Engine *act_engine;
};