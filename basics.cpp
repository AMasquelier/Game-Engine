#include "basics.h"

Vector Vector2(double x, double y)
{
	Vector v = Vec2;
	v[0] = x; v[1] = y;
	return v;
}

Point2D::Point2D()
{
	_p.reserve(2);
}

Point2D::Point2D(const Vector & v)
{
	if (v.is_defined()) _p = v;
}

Point2D::Point2D(double x, double y)
{
	_p.reserve(2);
	_p[0] = x;
	_p[1] = y;
}

void Point2D::Set(double x, double y)
{
	_p[0] = x; _p[1] = y;
}

void Point2D::SetX(double x)
{
	_p[0] = x;
}

void Point2D::SetY(double y)
{
	_p[1] = y;
}

double Point2D::X() const
{
	return _p(0);
}

double Point2D::Y() const
{
	return _p(1);
}

Vector & Point2D::to_vector()
{
	return _p;
}

Component::Component()
{
}

// Component
Component::Component(int type, unsigned int ID)
{
	_ID = ID;
	_type = type;
}

unsigned int Component::get_ID()
{
	return _ID;
}

int Component::get_type()
{
	return _type;
}

Component & Game_Object::get_component(int type)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i].get_type() == type) return _components[i];
	}
}
