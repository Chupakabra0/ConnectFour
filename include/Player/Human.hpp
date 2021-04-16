#pragma once

#include "Player.hpp"

class Human final : public Player {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Human() = delete;
    Human(const Human&) = delete;
    Human(Human&&) noexcept = default;
    explicit Human(MoveCharacters moveCharacter) : Player(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Human& operator=(const Human&) = delete;
    Human& operator=(Human&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Human() noexcept override = default;

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    Column MakeMove(Board* board) override {
        short turn = -1;
        do {
            std::cin >> turn;
            --turn;
        } while (turn < static_cast<short>(Column::FIRST) || turn > board->GetColumns());

        return Column(turn);
    }
};
