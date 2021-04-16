#pragma once

#include "Player.hpp"

#include <memory>

class PlayerFactory {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    PlayerFactory() = delete;
    PlayerFactory(const PlayerFactory&) = delete;
    PlayerFactory(PlayerFactory&&) noexcept = default;
    explicit PlayerFactory(MoveCharacters moveCharacter) : moveCharacter_(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    PlayerFactory& operator=(const PlayerFactory&) = delete;
    PlayerFactory& operator=(PlayerFactory&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~PlayerFactory() noexcept = default;

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] auto GetCharacter() const {
        return this->moveCharacter_;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    virtual Player* CreatePlayer() = 0;

protected:
    MoveCharacters moveCharacter_;
};
