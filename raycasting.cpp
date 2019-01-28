#include "raycasting.h"

// Ray
Ray::Ray()
{
	_src = Vector2(0, 0);
	_a = 0;
}

Ray::Ray(Vector p, float a)
{
	_src = p;
	_a = a;
	if (_a < 0) _a = 2 * PI + _a;
}

void Ray::load(Vector p, float a)
{
	_src = p;
	_a = a;
	if (_a < 0) _a = 2 * PI + _a;
}

void Ray::debug_display()
{
	Draw::Line(_src, _cast, rgb_color(1, 0, 0));
	Draw::Circle(_cast, 2.5, 50, rgb_color(1, 0, 0));
}

int Ray::get_ID()
{
	return _ID;
}

void Ray::set_ID(int ID)
{
	_ID = ID;
}

Vector Ray::get_src()
{
	return _src;
}

void Ray::set_src(Vector p)
{
	_src = p;
}

void Ray::set_cast(Vector cast)
{
	_cast = cast;
}

double Ray::get_range()
{
	return _range;
}

void Ray::set_range(double r)
{
	_range = r;
}

float Ray::get_a()
{
	return _a;
}

void Ray::set_a(double a)
{
	_a = a;
	if (_a < 0) _a = 2 * PI + _a;
}

void Ray::clear()
{
	_cast = Vector2(0, 0);
}

void Ray::set_on(bool o)
{
	_on = o;
}

bool Ray::is_on()
{
	return _on;
}


// Ray Caster
RayCaster::RayCaster()
{
}

void RayCaster::debug_display()
{
	glColor4f(1, 1, 1, 1);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < _polygon.size(); i++)
	{
		glVertex2d(_polygon[i][0], _polygon[i][1]);
	}
	glEnd();
}

void RayCaster::add_point(Vector p)
{
	_polygon.push_back(p);
}

int RayCaster::get_nb_points()
{
	return _polygon.size();;
}

Vector RayCaster::get_point(int i)
{
	if (i < _polygon.size()) return _polygon[i];
	return Vector2(0, 0);
}

void RayCaster::set_point(int i, Vector p)
{
	if (i < _polygon.size())  _polygon[i] = p;
}

int RayCaster::cast(Ray ray, Vector * cast)
{
	double x1 = ray.get_src()[0], y1 = ray.get_src()[1];
	float a = ray.get_a();

	Vector target = *cast;
	//cast->Set(x1 + cos(a) * 1000, y1 + sin(a) * 1000);
	double dx = target[0] - x1, dy = target[1] - y1;
	int size = _polygon.size();

	int ret = 0;

	for (int i = 0; i < size; i++)
	{
		double px1 = _polygon[i][0], py1 = _polygon[i][1];
		double pdx = _polygon[(i + 1) % size][0] - px1, pdy = _polygon[(i + 1) % size][1] - py1;

		double t2 = ((px1 - x1)*dy - (py1 - y1) * dx) / (pdy*dx - pdx * dy);
		double t1 = (px1 - x1 + t2 * pdx) / dx;

		if (dx == 0)
		{
			if (y1 <= py1 && py1 <= target[1])
			{
				if (dist2(ray.get_src(), Vector2(x1, py1)) <= dist2(ray.get_src(), target))
				{
					if (_opacity)
					{
						target[0] = x1;
						target[1] = py1;
						ret = 1;
					}
					else _isTouched = true;
				}
			}
		}
		else
		{
			if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
			{
				
				if (dist2(ray.get_src(), Vector2(x1 + t1 * dx, y1 + t1 * dy)) <= dist2(ray.get_src(), target))
				{
					
					if (_opacity)
					{
						target[0] = x1 + t1 * dx;
						target[1] = y1 + t1 * dy;	
						ret = 1;
					}
					else _isTouched = true;
				}
			}
		}
	}
	(*cast) = target;

	return ret;
}

int RayCaster::get_ID()
{
	return _ID;
}

void RayCaster::set_ID(int ID)
{
	_ID = ID;
}

void RayCaster::set_opacity(bool opacity)
{
	_opacity = opacity;
}

bool RayCaster::is_opaque()
{
	return _opacity;
}

bool RayCaster::is_touched()
{
	return _isTouched;
}

void RayCaster::clear()
{
	_isTouched = false;
}


// Ray Casting
RayCasting::RayCasting()
{
}

void RayCasting::init()
{
	// Nothing
}

void RayCasting::run()
{
	for (int l = 0; l < _RC.size(); l++) _RC[l].clear();
	for (int i = 0; i < _R.size(); i++)
	{
		_R[i].clear();
		if (_R[i].is_on())
		{
			for (int k = 0; k < _R.size(); k++)
			{
				Vector cast;
				cast = Vector2(_R[i].get_src()[0] + cos(_R[k].get_a()) * _R[k].get_range(), _R[i].get_src()[1] + sin(_R[k].get_a()) *  _R[k].get_range());
				
				for (int l = 0; l < _RC.size(); l++)
				{
					_RC[l].cast(_R[k], &cast);
				}
				_R[i].set_cast(cast);
				
			}
		}
	}
}

void RayCasting::debug_display()
{
	for (int i = 0; i < _R.size(); i++)
	{
		if (_R[i].is_on()) _R[i].debug_display();
	}
	for (int i = 0; i < _RC.size(); i++) _RC[i].debug_display();
}

unsigned int RayCasting::add_raycaster()
{
	RayCaster buf;
	_RC.push_back(buf);
	_RC[_RC.size() - 1].set_ID(_RC_ID); _RC_ID++;
	return _RC_ID-1;
}

unsigned int RayCasting::add_ray()
{
	Ray buf;
	_R.push_back(buf);
	_R[_RC.size() - 1].set_ID(_RC_ID); _RC_ID++;
	return _RC_ID-1;
}

Ray * RayCasting::get_ray_ptr(unsigned int ID)
{
	for (int i = 0; i < _R.size(); i++)
	{
		if (_R[i].get_ID() == ID) return &_R[i];
	}
	return nullptr;
}

RayCaster * RayCasting::get_raycaster_ptr(unsigned int ID)
{
	for (int i = 0; i < _RC.size(); i++)
	{
		if (_RC[i].get_ID() == ID) return &_RC[i];
	}
	return nullptr;
}

void RayCasting::remove_ray(unsigned int ID)
{
	for (int i = 0; i < _R.size(); i++)
	{
		if (_R[i].get_ID() == ID)
		{
			_R.erase(_R.begin() + i);
			break;
		}
	}
}

void RayCasting::remove_raycaster(unsigned int ID)
{
	for (int i = 0; i < _RC.size(); i++)
	{
		if (_RC[i].get_ID() == ID)
		{
			_RC.erase(_RC.begin() + i);
			break;
		}
	}
}
