#include "Game/Game.hpp"

int main(const int argc, const char* argv[]) {
    return Game::GetInstance().LaunchGameLoop(argc, argv);
}