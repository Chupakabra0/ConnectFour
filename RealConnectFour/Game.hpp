#pragma once

#include <memory>
#include <sstream>
#include <fstream>
#include <thread>

#include "Board.hpp"
#include "Player.hpp"
#include "ISolver.hpp"

class Game {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) noexcept = default;
	
    explicit Game(const Player& first, const Player& second, ISolver* solver)
        : board_(std::make_unique<Board>()), solver_(solver),
		  firstPlayer_(std::make_shared<Player>(first)), secondPlayer_(std::make_shared<Player>(second)),
          currentPlayer_(firstPlayer_) {}

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

        std::chrono::seconds time(0);
    	
    	this->Clear();

    	std::clog << "Go first: ";
		std::string temp;
        std::cin >> temp;
        std::ranges::transform(temp.begin(), temp.end(), temp.begin(), tolower);
    	
    	if (temp == "y" || temp == "yes" || temp == "1" || temp == "+") {
            this->currentPlayer_ = this->firstPlayer_;
    	}
    	else {
            this->currentPlayer_ = this->secondPlayer_;
        }

    	this->Clear();
    	
        while (true) {
            if (this->currentPlayer_ == this->firstPlayer_) {
                this->board_->ColorPrint(std::clog,
                    { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });
                std::clog << "Unique key: " << dye::bright_white(this->board_->ToKey()) << std::endl;
            	
                if (!this->board_->GetHistory().empty()) {
                    std::clog << "Move was made in " << dye::green(this->board_->GetLastMove() + 1) << " column" << std::endl;
                    std::clog << "Time: " << dye::light_aqua(time) << std::endl;
                }
               	std::clog << Board::ColorPrintCell(this->currentPlayer_->GetCharacter(),
                    {this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter()}) << " Make move: ";
                std::clog << std::flush;
            	
        		short move;
                std::cin >> move;
                --move;

                this->Clear();
        	
        		if (move < 0 || move >= this->board_->GetColumnsCount()) {
                    std::clog << "This column doesn't exist!" << std::endl;
                    continue;
        		}
                if (!this->currentPlayer_->MakeMove(this->board_.get(), move)) {
                    std::clog << "This column was filled!" << std::endl;
                    continue;
                }
            }
            else {
                std::clog << dye::yellow("Computer is thinking a lot...");
                const auto begin = std::chrono::steady_clock::now();
                if (!this->currentPlayer_->MakeMove(this->board_.get(),
                    this->solver_->Solve(*this->board_, { *this->secondPlayer_, *this->firstPlayer_ }))) {
                    throw std::runtime_error("-1 turn");
                }
            	
                const auto end = std::chrono::steady_clock::now();

                time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

                this->Clear();
            }

            switch (this->board_->GetWinnerCharacter()) {
				case 'X': case 'O': {
	                std::clog << Board::ColorPrintCell(this->currentPlayer_->GetCharacter(),
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() }) << " player has won!" << std::endl;
                    this->board_->ColorPrint(std::clog,
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });

                    //for (auto i = 0u; i < this->board_->GetHistory().size(); ++i) {
                    //    i % 2u == 0u ?
                    //        std::clog << dye::red(this->board_->GetHistory()[i]) :
                    //        std::clog << dye::blue(this->board_->GetHistory()[i]);
                    //}
                    //std::clog << std::endl;
					
                    this->Pause();
					
	                return EXIT_SUCCESS;
	            }
                case ' ': {
	                std::clog << "It's a tie" << std::endl;
                    this->board_->ColorPrint(std::clog,
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });

                    //for (auto i = 0u; i < this->board_->GetHistory().size(); ++i) {
                    //    i % 2u == 0u ?
                    //        std::clog << dye::red(this->board_->GetHistory()[i]) :
                    //	    std::clog << dye::blue(this->board_->GetHistory()[i]);
                    //}
                    //std::clog << std::endl;
					
                    this->Pause();
					
	                return EXIT_SUCCESS;
	            }
            	default: {
					break;
				}
            }

            this->SwitchPlayer();
        }
    }

private:

    // TODO REMOVE IT FROM THIS CLASS
    void Clear() {
        std::clog << dye::white('\n');
		#if defined(_WIN32)
			system("cls");
		#else
			system("clear");
		#endif
    }
    //

    // TODO REMOVE IT FROM THIS CLASS
    void Pause() {
		#if defined(_WIN32)
			system("pause");
		#endif
    }
    //

    std::unique_ptr<Board> board_;
    ISolver* solver_;

    std::shared_ptr<Player> firstPlayer_;
    std::shared_ptr<Player> secondPlayer_;
    std::shared_ptr<Player> currentPlayer_;
};
