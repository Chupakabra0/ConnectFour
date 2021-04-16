#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "MoveCharacters/MoveCharacters.hpp"

enum class Row : short {
    FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, COUNT
};

enum class Column : short {
    FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH, COUNT
};

class Board {
public:
    //------------------------------------------------- CTOR SECTION -------------------------------------------------//

    Board() {
        this->field_.assign(this->rowsCount_ * this->columnsCount_, MoveCharacters::NONE);
    }

    Board(const Board&) = delete;
    Board(Board&&) noexcept = default;

    //--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

    Board& operator=(const Board&) = delete;
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
            out << "| " << static_cast<char>(board.GetCell(Row(i), Column::FIRST)) << " | ";
            for (auto j = 1; j < board.columnsCount_; ++j) {
                out << static_cast<char>(board.GetCell(Row(i), Column(j))) << " | ";
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

    [[nodiscard]] MoveCharacters GetCell(const Row row, const Column column) const {
        if (static_cast<short>(row) >= this->rowsCount_) {
            throw std::out_of_range("Row index out of boundaries.");
        }
        else if (static_cast<short>(column) >= this->columnsCount_) {
            throw std::out_of_range("Column index out of boundaries.");
        }

        return this->field_[static_cast<size_t>(row) * this->columnsCount_ + static_cast<size_t>(column)];
    }

    // TODO Player class as second param
    bool MakeMove(Column column, MoveCharacters move) {
        auto index = static_cast<int>(column);
        while (index < this->GetSize() && this->field_[index] == MoveCharacters::NONE) {
            index += this->columnsCount_;
        }

        index -= this->columnsCount_;

        if (index < 0) {
            return false;
        }

        this->field_[index] = move;
        return true;
    }

private:
    short rowsCount_ = 6, columnsCount_ = 7;
    std::vector<MoveCharacters> field_;
};
