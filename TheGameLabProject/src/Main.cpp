#include "Logging/Logger.h"
#include "Game.h"

int main() {

	Logger::Init();

	Game game;

	game.run();

	return 0;
}



