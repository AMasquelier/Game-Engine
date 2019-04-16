#include "engine.h"
#include "raycasting.h"
#include "tests.h"
#include "Box2D/Box2D.h"

Game_Engine::Game_Engine()
{
}

void Game_Engine::Init()
{
	if (!init_graphics())
	{
		close_game = true;
		std::cout << "Failed to init graphics" << std::endl;
	}
	if (!_window.create("Game Engine", 1080, 720, 200, 120))
	{
		close_game = true;
		std::cout << "Failed to create window" << std::endl;
	}
	Draw::Init();
}

void Game_Engine::MainLoop()
{
	Clock framerate_timer; framerate_timer.start();

	_physics.add_box_rigidbody(10, 2, 5, 5);
	_physics.add_box_rigidbody(17, -2, 5, 5);
	_physics.add_box_rigidbody(23, 2, 5, 5);
	_physics.add_box_rigidbody(5, -2, 5, 5);
	_physics.add_box_rigidbody(5, -10, 5, 5);
	_physics.add_box_rigidbody(11, -10, 5, 5);
	_physics.add_box_rigidbody(27, -10, 5, 5);
	_physics.add_box_rigidbody(23, -20, 5, 5);
	_physics.add_box_rigidbody(17, -16, 5, 5);
	_physics.add_collider(0, 30, 80, 5);

	bool debug_mode = true;
	

	while (!close_game)
	{
		if (framerate_timer.duration() >= 1000000.0 / main_framerate)
		{
			double act_fps = ceilf(1000000.0 / framerate_timer.duration());
			framerate_timer.start();
			// Inputs
			_input.UpdateControllerInputs(true);
			_input.UpdateKeyboardInputs(true);
			if (_input.CloseGame) close_game = true;

			if (_input.pushedInput(_input.F4))		debug_mode = !debug_mode;
			if (_input.GetInput(_input.Space)) _physics.set_dt(1 / 600.0);
			else _physics.set_dt(1 / 60.0);

			// Update game objects
			_physics.run();

			// Rendering
			_window.clear();

			//player.display();

			if (debug_mode)
			{
				_physics.debug_display();
				Draw::Debug_Text(Vector2(1000, 10), to_string(int(act_fps)) + " FPS", 14, rgb_color(1, 1, 1));
			}
			_window.update();
		}
		else Clock::sleep(1000.0 / main_framerate - framerate_timer.duration() * 0.001);
	}
}
