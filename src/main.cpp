#include <iostream>
#include <memory>

#include "Board/Board.hpp"

int main(const int argc, const char* argv[]) {
    const auto board = std::make_unique<Board<6, 7>>();

    std::clog << *board;

    return EXIT_SUCCESS;
}