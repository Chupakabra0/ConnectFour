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
        return *this->board_;
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

    [[nodiscard]] auto* GetCurrentPlayer() const {
        return this->currentPlayer_.get();
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    void SwitchPlayer() {
        if (this->currentPlayer_ == this->players_.first) {
            this->currentPlayer_ = this->players_.second;
        }
        else {
            this->currentPlayer_ = this->players_.first;
        }
    }

    int LaunchGameLoop(const int argc, const char* argv[]) {
        this->Cls();

        while (true) {
            short turn;
            std::clog << *this->board_ << std::endl << "Your turn: ";
            std::cin >> turn;
            --turn;

            if (turn < static_cast<short>(Column::FIRST) || turn > static_cast<short>(Column::SEVENTH)) {
                this->Cls();
                std::clog << "Please, enter correct number between " << static_cast<short>(Column::FIRST) - 1
                    << " and " << static_cast<short>(Column::SEVENTH) - 1 << std::endl;
                continue;
            }

            this->board_->MakeMove(Column(turn), this->currentPlayer_.get());
            this->SwitchPlayer();
        }

        return EXIT_SUCCESS;
    };

private:
    Game()
        : players_({ std::make_unique<Human>(MoveCharacters::FIRST_PLAYER),
            std::make_unique<Human>(MoveCharacters::SECOND_PLAYER) }),
            currentPlayer_(this->players_.first) {
        this->board_ = std::make_unique<Board<RowsCount, ColumnsCount>>();
    }

    void Cls() {
        #if defined(WIN32)
            system("cls");
        #else
            system("clear");
        #endif
    }

    std::unique_ptr<Board<RowsCount, ColumnsCount>> board_;
    std::pair<std::shared_ptr<Human>, std::shared_ptr<Human>> players_;
    std::shared_ptr<Human> currentPlayer_;
};
