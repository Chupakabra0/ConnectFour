#pragma once

#include <memory>

#include "PlayerFactory.hpp"
#include "Computer.hpp"

class ComputerFactory : public PlayerFactory {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    ComputerFactory() = delete;
    ComputerFactory(const ComputerFactory&) = delete;
    ComputerFactory(ComputerFactory&&) noexcept = default;
    explicit HumanFactory(MoveCharacters moveCharacter) : PlayerFactory(moveCharacter) {}

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    ComputerFactory& operator=(const ComputerFactory&) = delete;
    ComputerFactory& operator=(ComputerFactory&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~ComputerFactory() noexcept override = default;

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    Computer* CreatePlayer() override {
        return new Computer(this->moveCharacter_);
    }
};
