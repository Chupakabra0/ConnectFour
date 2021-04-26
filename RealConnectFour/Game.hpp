#pragma once

#include <array>
#include <memory>
#include <sstream>
#include <fstream>
#include <thread>

#include "Board.hpp"
#include "Player.hpp"
#include "Enums.hpp"
#include "Solver.hpp"

class Game {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) noexcept = default;
	
    explicit Game(const Player& first, const Player& second)
        : board_(std::make_unique<Board>()), firstPlayer_(std::make_shared<Player>(first)),
		  secondPlayer_(std::make_shared<Player>(second)), currentPlayer_(firstPlayer_) {}


    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) noexcept = default;

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Game() noexcept = default;

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] const auto& GetBoard() const {
        return *this->board_;
    }

    [[nodiscard]] auto* GetFirstPlayer() const {
        return this->firstPlayer_.get();
    }

    [[nodiscard]] auto* GetSecondPlayer() const {
        return this->secondPlayer_.get();
    }

    [[nodiscard]] auto* GetPlayerByIndex(const short index) const {
        return index == 1 ? this->GetFirstPlayer() : this->GetSecondPlayer();
    }

    [[nodiscard]] auto* GetCurrentPlayer() const {
        return this->currentPlayer_.get();
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    void SwitchPlayer() {
        if (this->currentPlayer_ == this->firstPlayer_) {
            this->currentPlayer_ = this->secondPlayer_;
        }
        else {
            this->currentPlayer_ = this->firstPlayer_;
        }
    }

    int LaunchGameLoop([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
        Solver solver;

        std::stringstream ss;
        ss << std::this_thread::get_id;
        std::fstream fileLog(std::string("log") + ss.str() + ".log", std::ios_base::out);
    	this->Cls();

        while (true) {
            //if (this->currentPlayer_ == this->firstPlayer_) {
                std::clog << *this->board_ << std::endl;

                const auto score = solver.Solve
        			(*this->board_, { *this->firstPlayer_, *this->secondPlayer_ }
                       , -static_cast<int>(this->board_->GetSize()) - 1, static_cast<int>(this->board_->GetSize()) + 1);

                std::clog << "Points: " << score.amount << std::endl;
                std::clog << "Best move: " << score.bestMove + 1 << std::endl;
                std::clog << "Node: " << solver.GetNodeCount() << std::endl;
                std::clog << this->currentPlayer_->GetCharacter() << " Make move: ";

        		short move;
                std::cin >> move;
                fileLog << *this->board_ << std::endl;
                fileLog << "Move: " << move << std::endl;
                fileLog << "Points: " << score.amount << std::endl;
                fileLog << "Best move: " << score.bestMove + 1 << std::endl;
                fileLog << "Node: " << solver.GetNodeCount() << std::endl;
                --move;
        	
                this->Cls();
        	
        		if (move < 0 || move > this->board_->GetColumns()) {
                    std::clog << "This column doesn't exist!" << std::endl;
                    continue;
        		}
                if (!this->currentPlayer_->MakeMove(this->board_.get(), move)) {
                    std::clog << "This column was filled!" << std::endl;
                    continue;
                }
            //}
            //else {
	           // 
            //}

            switch (this->board_->GetWinner()) {
	            case WinnerCode::WIN: {
	                std::clog << static_cast<char>(this->currentPlayer_->GetCharacter()) << " player has won!" << std::endl;
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
    std::shared_ptr<Player> firstPlayer_;
    std::shared_ptr<Player> secondPlayer_;
    std::shared_ptr<Player> currentPlayer_;
};
