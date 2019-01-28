#pragma once
#include <iostream>
#include <vector>
#include "math.h"

static Vector Vec2 = Vector(2);

Vector Vector2(double x, double y);

class Point2D
{
	public:
		Point2D();
		Point2D(const Vector & v);
		Point2D(double x, double y);
		void Set(double x, double y);
		void SetX(double x);
		void SetY(double y);
		double X() const;
		double Y() const;
		Vector &to_vector();

	private:
		Vector _p;
};

class Component
{
	public:
		Component();
		Component(int type, unsigned int ID);
		unsigned int get_ID();
		int get_type();

		static enum{ RIGIDBODY_E, COLLIDER_E };

	private:
		int _type = 0;
		unsigned int _ID = -1;

};

class Game_Object
{
	public:
		Component &get_component(int type);

	protected:
		std::vector<Component> _components;
};