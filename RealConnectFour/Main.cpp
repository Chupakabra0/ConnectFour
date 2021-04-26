#include <memory>

#include "Game.hpp"

int main(const int argc, const char* argv[]) {
	const auto firstPlayer  = std::make_shared<Player>('X');
	const auto secondPlayer = std::make_shared<Player>('O');

	auto game         = std::make_unique<Game>(*firstPlayer, *secondPlayer);

	const auto returnCode = game->LaunchGameLoop(argc, argv);

	return returnCode;
}