#pragma once
#include <vector>
#include "graphics.h"
#include "basics.h"

class Ray
{
	public:
		Ray();
		Ray(Vector p, float a);
		void load(Vector p, float a);
		void debug_display();

		int get_ID();
		void set_ID(int ID);
		Vector get_src();
		void set_src(Vector p);
		void set_cast(Vector cast);
		double get_range();
		void set_range(double r);
		float get_a();
		void set_a(double a);
		
		void clear();
		void set_on(bool o);
		bool is_on();

	private:
		unsigned int _ID;
		float _a = 0;
		double _range = 99;
		Vector _src, _cast;

		bool _on = true; // If the ray is to check
};

class RayCaster
{
	public:
		RayCaster();
		void debug_display();
		void add_point(Vector p);
		int get_nb_points();
		Vector get_point(int i);
		void set_point(int i, Vector p);
		int cast(Ray ray, Vector *cast);
		int get_ID();
		void set_ID(int ID);
		void set_opacity(bool opacity);
		bool is_opaque();
		bool is_touched();
		void clear();

	private:
		unsigned int _ID;
		bool _opacity = true;
		bool _isTouched = false;
		std::vector<Vector> _polygon;
};



class RayCasting
{
	public:
		RayCasting();
		void init();
		void run();
		void debug_display();
		unsigned int add_raycaster();
		unsigned int add_ray();
		Ray *get_ray_ptr(unsigned int ID);
		RayCaster *get_raycaster_ptr(unsigned int ID);
		void remove_ray(unsigned int ID);
		void remove_raycaster(unsigned int ID);

	private:
		unsigned int _R_ID, _RC_ID;
		std::vector<RayCaster> _RC;
		std::vector<Ray> _R;
};