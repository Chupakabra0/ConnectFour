#include <iostream>
#include <string>

#include "Game/Game.hpp"

int main(const int argc, const char* argv[]) {

    std::clog << Game<6, 7>::GetInstance().GetBoard() << std::endl << Game<10, 5>::GetInstance().GetBoard();

    return EXIT_SUCCESS;
}