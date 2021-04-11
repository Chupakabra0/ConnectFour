#pragma once

#include "Board/Board.hpp"

class Human {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Human() = delete;
    Human(const Human&) = delete;
    Human(Human&&) noexcept = default;

    explicit Human(MoveCharacters moveCharacter) : moveCharacter_(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Human& operator=(const Human&) = delete;
    Human& operator=(Human&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Human() noexcept = default;

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] auto GetCharacter() const {
        return this->moveCharacter_;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

private:
    MoveCharacters moveCharacter_;
};
