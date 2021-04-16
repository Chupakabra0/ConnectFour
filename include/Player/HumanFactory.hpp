#pragma once

#include <memory>

#include "PlayerFactory.hpp"
#include "Human.hpp"

class HumanFactory : public PlayerFactory {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    HumanFactory() = delete;
    HumanFactory(const HumanFactory&) = delete;
    HumanFactory(HumanFactory&&) noexcept = default;
    explicit HumanFactory(MoveCharacters moveCharacter) : PlayerFactory(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    HumanFactory& operator=(const HumanFactory&) = delete;
    HumanFactory& operator=(HumanFactory&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~HumanFactory() noexcept override = default;

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    Human* CreatePlayer() override {
        return new Human(this->moveCharacter_);
    }
};
