#define FMT_HEADER_ONLY
#include <filesystem>
#include <memory>

#include "Game.hpp"
#include "Solver.hpp"
#include "Utils.hpp"

#include "include/cxxopts.hpp"

cxxopts::Options OptionsSetup([[maybe_unused]] const int argc, const char* argv[]) {
	auto options = cxxopts::Options(std::filesystem::path(std::string(argv[0])).filename().string(), "Connect four game with minimax AI implementation");
	options.add_options()
		("d, depth", "Depth of the AI", cxxopts::value<int>())
		("hotseat", "Play with a human on one PC", cxxopts::value<bool>())
		("f, first", "You go first", cxxopts::value<bool>())
		("s, second", "You go second", cxxopts::value<bool>())
		("t, time", "Fixing time of the turn", cxxopts::value<bool>())
		("h, help", "Help", cxxopts::value<bool>());
	options.allow_unrecognised_options();

	return options;
}

int main(const int argc, const char* argv[]) {
	const auto firstPlayer  = std::make_shared<Player>(PlayerSymbol::FIRST);
	const auto secondPlayer = std::make_shared<Player>(PlayerSymbol::SECOND);

	auto options = std::make_unique<cxxopts::Options>(OptionsSetup(argc, argv));

	std::string temp;
	const auto result = std::make_unique<cxxopts::ParseResult>(options->parse(argc, argv));

	auto depth     = 0;
	auto isHotseat = false;
	auto isFirst   = true;
	auto isTime    = false;

	if (result->count("help")) {
		std::clog << options->help() << std::endl;

		return EXIT_SUCCESS;
	}

	if (!result->count("depth") && !result->count("hotseat")) {
		std::clog << "Is hotseat: ";
		std::cin >> temp;

		isHotseat = temp == "y" || temp == "yes" || temp == "true" || temp == "t" || temp == "+";
	}
	else if (result->count("hotseat")) {
		isHotseat = (*result)["hotseat"].as<bool>();
	}

	if (!isHotseat) {
		if (!result->count("depth")) {
			std::clog << "Choose AI depth: ";
			std::cin >> temp;

			depth = static_cast<int>(strtol(temp.c_str(), nullptr, 10));
		}
		else {
			depth = (*result)["depth"].as<int>();
		}

		if (!result->count("first") && !result->count("second")) {
			std::clog << "Go first: ";
			std::cin >> temp;

			isFirst = temp == "y" || temp == "yes" || temp == "true" || temp == "t" || temp == "+";
		}
		else if (result->count("first")) {
			isFirst = (*result)["first"].as<bool>();
		}
		else {
			isFirst = !(*result)["second"].as<bool>();
		}
	}

	utils::ConsoleClear();
	
	const auto solver = std::static_pointer_cast<ISolver>(std::make_shared<ClassicSolver>(depth));
	auto game         = std::make_unique<Game>(*firstPlayer, *secondPlayer, solver.get(), isFirst, isHotseat);

	const auto returnCode = game->LaunchGameLoop(result->count("time"));

	return returnCode;
}
