#include "graphics.h"

bool init_graphics()
{
	glEnable(GL_TEXTURE_2D);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL initialisation error : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "TTF initialisation error : " << TTF_GetError() << std::endl;
		SDL_Quit();
		return false;
	}
	//std::cout << "SDL_mixer initialisation error : " << Mix_GetError() << std::endl;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	return true;
}

// Window
Window::Window()
{
}

bool Window::create(const char *name, int w, int h, int x, int y)
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	_w = w; _h = h;

	_screen = SDL_CreateWindow(name, x, y, w, h, SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_OPENGL);
	if (_screen == nullptr) return false;

	GL_context = SDL_GL_CreateContext(_screen);
	

	if (GL_context == 0)
	{
		std::cout << "OpenGL context creation error : " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(_screen);
		SDL_Quit();
		return false;
	}

	_default_cam.SetFormat(w, h);
	_default_cam.SetPos(0, 0);
	_default_cam.SetZoom(10);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glEnable(GL_DEPTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_MULTISAMPLE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return true;
}

bool Window::modify(const char * name, int w, int h, int x, int y)
{
	SDL_SetWindowSize(_screen, w, h);
	SDL_SetWindowPosition(_screen, x, y);
	SDL_SetWindowTitle(_screen, name);
	_x = x; _y = y;
	_w = w; _h = h;
	return true;
}

void Window::toggle_fullscreen()
{
	_fullscreen = !_fullscreen;
	if(_fullscreen) SDL_SetWindowFullscreen(_screen, SDL_WINDOW_FULLSCREEN);
	else SDL_SetWindowFullscreen(_screen, SDL_WINDOW_FULLSCREEN);
}

void Window::update()
{
	SDL_GL_SwapWindow(_screen);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_default_cam.Perspective();
}

void Window::set_clear_color(Color color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

int Window::get_width()
{
	return _w;
}

int Window::get_height()
{
	return _h;
}

int Window::get_x()
{
	return _x;
}

int Window::get_y()
{
	return _y;
}


// COLOR
Color rgba_color(float r, float g, float b, float a)
{
	Color c(r, g, b, a);
	return c;
}

Color rgb_color(float r, float g, float b)
{
	Color c(r, g, b, 1);
	return c;
}

Color::Color()
{
	r = g = b = a = 0;
}

Color::Color(float R, float G, float B, float A)
{
	r = R; g = G; b = B; a = A;
}


// Bitmap
void Texture::Load(const char *filename)
{
	SDL_Surface *img = IMG_Load(filename);

	if (img)
	{
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		int nColors;
		GLenum textureFormat;
		nColors = img->format->BytesPerPixel;
		_w = img->w; _h = img->h;
		//contains an alpha channel
		if (nColors == 4)
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
		}
		else if (nColors == 3) //no alpha channel
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
		}
		else
		{
			std::cout << nColors << std::endl;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, textureFormat, GL_UNSIGNED_BYTE, img->pixels);
		_loaded = true;

		SDL_FreeSurface(img);
	}
	else std::cout << "Can't load " << filename << " : " << IMG_GetError() << std::endl;
}

void Texture::LoadText(TTF_Font * font, const char * text, Color color)
{
	SDL_Color col;
	col.r = color.r * 255; col.g = color.g * 255; col.b = color.b * 255; col.a = color.a * 255;
	SDL_Surface *img = TTF_RenderText_Blended(font, text, col);

	if (_loaded) glDeleteTextures(1, &_texture);

	if (img)
	{
		_texture = 0;
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		int nColors;
		GLenum textureFormat;
		nColors = img->format->BytesPerPixel;
		_w = img->w; _h = img->h;
		//contains an alpha channel
		if (nColors == 4)
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
		}
		else if (nColors == 3) //no alpha channel
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
		}
		else
		{
			std::cout << nColors << std::endl;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, textureFormat, GL_UNSIGNED_BYTE, img->pixels);
		_loaded = true;
		SDL_FreeSurface(img);
	}
}

bool Texture::isLoaded()
{
	return _loaded;
}

GLuint Texture::get_tex()
{
	return _texture;
}

int Texture::get_w()
{
	return _w;
}

int Texture::get_h()
{
	return _h;
}

int Texture::GetW()
{
	return _w;
}

int Texture::GetH()
{
	return _h;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}



//Camera
Camera::Camera()
{

}

void Camera::Perspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double w = double(_w) / _zoom, h = double(_h) / _zoom;
	gluOrtho2D(-0.5 * w, 0.5 * w, 0.5 * h, -0.5 * h);
	
	glRotatef(_a, 0, 0, 1);

	glTranslated(-_pos.X(), -_pos.Y(), 0);

	glMatrixMode(GL_MODELVIEW);

}

void Camera::GUI()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 2D
	double w = double(_w), h = double(_h);
	gluOrtho2D(0, w, h, 0);


	glRotatef(0, 0, 0, 1);

	glTranslatef(0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
}

void Camera::FlatPerspective()
{
}

void Camera::Update()
{
	if (_follow != nullptr)
	{
		_pos.Set(_follow->X(), _follow->Y());
	}

	if (_x_limit_l != _x_limit_r)
	{
		if (_pos.X() - _w / (2 * _zoom) < _x_limit_l) _pos.SetX(_x_limit_l + _w / (2 * _zoom));
		if (_pos.X() + _w / (2 * _zoom) > _x_limit_r) _pos.SetX(_x_limit_r - _w / (2 * _zoom));
	}

	if (_y_limit_u != _y_limit_d)
	{
		if (_pos.Y() - _h / (2 * _zoom) < _y_limit_u) _pos.SetY(_y_limit_u + _h / (2 * _zoom));
		if (_pos.Y() + _h / (2 * _zoom) > _y_limit_d) _pos.SetY(_y_limit_d - _h / (2 * _zoom));
	}
}

void Camera::Follow(Point2D * p)
{
	_follow = p;
}

void Camera::GoTo(const Point2D &p)
{
	_pos.Set(p.X(), p.Y());
}

void Camera::Rotate(float a)
{
	_a = a;
}

void Camera::SetFormat(int w, int h)
{
	_w = w; _h = h;
}

void Camera::SetFOV(double fov)
{
	_fov = fov;
}

void Camera::SetZoom(double zoom)
{
	_zoom = zoom;
}

void Camera::SetPos(double x, double y)
{
	_pos.Set(x, y);
}

void Camera::SetLimit(double lx, double rx, double uy, double dy)
{
	_x_limit_l = lx; _x_limit_r = rx;
	_y_limit_d = dy; _y_limit_u = uy;
}

double Camera::X()
{
	return _pos.X();
}

double Camera::Y()
{
	return _pos.Y();
}



// Draw
int Draw::ASCII_table[255];
Texture Draw::_debug_font;
void Draw::Init()
{
	TTF_Font *dfont;
	dfont = TTF_OpenFont("Hack-Regular.ttf", 14);
	_debug_font.LoadText(dfont, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrtsuvwxyz0123456789 .,:'!?/*+-=_()", rgb_color(1, 1, 1));
	TTF_CloseFont(dfont);

	for (int i = 0; i < 255; i++)
		ASCII_table[i] = 67;
	for (int i = 0; i < 26; i++)
	{
		ASCII_table[65 + i] = i;
		ASCII_table[97 + i] = 26 + i;
	}
	for (int i = 0; i < 10; i++)
		ASCII_table[48 + i] = 52 + i;
	char buf = ASCII_table['s'];
	ASCII_table['s'] = ASCII_table['t'];
	ASCII_table['t'] = buf;


	ASCII_table[' '] = 62;
	ASCII_table['.'] = 63;
	ASCII_table[','] = 64;
	ASCII_table[':'] = 65;
	ASCII_table['\''] = 66;
	ASCII_table['!'] = 67;
	ASCII_table['?'] = 68;
	ASCII_table['/'] = 69;
	ASCII_table['*'] = 70;
	ASCII_table['+'] = 71;
	ASCII_table['-'] = 72;
	ASCII_table['='] = 73;
	ASCII_table['_'] = 74;
	ASCII_table['('] = 75;
	ASCII_table[')'] = 76;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Draw::Rectangle(Point2D p1, Point2D p2, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINE_LOOP);
	glVertex3f(p1.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p2.Y(), 0);
	glVertex3f(p1.X(), p2.Y(), 0);
	glEnd();
}

void Draw::Filled_Rectangle(Point2D p1, Point2D p2, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);
	glVertex3f(p1.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p2.Y(), 0);
	glVertex3f(p1.X(), p2.Y(), 0);
	glEnd();
}

void Draw::Circle(Point2D p, double r, int accuracy, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_POLYGON);
	float a = (2 * 3.1415) / accuracy;
	for (int i = 0; i < accuracy; i++)
		glVertex3f(p.X() + cos(i*a) * r, p.Y() + sin(i*a) * r, 0);
	glEnd();

}

void Draw::Line(Point2D p1, Point2D p2, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINE_STRIP);
	glVertex3f(p1.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p2.Y(), 0);
	glEnd();
}

void Draw::Arrow(Point2D p1, Point2D p2, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	Vector v = p2.to_vector() - p1.to_vector();
	float a = atan2f(-v[1], v[0]);

	glBegin(GL_LINE_STRIP);
	glVertex3f(p1.X(), p1.Y(), 0);
	glVertex3f(p2.X(), p2.Y(), 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(p2.X(), p2.Y(), 0);
	glVertex3f(p2.X() - cos(a + 0.4) * 1, p2.Y() + sin(a + 0.4) * 1, 0);
	glVertex3f(p2.X() - cos(a - 0.4) * 1, p2.Y() + sin(a - 0.4) * 1, 0);
	glEnd();
}

void Draw::Debug_Text(Point2D p, std::string text, int h, Color color)
{
	glEnable(GL_TEXTURE_2D);

	glColor4f(color.r, color.g, color.b, color.a);
	glBindTexture(GL_TEXTURE_2D, _debug_font.get_tex());
	glBegin(GL_QUADS);

	int c_w = _debug_font.get_w() / 77;
	float wp = 1 / 77.0;
	float scale = float(h) / 14.0;

	for (int i = 0; i < text.size(); i++)
	{
		int pos = ASCII_table[text[i]];
		glTexCoord2d(wp * pos, 0);
		glVertex3d(i * c_w * scale + p.X(), p.Y(), 0);

		glTexCoord2d(wp * (pos + 1), 0);
		glVertex3d(i * c_w * scale + p.X() + c_w * scale, p.Y(), 0);

		glTexCoord2d(wp * (pos + 1), 1);
		glVertex3d(i * c_w  * scale + p.X() + c_w * scale, p.Y() + _debug_font.get_h() * scale, 0);

		glTexCoord2d(wp * pos, 1);
		glVertex3d(i * c_w * scale + p.X(), p.Y() + _debug_font.get_h() * scale, 0);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Draw::TEXTURE(Point2D p, Texture & tex)
{
	glEnable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, tex.get_tex());
	glBegin(GL_QUADS);

		glTexCoord2d(0, 0);
		glVertex3d(p.X(), p.Y(), 0);

		glTexCoord2d(1, 0);
		glVertex3d(p.X() + tex.get_w(), p.Y(), 0);

		glTexCoord2d(1, 1);
		glVertex3d(p.X() + tex.get_w(), p.Y() + tex.get_h(), 0);

		glTexCoord2d(0, 1);
		glVertex3d(p.X(), p.Y() + tex.get_h(), 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Draw::TEXTURE_region(Point2D dst, double dw, double dh, Point2D src, double sw, double sh, Texture & tex)
{
	glEnable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, tex.get_tex());
	glBegin(GL_QUADS);

		glTexCoord2d(src.X(), src.Y());
		glVertex3d(dst.X(), dst.Y(), 0);

		glTexCoord2d(src.X() + double(sw) / tex.get_w(), src.Y());
		glVertex3d(dst.X() + dw, dst.Y(), 0);

		glTexCoord2d(src.X() + double(sw) / tex.get_w(), src.Y() + double(sh) / tex.get_h());
		glVertex3d(dst.X() + dw, dst.Y() + dh , 0);

		glTexCoord2d(src.X(), src.Y() + double(sh) / tex.get_h());
		glVertex3d(dst.X(), dst.Y() + dh, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
