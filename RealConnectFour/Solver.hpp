#pragma once
#include "Board.hpp"
#include "Player.hpp"

struct Score {
	int amount;
	short bestMove;
};

class Solver {
public:
	Solver() = default;
	Solver(const Solver&) = default;
	Solver(Solver&&) noexcept = default;

	~Solver() noexcept = default;
	
	Solver& operator=(const Solver&) = default;
	Solver& operator=(Solver&&) noexcept = default;

	[[nodiscard]] unsigned GetNodeCount() const {
		return this->nodeCounter_;
	}
	
	[[nodiscard]] Score Solve(const Board& board, const std::pair<Player, Player>& players, int alpha, int beta) {
		this->RefreshNode();
		return this->NegaMax(board, players, alpha, beta);
		//Score min = { -static_cast<int>((board.GetSize() - board.GetNumberOfMoves()) / 2), -10 };
		//Score max = { static_cast<int>((board.GetSize() + 1 - board.GetNumberOfMoves()) / 2), -20 };

		//while (min.amount < max.amount) {                    // iteratively narrow the min-max exploration window
		//	auto med = min.amount + (max.amount - min.amount) / 2;
		//	if (med <= 0 && min.amount / 2 < med) {
		//		med = min.amount / 2;
		//	}
		//	else if (med >= 0 && max.amount / 2 > med) {
		//		med = max.amount / 2;
		//	}
		//	
		//	const auto r = this->NegaMax(board, players, med, med + 1);   // use a null depth window to know if the actual score is greater or smaller than med
		//	if (r.amount <= med) {
		//		max = r;
		//	}
		//	else {
		//		min = r;
		//	}
		//}
		//return min;
	}

private:
	unsigned nodeCounter_ = 0u;
	std::vector<short> columnsOrder {
		3, 2, 4, 1, 5, 0, 6
	};
	
	void RefreshNode() {
		this->nodeCounter_ = 0u;
	}
	
	[[nodiscard]] Score NegaMax(const Board& board, const std::pair<Player, Player>& players, int alpha, int beta) {
		++this->nodeCounter_;

		if (board.IsTie()) {
			return { 0, -1 };
		}

		auto bestScore = static_cast<int>((board.GetSize() - board.GetNumberOfMoves() - 1) / 2);

		for (const auto i : this->columnsOrder) {
			auto tempBoard = board;
			if (tempBoard.MakeMove(i, players.first.GetCharacter()) && tempBoard.GetWinner() == WinnerCode::WIN) {
				return {static_cast<int>((board.GetSize() - board.GetNumberOfMoves() + 1) / 2), i };
			}
			//for (auto j : this->columnsOrder) {
			//	if (tempBoard.MakeMove(j, players.second.GetCharacter()) && tempBoard.GetWinner() == WinnerCode::WIN) {
			//		return { static_cast<int>((board.GetSize() - board.GetNumberOfMoves() + 1) / 2), i };
			//	}
			//}
		}

		if (beta > bestScore) {
			beta = bestScore;
			if (alpha >= beta) {
				//return { beta, -2 };
			}
		}

		for (const auto i : this->columnsOrder) {
			auto tempBoard = board;
			if (tempBoard.MakeMove(i, players.first.GetCharacter())) {
				int currentScore = -this->NegaMax(tempBoard, { players.second, players.first }, -alpha, -beta).amount;

				if (currentScore > bestScore) {
					bestScore = currentScore;
				}
				if (currentScore >= beta) {
					return { currentScore, i };
				}
				if (currentScore >= alpha) {
					alpha = currentScore;
				}
			
			}
		}

		return { bestScore, -3 };
	}
};
