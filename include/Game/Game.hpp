#pragma once

#include <forward_list>
#include <functional>
#include <memory>
#include <numeric>

#include "Board/Board.hpp"

class Game {
    enum class WinnerCode : short {
        NONE,
        TIE,
        WIN
    };
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

            this->Cls();

            if (turn < static_cast<short>(Column::FIRST) || turn > static_cast<short>(Column::SEVENTH)) {
                std::clog << "Please, enter correct number between " << static_cast<short>(Column::FIRST) + 1
                    << " and " << static_cast<short>(Column::SEVENTH) + 1 << std::endl;
                continue;
            }

            if (!this->board_->MakeMove(Column(turn), this->currentPlayer_.get())) {
                std::clog << "This column was filled!" << std::endl;
            }
            
            switch (this->GetWinner()) {
                case WinnerCode::WIN: {
                    if (this->currentPlayer_ == this->players_.first) {
                        std::clog << "First player has won!" << std::endl;
                    }
                    else {
                        std::clog << "Second player has won!" << std::endl;
                    }
                    std::clog << *this->board_ << std::endl;

                    return EXIT_SUCCESS;
                }
                case WinnerCode::TIE: {
                    std::clog << "It's a tie" << std::endl;
                    std::clog << *this->board_ << std::endl;

                    return EXIT_SUCCESS;
                }
            }

            this->SwitchPlayer();
        }
    };

private:
    Game()
        : players_({ std::make_unique<Human>(MoveCharacters::FIRST_PLAYER),
            std::make_unique<Human>(MoveCharacters::SECOND_PLAYER) }),
            currentPlayer_(this->players_.first), board_(std::make_unique<Board>()) {}

    WinnerCode GetWinner() {
        #pragma region hardcoded winning combos
        const std::vector<std::array<std::pair<Row, Column>, 4>> WINNING_COMBOS = {
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FIRST),
                    std::pair(Row::SECOND, Column::FIRST),
                    std::pair(Row::THIRD, Column::FIRST),
                    std::pair(Row::FOURTH, Column::FIRST)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FIRST),
                    std::pair(Row::THIRD, Column::FIRST),
                    std::pair(Row::FOURTH, Column::FIRST),
                    std::pair(Row::FIFTH, Column::FIRST)
            },
            std::array<std::pair<Row, Column>, 4> {
	                std::pair(Row::THIRD, Column::FIRST),
	                std::pair(Row::FOURTH, Column::FIRST),
	                std::pair(Row::FIFTH, Column::FIRST),
	                std::pair(Row::SIXTH, Column::FIRST)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::SECOND),
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::SECOND)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::SECOND)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::SECOND),
                    std::pair(Row::SIXTH, Column::SECOND)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::THIRD),
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::THIRD)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::THIRD)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::SIXTH, Column::THIRD)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::SIXTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FIFTH),
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::FIFTH),
                    std::pair(Row::SIXTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::SIXTH),
                    std::pair(Row::SECOND, Column::SIXTH),
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::FOURTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::SIXTH),
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::FOURTH, Column::SIXTH),
                    std::pair(Row::FIFTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::FOURTH, Column::SIXTH),
                    std::pair(Row::FIFTH, Column::SIXTH),
                    std::pair(Row::SIXTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::SEVENTH),
                    std::pair(Row::SECOND, Column::SEVENTH),
                    std::pair(Row::THIRD, Column::SEVENTH),
                    std::pair(Row::FOURTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::SEVENTH),
                    std::pair(Row::THIRD, Column::SEVENTH),
                    std::pair(Row::FOURTH, Column::SEVENTH),
                    std::pair(Row::FIFTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::SEVENTH),
                    std::pair(Row::FOURTH, Column::SEVENTH),
                    std::pair(Row::FIFTH, Column::SEVENTH),
                    std::pair(Row::SIXTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FIRST),
                    std::pair(Row::FIRST, Column::SECOND),
                    std::pair(Row::FIRST, Column::THIRD),
                    std::pair(Row::FIRST, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::SECOND),
                    std::pair(Row::FIRST, Column::THIRD),
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::FIRST, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::THIRD),
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::FIRST, Column::FIFTH),
                    std::pair(Row::FIRST, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::FIRST, Column::FIFTH),
                    std::pair(Row::FIRST, Column::SIXTH),
                    std::pair(Row::FIRST, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FIRST),
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::SECOND, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::SECOND, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::SECOND, Column::SIXTH),
                    std::pair(Row::SECOND, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FIRST),
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::THIRD, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::THIRD, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::FIRST),
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::SIXTH),
                    std::pair(Row::FOURTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::FIRST),
                    std::pair(Row::FIFTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::SIXTH),
                    std::pair(Row::FIFTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::FIRST),
                    std::pair(Row::SIXTH, Column::SECOND),
                    std::pair(Row::SIXTH, Column::THIRD),
                    std::pair(Row::SIXTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::SECOND),
                    std::pair(Row::SIXTH, Column::THIRD),
                    std::pair(Row::SIXTH, Column::FOURTH),
                    std::pair(Row::SIXTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::THIRD),
                    std::pair(Row::SIXTH, Column::FOURTH),
                    std::pair(Row::SIXTH, Column::FIFTH),
                    std::pair(Row::SIXTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::FOURTH),
                    std::pair(Row::SIXTH, Column::FIFTH),
                    std::pair(Row::SIXTH, Column::SIXTH),
                    std::pair(Row::SIXTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FIRST),
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::SIXTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FIRST),
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::SIXTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FIRST),
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::SECOND),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH),
                    std::pair(Row::SIXTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::SECOND),
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::SIXTH),
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::FIFTH, Column::SIXTH),
                    std::pair(Row::SIXTH, Column::SEVENTH),
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::THIRD),
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::SIXTH),
                    std::pair(Row::FIFTH, Column::SEVENTH),
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::FOURTH, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FOURTH),
                    std::pair(Row::SECOND, Column::THIRD),
                    std::pair(Row::THIRD, Column::SECOND),
                    std::pair(Row::FOURTH, Column::FIRST)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::FOURTH),
                    std::pair(Row::FIFTH, Column::FIFTH),
                    std::pair(Row::FOURTH, Column::SIXTH),
                    std::pair(Row::THIRD, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIRST, Column::FIFTH),
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::SECOND)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SECOND, Column::FOURTH),
                    std::pair(Row::THIRD, Column::THIRD),
                    std::pair(Row::FOURTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::FIRST)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::FIRST),
                    std::pair(Row::FIFTH, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::SECOND),
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::THIRD),
                    std::pair(Row::THIRD, Column::FOURTH),
                    std::pair(Row::SECOND, Column::FIFTH),
                    std::pair(Row::FIRST, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::SECOND),
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::THIRD),
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::SECOND, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FOURTH, Column::FOURTH),
                    std::pair(Row::THIRD, Column::FIFTH),
                    std::pair(Row::SECOND, Column::SIXTH),
                    std::pair(Row::FIRST, Column::SEVENTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::SIXTH, Column::THIRD),
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::THIRD, Column::SIXTH)
            },
            std::array<std::pair<Row, Column>, 4> {
                    std::pair(Row::FIFTH, Column::FOURTH),
                    std::pair(Row::FOURTH, Column::FIFTH),
                    std::pair(Row::THIRD, Column::SIXTH),
                    std::pair(Row::SECOND, Column::SEVENTH)
            },
        };
        #pragma endregion

//         auto i = 0;
        for (const auto& combo : WINNING_COMBOS) {
            const auto temp = std::array {
                this->board_->GetCell(combo[0].first, combo[0].second) != MoveCharacters::NONE,
                this->board_->GetCell(combo[0].first, combo[0].second) == this->board_->GetCell(combo[1].first, combo[1].second),
                this->board_->GetCell(combo[1].first, combo[1].second) == this->board_->GetCell(combo[2].first, combo[2].second),
                this->board_->GetCell(combo[2].first, combo[2].second) == this->board_->GetCell(combo[3].first, combo[3].second)
            };

//            ++i;
            //std::clog << static_cast<char>(this->board_->GetCell(combo[0].first, combo[0].second)) << " "
        			 // << static_cast<char>(this->board_->GetCell(combo[1].first, combo[1].second)) << " "
        		  //    << static_cast<char>(this->board_->GetCell(combo[2].first, combo[2].second)) << " "
        		  //    << static_cast<char>(this->board_->GetCell(combo[3].first, combo[3].second)) << std::endl;
        	
            if (std::accumulate(temp.begin(), temp.end(), true, std::bit_and<>()) == true) {
//                std::clog << i << std::endl;
                return WinnerCode::WIN;
            }
        }

        if (std::find(this->board_->GetField().cbegin(), this->board_->GetField().cend(), MoveCharacters::NONE)
                != this->board_->GetField().cend()) {
            return WinnerCode::NONE;
        }

        return WinnerCode::TIE;
    }

    // TODO REMOVE IT FROM THIS CLASS
    void Cls() {
        #if defined(WIN32)
            system("cls");
        #else
            system("clear");
        #endif
    }
    //

    std::unique_ptr<Board> board_;
    std::pair<std::shared_ptr<Human>, std::shared_ptr<Human>> players_;
    std::shared_ptr<Human> currentPlayer_;
};
