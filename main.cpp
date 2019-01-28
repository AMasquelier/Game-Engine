#include "math.h"
#include "engine.h"

int main(int argc, char *argv[])
{
	Game_Engine engine;
	engine.Init();

	engine.MainLoop();

	return 0;
}