#pragma once

#include "MoveCharacters/MoveCharacters.hpp"
#include "Board/Board.hpp"

class Player {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Player() = delete;
    Player(const Player&) = delete;
    Player(Player&&) noexcept = default;
    explicit Player(MoveCharacters moveCharacter) : moveCharacter_(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    virtual ~Player() noexcept = default;

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] auto GetCharacter() const {
        return this->moveCharacter_;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    virtual Column MakeMove(Board* board) = 0;

protected:
    MoveCharacters moveCharacter_;
};