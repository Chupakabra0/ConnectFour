#pragma once

#include <memory>

#include "Board/Board.hpp"

template<short RowsCount, short ColumnsCount>
class Game {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) noexcept = default;

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//


    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Board() noexcept = default;

    //--------------------------------------------- STATIC SECTION ---------------------------------------------------//

    static auto& GetInstance(Human* first, Human* second) {
        static Game instance(first, second);
        return &instance;
    }

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    

private:
    explicit Game(Human* first, Human* second)
        : players_({ std::make_unique<Human>(first), std::make_unique<Human>(second) }), board_() {}

    Board<RowsCount, ColumnsCount> board_;
    std::pair<std::unique_ptr<Human>, std::unique_ptr<Human>> players_;
};
