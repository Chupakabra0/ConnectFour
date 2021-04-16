#include "Game/Game.hpp"

int main(const int argc, const char* argv[]) {
    return Game::GetInstance(new Human(MoveCharacters::FIRST_PLAYER), new Human(MoveCharacters::SECOND_PLAYER))
        .LaunchGameLoop(argc, argv);
}
