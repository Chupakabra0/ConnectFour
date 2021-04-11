#include "Game/Game.hpp"

int main(const int argc, const char* argv[]) {
    return Game<6, 7>::GetInstance().LaunchGameLoop(argc, argv);
}