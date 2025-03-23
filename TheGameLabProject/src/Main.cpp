#include <Game.h>
#include "Logger.h"

int main() {

	Logger::Init();

	Game game;

	game.run();

	return 0;
}



