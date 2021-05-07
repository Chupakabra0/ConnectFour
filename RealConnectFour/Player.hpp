#pragma once

#include "Board.hpp"

class Player {
public:

	Player() = delete;
	Player(const Player&) = default;
	Player(Player&&) noexcept = default;
	explicit Player(const char character)
		: character_(character) {}


	Player& operator=(const Player&) = default;
	Player& operator=(Player&&) noexcept = default;
	
	virtual ~Player() noexcept = default;
	
	[[nodiscard]] char GetCharacter() const {
		return this->character_;
	}

	bool MakeMove(Board* board, short move) const {
		return board->MakeMove(move, this->character_);
	}

protected:
	char character_;
};
