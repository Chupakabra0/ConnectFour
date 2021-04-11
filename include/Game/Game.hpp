#pragma once

#include <memory>

#include "Board/Board.hpp"

template<short RowsCount, short ColumnsCount>
class Game {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Game(const Game&) = delete;
    Game(Game&&) noexcept = default;

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//


    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Game() noexcept = default;

    //--------------------------------------------- STATIC SECTION ---------------------------------------------------//

    static auto& GetInstance() {
        static Game instance;
        return instance;
    }

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] const auto& GetBoard() const {
        return this->board_;
    }

    [[nodiscard]] auto* GetFirstPlayer() const {
        return this->players_.first.get();
    }

    [[nodiscard]] auto* GetSecondPlayer() const {
        return this->players_.second.get();
    }

    [[nodiscard]] auto* GetPlayerByIndex(const short index) const {
        return index == 1 ? this->GetFirstPlayer() : this->GetSecondPlayer();
    }

private:
    Game()
        : players_({ std::make_unique<Human>(MoveCharacters::FIRST_PLAYER),
            std::make_unique<Human>(MoveCharacters::SECOND_PLAYER) }), board_() {}

    Board<RowsCount, ColumnsCount> board_;
    std::pair<std::unique_ptr<Human>, std::unique_ptr<Human>> players_;
};
