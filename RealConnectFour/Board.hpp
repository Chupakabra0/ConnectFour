#pragma once
#ifndef NOMINMAX
    #define NOMINMAX
#endif

#include <array>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>
#include <deque>

#include "Enums.hpp"
#include "Utils.hpp"

//#include "include/color.hpp"
#include <fmt/color.h>

class Board {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    explicit Board() {
        this->field_.assign(this->rowsCount_ * this->columnsCount_, ' ');
		this->field_.shrink_to_fit();
    }

    Board(const Board&) = default;
    Board(Board&&) noexcept = default;

	//----------------------------------------------- DTOR SECTION ---------------------------------------------------//

	~Board() noexcept = default;
	
    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Board& operator=(const Board&) = default;
    Board& operator=(Board&&) noexcept = default;

    void ColorPrint(const std::pair<char, char>& playerSymbols) {
        if (this->field_.empty()) {
            return;
        }

		fmt::print("| {} | ", 1);
		for (auto i = 1; i < this->columnsCount_; ++i) {
			fmt::print("{} | ", i + 1);
		}
		fmt::print("\n{}\n", utils::GetDelimiter('-', 4 * this->columnsCount_ + this->columnsCount_ / 7));
    	
        for (auto i = 0; i < this->rowsCount_; ++i) {

        	fmt::print("| ");
			Board::ColorPrintCell(this->GetCell(i, 0), playerSymbols);
        	fmt::print(" | ");
        	
            for (auto j = 1; j < this->columnsCount_; ++j) {
				Board::ColorPrintCell(this->GetCell(i, j), playerSymbols);
            	fmt::print(" | ");
            }

			fmt::print("\n{}", utils::GetDelimiter('-', 4 * this->columnsCount_ + this->columnsCount_ / 7));
            if (i != this->rowsCount_ - 1) {
				fmt::print("\n");
            }
        }

		fmt::print("\n");
    }

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] const auto& GetField() const {
        return this->field_;
    }

    [[nodiscard]] size_t GetSize() const {
        return this->field_.size();
    }

    [[nodiscard]] auto GetRowsCount() const {
        return this->rowsCount_;
    }

    [[nodiscard]] auto GetColumnsCount() const {
        return this->columnsCount_;
    }

    [[nodiscard]] const auto& GetHistory() const {
        return this->historyMoves_;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    [[nodiscard]] short GetLastMove() const {
        return this->historyMoves_.empty() ? -1 : this->historyMoves_.back();
    }
	
    [[nodiscard]] char GetCell(const short row, const short column) const {
        if (static_cast<short>(row) >= this->rowsCount_) {
            throw std::out_of_range("Row index out of boundaries.");
        }
        if (static_cast<short>(column) >= this->columnsCount_) {
            throw std::out_of_range("Column index out of boundaries.");
        }

        return this->field_[static_cast<size_t>(row) * this->columnsCount_ + static_cast<size_t>(column)];
    }

	template<class OutIter>
	[[nodiscard]] bool TryGetRow(const short row, OutIter result) const {
		if (row >= this->rowsCount_) {
			return false;
		}
		
		for (auto i = row * this->columnsCount_; i < (row + 1) * this->columnsCount_; ++i) {
			*result++ = this->field_[i];
		}

		return true;
    }

	template<class OutIter>
	[[nodiscard]] bool TryGetColumn(const short column, OutIter result) const {
		if (column >= this->columnsCount_) {
			return false;
		}
        
		for (short i = column; i < this->GetSize(); i += this->columnsCount_) {
            *result++ = this->field_[i];
        }
	    	
        return true;
    }

	template<class OutIter>
	bool TryGetAvailableMoves(OutIter result) const {
		try {
			for (auto i = 0; i < this->columnsCount_; ++i) {
				if (this->field_[i] == ' ') {
					*result++ = i;
				}
			}

			return true;
		}
		catch (const std::exception&) {
			return false;
		}
	}

    [[nodiscard]] char GetWinnerCharacter() const {
        for (const auto& combo : this->WINNING_COMBOS) {
            const auto temp = std::array{
                this->GetCell(static_cast<short>(combo[0].first), static_cast<short>(combo[0].second)) != ' ',
                this->GetCell(static_cast<short>(combo[0].first), static_cast<short>(combo[0].second)) == this->GetCell(static_cast<short>(combo[1].first), static_cast<short>(combo[1].second)),
                this->GetCell(static_cast<short>(combo[1].first), static_cast<short>(combo[1].second)) == this->GetCell(static_cast<short>(combo[2].first), static_cast<short>(combo[2].second)),
                this->GetCell(static_cast<short>(combo[2].first), static_cast<short>(combo[2].second)) == this->GetCell(static_cast<short>(combo[3].first), static_cast<short>(combo[3].second))
            };

            if (std::accumulate(temp.begin(), temp.end(), true, std::bit_and<>())) {
                return this->GetCell(static_cast<short>(combo.begin()->first), static_cast<short>(combo.begin()->second));
            }
        }

        if (this->IsTie()) {
            return ' ';
        }

    	return '=';
    }

	[[nodiscard]] uint64_t ToKey() const {
		auto seed = this->field_.size();
    	
		for (const auto& i : this->field_) {
			seed ^= i + 2654435769ull + (seed << 6ull) + (seed >> 2ull);
		}
    	
		return seed;
	}

    bool MakeMove(short column, char moveSymbol) {
        auto index = static_cast<int>(column);
        while (index < this->GetSize() && this->field_[index] == ' ') {
            index += this->columnsCount_;
        }

        index -= this->columnsCount_;

        if (index < 0) {
            return false;
        }

        this->field_[index] = moveSymbol;
        this->historyMoves_.push_back(column);
        
		return true;
    }

    [[nodiscard]] bool IsTie() const {
        return this->GetNumberOfMoves() == this->field_.size();
    }

	[[nodiscard]] bool IsWinningMove(const short column, const char character) const {
		auto copy = *this;
		return copy.IsWinningMove(column, character);
	}

	[[nodiscard]] bool IsWinningMove(const short column, const char character) {
		this->MakeMove(column, character);
		const auto isWin = this->GetWinnerCharacter() == character;
		this->CancelLastMove();

		return isWin;
	}

	[[nodiscard]] unsigned GetNumberOfMoves() const {
        return this->historyMoves_.size();
    }

    static void ColorPrintCell(const char cell, const std::pair<char, char>& playerSymbols) {
        return cell == playerSymbols.first
    		? fmt::print(fg(fmt::color::dark_red), "{}", cell)
    		: cell == playerSymbols.second
    			? fmt::print(fg(fmt::color::green_yellow), "{}", cell)
    			: fmt::print(fg(fmt::color::white), "{}", cell);
    }

	bool CancelLastMove() {
		return this->CancelMove(this->historyMoves_.back());
	}

private:
    short rowsCount_ = 6, columnsCount_ = 7;
    std::vector<char> field_;
    std::deque<short> historyMoves_;

	bool CancelMove(short column) {
		auto index = static_cast<int>(column);
		while (index < this->GetSize() && this->field_[index] == ' ') {
			index += this->columnsCount_;
		}

		if (index >= this->GetSize()) {
			return false;
		}

		this->field_[index] = ' ';

		auto it = std::ranges::find(this->historyMoves_, column);
		if (it != this->historyMoves_.cend()) {
			this->historyMoves_.erase(it);
		}

		return true;
	}

    const static std::vector<std::array<std::pair<Row, Column>, 4>> WINNING_COMBOS;
};

#pragma region hardcoded winning combos
	const std::vector<std::array<std::pair<Row, Column>, 4>> Board::WINNING_COMBOS = {
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
