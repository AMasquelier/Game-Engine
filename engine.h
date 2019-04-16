#pragma once
#include "graphics.h"
#include "core.h"
#include "physics.h"
#include "inputs.h"
#include "debug.h"

class Game_Engine
{
	public:
		Game_Engine();
		void Init();
		void MainLoop();

	private:
		Window _window;
		Inputs _input;
		Physics_Engine _physics;
		void *_update_func;
		void *_display_func;


		double main_framerate = 90;
		bool close_game = false;

};