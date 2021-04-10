#pragma once

#include <array>
#include <algorithm>
#include <iostream>

enum class Row : short {
    FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, COUNT
};

enum class Column : short {
    FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH, COUNT
};

enum class MoveCharacters : char {
    NONE =
    #if defined(DEBUG)
        '&',
    #else
        ' ',
    #endif
    FIRST_PLAYER = 'X',
    SECOND_PLAYER = 'O',
    COUNT
};

template<short RowsCount, short ColumnsCount>
class Board {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Board() {
        //std::fill(this->field_.begin(), this->field_.end(), MoveCharacters::NONE);
        auto sym = 'A';
        std::for_each(this->field_.begin(), this->field_.end(), [&](auto& a) { a = (char )MoveCharacters::NONE; });
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
                out << static_cast<char>(board.GetCell(Row(i), Column(j)));
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

    [[nodiscard]] constexpr auto GetSize() const {
        return this->boardSize_;
    }

    [[nodiscard]] constexpr auto GetRows() const {
        return RowsCount;
    }

    [[nodiscard]] constexpr auto GetColumns() const {
        return ColumnsCount;
    }

    //--------------------------------------------- METHOD SECTION ---------------------------------------------------//

    [[nodiscard]] auto GetCell(const Row row, const Column column) const {
        if (static_cast<short>(row) >= RowsCount) {
            throw std::out_of_range("Row index out of boundaries.");
        }
        else if (static_cast<short>(column) >= ColumnsCount) {
            throw std::out_of_range("Column index out of boundaries.");
        }

        return this->field_[static_cast<size_t>(row) * ColumnsCount + static_cast<size_t>(column)];
    }

    // TODO Player class as second param
    bool MakeMove(Column column, MoveCharacters move) {
        auto index = static_cast<short>(column);
        while (index < this->boardSize_ && this->field_[index] == (char)MoveCharacters::NONE) {
            index += static_cast<short>(ColumnsCount);
        }

        this->field_[index - ColumnsCount] = (char)move;
        return true;
    }

private:
    static constexpr auto boardSize_ = RowsCount * ColumnsCount;
    std::array<char, Board::boardSize_> field_;
};
