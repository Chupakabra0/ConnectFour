#pragma once

#include "Board.hpp"
#include "Player.hpp"

struct ISolver {
public:
	ISolver() = default;
	ISolver(const ISolver&) = default;
	ISolver(ISolver&&) noexcept = default;

	ISolver& operator=(const ISolver&) = default;
	ISolver& operator=(ISolver&&) noexcept = default;

	virtual ~ISolver() noexcept = default;

	[[nodiscard]] virtual short Solve(const Board&, const std::pair<Player, Player>& players) = 0;

protected:

};