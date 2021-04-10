#pragma once

#include <array>
#include <algorithm>

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
    Board() {
        std::fill(this->field_.begin(), this->field_.end(), MoveCharacters::NONE);
    }

    Board(const Board&)     = delete;
    Board(Board&&) noexcept = default;

    Board& operator=(const Board&)     = delete;
    Board& operator=(Board&&) noexcept = delete;

    ~Board() noexcept = default;

     const auto& GetField() {
         return this->field_;
     }

private:
    static constexpr auto boardSize_ = RowsCount * ColumnsCount;
    std::array<MoveCharacters, Board::boardSize_> field_;
};
