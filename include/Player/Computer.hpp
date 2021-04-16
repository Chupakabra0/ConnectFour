#pragma once

#include "Player.hpp"

class Computer : public Player {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Computer() = delete;
    Computer(const Computer&) = delete;
    Computer(Computer&&) noexcept = default;
    explicit Computer(MoveCharacters moveCharacter) : Player(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Computer& operator=(const Computer&) = delete;
    Computer& operator=(Computer&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Computer() noexcept = default;

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

};
