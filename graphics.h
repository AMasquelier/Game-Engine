#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <gl\GLU.h>
#include <vector>

#include "math.h"
#include "basics.h"

#define GE_WINDOWS_CENTERED SDL_WINDOWPOS_CENTERED

class Color;
class Camera;

bool init_graphics();


Color rgba_color(float r, float g, float b, float a);
Color rgb_color(float r, float g, float b);
class Color
{
	public:
		Color();
		Color(float R, float G, float B, float A);
		float r, g, b, a;

};

class Texture
{
	public:
		void Load(const char *filename);
		void LoadText(TTF_Font *font, const char *text, Color color);
		bool isLoaded();
		GLuint get_tex();
		int get_w();
		int get_h();
		int GetW();
		int GetH();
		~Texture();


	private:
		GLuint _texture;
		bool _loaded = false;
		int _w, _h;

};


class Camera
{
	public:
		Camera();
		void Perspective();
		void GUI();
		void FlatPerspective();
		void Update();
		void Follow(Point2D *p);
		void GoTo(const Point2D &p);
		void Rotate(float a);
		void SetFormat(int w, int h);
		void SetFOV(double fov);
		void SetZoom(double zoom);
		void SetPos(double x, double y);
		void SetLimit(double lx, double rx, double uy, double dy);
		double X();
		double Y();

	private:
		Point2D _pos;
		Point2D*_follow = nullptr;
		double _zoom = 1;
		double _fov = 36;
		float _a = 0;
		int _w, _h;
		double _x_limit_l = 0, _x_limit_r = 0;
		double _y_limit_u = 0, _y_limit_d = 0;
};

class Window
{
	public:
		Window();
		bool create(const char *name, int w, int h, int x, int y);
		bool modify(const char *name, int w, int h, int x, int y);
		void toggle_fullscreen();
		void update();
		void clear();
		void set_clear_color(Color color);
		int get_width();
		int get_height();
		int get_x();
		int get_y();

	private:
		Camera _default_cam;
		SDL_Window* _screen = nullptr;
		SDL_GLContext GL_context;
		int _w, _h;
		int _x = 0, _y = 0;
		bool _fullscreen = false;
};


class Draw
{
	public:
		// 2D
		static void Init();
		static void Rectangle(Point2D p1, Point2D p2, Color color);
		static void Filled_Rectangle(Point2D p1, Point2D p2, Color color);
		static void Circle(Point2D p, double r, int accuracy, Color color);
		static void Line(Point2D p1, Point2D p2, Color color);
		static void Arrow(Point2D p1, Point2D p2, Color color);
		static void Debug_Text(Point2D p, std::string text, int h, Color color);
		static void Text(std::string text, double size, double x, double y);
		static void TEXTURE(Point2D p, Texture &tex);
		static void Rotated_BITMAP(Vector &p, Vector &c, float a, Texture &tex);
		static void TEXTURE_region(Point2D dst, double dw, double dh, Point2D src, double sw, double sh, Texture &tex);
		static void tinted_BITMAP_region(Vector &dst, double dw, double dh, Vector &src, double sw, double sh, Color color, Texture &tex);
		static void Rotated_BITMAP_Region(Vector &dst, double dw, double dh, Vector &src, double sw, double sh, Vector &c, float a, Texture &tex);
		static void tinted_Rotated_BITMAP_Region(Vector &dst, double dw, double dh, Vector &src, double sw, double sh, Vector &c, float a, Color &color, Texture &tex);
	
	private:
		static int ASCII_table[255];
		static Texture _debug_font;
};



class Graphics_Engine
{
	public:

	private:

};