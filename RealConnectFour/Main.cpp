#include <memory>

#include "Game.hpp"
#include "Solver.hpp"

int main(const int argc, const char* argv[]) {
	const auto firstPlayer  = std::make_shared<Player>('X');
	const auto secondPlayer = std::make_shared<Player>('O');

	std::string temp;
	std::clog << "Choose AI depth: ";
	std::cin >> temp;
	const auto depth = static_cast<int>(strtol(temp.c_str(), nullptr, 10));

	const auto solver = std::static_pointer_cast<ISolver>(std::make_shared<ClassicSolver>(depth));

	auto game         = std::make_unique<Game>(*firstPlayer, *secondPlayer, solver.get());

	const auto returnCode = game->LaunchGameLoop(argc, argv);

	return returnCode;
}
