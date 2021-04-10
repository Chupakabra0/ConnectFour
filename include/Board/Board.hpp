#pragma once

#include <array>
#include <algorithm>
#include <iostream>

enum class MoveCharacters : char {
    NONE =
    #if defined(DEBUG)
    '&',
    #else
    ' ',
    #endif
    FIRST_PLAYER = 'X',
    SECOND_PLAYER = 'O'
};

template<short RowsCount, short ColumnsCount>
class Board {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Board() {
        std::fill(this->field_.begin(), this->field_.end(), MoveCharacters::NONE);
    }

    Board(const Board&) = delete;
    Board(Board&&) noexcept = default;

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Board& operator=(const Board&) = delete;
    Board& operator=(Board&&) noexcept = delete;

    template<class Type>
    friend auto& operator<<(std::basic_ostream<Type>& out, const Board& board) {
        if (board.field_.empty()) {
            return out;
        }

        for (auto i = 0; i < RowsCount; ++i) {
            for (auto j = 0; j < ColumnsCount; ++j) {
                out << static_cast<char>(board.GetCell(i, j));
                if (j != ColumnsCount - 1) {
                    out << " ";
                }
            }
            if (i != RowsCount - 1) {
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

    [[nodiscard]] constexpr auto& GetSize() const {
        return this->boardSize_;
    }

    [[nodiscard]] constexpr auto& GetRows() const {
        return RowsCount;
    }

    [[nodiscard]] constexpr auto& GetColumns() const {
        return ColumnsCount;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    [[nodiscard]] auto GetCell(const size_t row, const size_t column) const {
        if (row >= RowsCount) {
            throw std::out_of_range("Row index out of boundaries.");
        }
        else if (column >= ColumnsCount) {
            throw std::out_of_range("Column index out of boundaries.");
        }

        return this->field_[row * RowsCount + column];
    }

private:
    static constexpr auto boardSize_ = RowsCount * ColumnsCount;
    std::array<MoveCharacters, Board::boardSize_> field_;
};
