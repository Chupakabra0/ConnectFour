#pragma once

#include <memory>
#include <thread>

#include "Board.hpp"
#include "Player.hpp"
#include "ISolver.hpp"
#include "Utils.hpp"

#include "include/color.hpp"

class Game {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) noexcept = default;
	
    explicit Game(const Player& first, const Player& second, ISolver* solver, bool isFirstPlayerFirst, bool isHotseat)
        : board_(std::make_unique<Board>()), solver_(solver),
		  firstPlayer_(std::make_shared<Player>(first)), secondPlayer_(std::make_shared<Player>(second)),
          currentPlayer_(isFirstPlayerFirst ? firstPlayer_ : secondPlayer_), isHotseat_(isHotseat) {}

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

    int LaunchGameLoop(bool isTime) {

    	
        [[maybe_unused]] std::chrono::milliseconds time(0);
        utils::ConsoleClear();
    	
        while (true) {
            if (this->currentPlayer_ == this->firstPlayer_ || this->isHotseat_) {
                this->board_->ColorPrint(std::clog,
                    { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });
                //std::clog << "Unique key: " << dye::bright_white(this->board_->ToKey()) << std::endl;
                std::clog << "Make a move. Enter the value " << 1 << "-" << this->board_->GetColumnsCount() << std::endl;

                if (!this->board_->GetHistory().empty()) {
                    std::clog << "Move was made in " << dye::green(this->board_->GetLastMove() + 1) << " column" << std::endl;
                    if (isTime) {
                        std::clog << "Time: " << dye::light_aqua(time) << std::endl;
                    }
                }
               	std::clog << Board::ColorPrintCell(this->currentPlayer_->GetCharacter(),
                    {this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter()}) << " Make move: ";
                std::clog << std::flush;

                std::string moveStr;
                std::cin >> moveStr;
                const auto move = utils::StrToInt(moveStr) - 1;

                utils::ConsoleClear();
        	
        		if (move < 0 || move >= this->board_->GetColumnsCount()) {
                    std::clog << dye::red("This column doesn't exist!") << std::endl;
                    continue;
        		}
                if (!this->currentPlayer_->MakeMove(this->board_.get(), move)) {
                    std::clog << dye::red("This column was filled!") << std::endl;
                    continue;
                }

                buffer_ += moveStr + ' ';
            }
            else {
                std::clog << dye::yellow("Computer is thinking a lot...");
                const auto begin = std::chrono::steady_clock::now();
                if (!this->currentPlayer_->MakeMove(this->board_.get(),
                    this->solver_->Solve(*this->board_, { *this->secondPlayer_, *this->firstPlayer_ }))) {
                    throw std::runtime_error("-1 turn");
                }
            	
                const auto end = std::chrono::steady_clock::now();

                time = std::chrono::duration_cast<decltype(time)>(end - begin);

                utils::ConsoleClear();
            }

            switch (this->board_->GetWinnerCharacter()) {
				case 'X': case 'O': {
	                std::clog << Board::ColorPrintCell(this->currentPlayer_->GetCharacter(),
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() }) << dye::green(" player has won!") << std::endl;
                    this->board_->ColorPrint(std::clog,
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });
					
                    std::clog << "Your moves: " << this->buffer_ << std::endl;

                    utils::ConsolePause();

	                return EXIT_SUCCESS;
	            }
                case ' ': {
	                std::clog << "It's a tie" << std::endl;
                    this->board_->ColorPrint(std::clog,
                        { this->firstPlayer_->GetCharacter(), this->secondPlayer_->GetCharacter() });
					
                    std::clog << "Your moves: " << this->buffer_ << std::endl;

                    utils::ConsolePause();
					
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
    std::unique_ptr<Board> board_;
    ISolver* solver_;

    std::string buffer_;
    std::shared_ptr<Player> firstPlayer_;
    std::shared_ptr<Player> secondPlayer_;
    std::shared_ptr<Player> currentPlayer_;

    bool isHotseat_;
};
