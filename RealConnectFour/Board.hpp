#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "Enums.hpp"

class Board {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    explicit Board() {
        this->field_.assign(this->rowsCount_ * this->columnsCount_, ' ');
    }

    Board(const Board&) = default;
    Board(Board&&) noexcept = default;

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Board& operator=(const Board&) = default;
    Board& operator=(Board&&) noexcept = default;

    template<class Type>
    friend auto& operator<<(std::basic_ostream<Type>& out, const Board& board) {
        if (board.field_.empty()) {
            return out;
        }

        // TODO some util-func
        std::string delimiter;
        for (auto i = 0u; i < 2 * board.columnsCount_ + board.columnsCount_ / 7; ++i) {
            delimiter += "--";
        }
        //

        for (auto i = 0; i < board.rowsCount_; ++i) {
            out << "| " << board.GetCell(i, 0) << " | ";
            for (auto j = 1; j < board.columnsCount_; ++j) {
                out << board.GetCell(i, j) << " | ";
            }

            out << std::endl << delimiter;
            if (i != board.rowsCount_ - 1) {
                out << std::endl;
            }
        }

        return out;
    }

    //----------------------------------------------- DTOR SECTION ---------------------------------------------------//

    ~Board() noexcept = default;

    //-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

    [[nodiscard]] const auto& GetField() const {
        return this->field_;
    }

    [[nodiscard]] auto GetSize() const {
        return this->field_.size();
    }

    [[nodiscard]] auto GetRows() const {
        return this->rowsCount_;
    }

    [[nodiscard]] auto GetColumns() const {
        return this->columnsCount_;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    [[nodiscard]] char GetCell(const short row, const short column) const {
        if (static_cast<short>(row) >= this->rowsCount_) {
            throw std::out_of_range("Row index out of boundaries.");
        }
        if (static_cast<short>(column) >= this->columnsCount_) {
            throw std::out_of_range("Column index out of boundaries.");
        }

        return this->field_[static_cast<size_t>(row) * this->columnsCount_ + static_cast<size_t>(column)];
    }


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

        for (const auto& combo : WINNING_COMBOS) {
            const auto temp = std::array{
                this->GetCell(static_cast<short>(combo[0].first), static_cast<short>(combo[0].second)) != ' ',
                this->GetCell(static_cast<short>(combo[0].first), static_cast<short>(combo[0].second)) == this->GetCell(static_cast<short>(combo[1].first), static_cast<short>(combo[1].second)),
                this->GetCell(static_cast<short>(combo[1].first), static_cast<short>(combo[1].second)) == this->GetCell(static_cast<short>(combo[2].first), static_cast<short>(combo[2].second)),
                this->GetCell(static_cast<short>(combo[2].first), static_cast<short>(combo[2].second)) == this->GetCell(static_cast<short>(combo[3].first), static_cast<short>(combo[3].second))
            };

            if (std::accumulate(temp.begin(), temp.end(), true, std::bit_and<>())) {
                return WinnerCode::WIN;
            }
        }

        if (this->IsTie()) {
            return WinnerCode::TIE;
        }

    	return WinnerCode::NONE;
    }

    // TODO Player class as second param
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
        return true;
    }

    [[nodiscard]] bool IsTie() const {
        return this->GetNumberOfMoves() == this->field_.size();
    }

	[[nodiscard]] unsigned GetNumberOfMoves() const {
        return std::ranges::count_if(this->field_, [](auto el) { return el != ' '; });
    }

private:
    short rowsCount_ = 6, columnsCount_ = 7;
    std::vector<char> field_;
};
