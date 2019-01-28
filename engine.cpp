#include "engine.h"
#include "raycasting.h"
#include "tests.h"
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

	BoxCollider c;
	c.setAABB(Vector2(250, 600), Vector2(1200, 700));
	_physics.add_collider(c);

	c.setAABB(Vector2(900, 50), Vector2(1000, 700));
	c.set_restitution(0.8);
	_physics.add_collider(c);

	c.setAABB(Vector2(200, 50), Vector2(300, 700));
	c.set_restitution(0.8);
	_physics.add_collider(c);

	Inspector *ins;
	Rigidbody *rb2 = _physics.get_rigidbody(0);

	Character player(Vector2(500, 500));


	bool debug_mode = false;

	while (!close_game)
	{
		if (framerate_timer.duration() > 1000000.0 / main_framerate)
		{
			double act_fps = ceilf(1000000.0 / framerate_timer.duration());
			framerate_timer.start();
			// Inputs
			_input.UpdateControllerInputs(true);
			_input.UpdateKeyboardInputs(true);
			if (_input.CloseGame) close_game = true;

			if (_input.pushedInput(_input.F4))		debug_mode = !debug_mode;

			if (_input.GetInput(_input.Space))		player.jump(0.8);
			if (_input.GetInput(_input.Q))		player.go_left(0.8);
			if (_input.GetInput(_input.D))		player.go_right(0.8);


			// Update game objects
			_physics.run();
			player.update();

			// Rendering
			_window.clear();

			player.display();

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
