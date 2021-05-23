#pragma once

#include <ranges>
#include <unordered_map>

#include "ISolver.hpp"
#include "TranspositionTable.hpp"

enum PlayerScore : short {
	PLAYER_FOUR_IN_A_ROW = 100,
	PLAYER_THREE_IN_A_ROW = 20,
	PLAYER_TWO_IN_A_ROW = 10,
	PLAYER_CENTER = 40,
	PLAYER_NEAR_CENTER = 0,
	PLAYER_NEAR_EDGE = -10,
	PLAYER_EDGE = -10
};

enum OpponentScore : short {
	OPPONENT_FOUR_IN_A_ROW = -101,
	OPPONENT_THREE_IN_A_ROW = -100,
	OPPONENT_TWO_IN_A_ROW = -10,
	OPPONENT_CENTER = 0,
	OPPONENT_NEAR_CENTER = 0,
	OPPONENT_NEAR_EDGE = 0,
	OPPONENT_EDGE = 0
};

class ClassicSolver : public ISolver {
public:
	ClassicSolver() = delete;
	ClassicSolver(const ClassicSolver&) = default;
	ClassicSolver(ClassicSolver&&) noexcept = default;

	explicit ClassicSolver(const int depth) : depth_(depth) {}

	~ClassicSolver() noexcept override = default;

	ClassicSolver& operator=(const ClassicSolver&) = default;
	ClassicSolver& operator=(ClassicSolver&&) noexcept = default;

	[[nodiscard]] int GetDepth() const {
		return this->depth_;
	}

	void SetDepth(const int depth) {
		if (this->depth_ == depth) {
			return;
		}

		this->depth_ = depth;
	}

	[[nodiscard]] short Solve(const Board& board, const std::pair<Player, Player>& players) override {
		if (board.GetNumberOfMoves() == 0) {
			return this->columnsOrder.begin()->first;
		}
		
		for (const auto& column : this->columnsOrder | std::ranges::views::keys) {
			if (board.IsWinningMove(column, players.first.GetCharacter())) {
				return column;
			}
		}

		for (const auto& column : this->columnsOrder | std::ranges::views::keys) {
			if (board.IsWinningMove(column, players.second.GetCharacter())) {
				return column;
			}
		}

		const auto move = this->PrunedMiniMax(board, players,
			this->depth_, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true).second;

		this->table_.Clear();
		
		return move;
	}

private:
	const std::unordered_map<short, std::pair<PlayerScore, OpponentScore>> columnsOrder {
		{3, { PlayerScore::PLAYER_CENTER, OpponentScore::OPPONENT_CENTER }},
		{2, { PlayerScore::PLAYER_NEAR_CENTER, OpponentScore::OPPONENT_NEAR_CENTER }},
		{4, { PlayerScore::PLAYER_NEAR_CENTER, OpponentScore::OPPONENT_NEAR_CENTER }},
		{1, { PlayerScore::PLAYER_NEAR_EDGE, OpponentScore::OPPONENT_NEAR_EDGE }},
		{5, { PlayerScore::PLAYER_NEAR_EDGE, OpponentScore::OPPONENT_NEAR_EDGE }},
		{0, { PlayerScore::PLAYER_EDGE, OpponentScore::OPPONENT_EDGE }},
		{6, { PlayerScore::PLAYER_EDGE, OpponentScore::OPPONENT_EDGE }}
	};
	
	TranspositionTable table_;
	
	int depth_;
	
	[[nodiscard]] int ScoreBoard(const Board& board, const std::pair<Player, Player>& players) const {
		auto score = 0;

		// Column position check
		for (const auto& [column, cost] : this->columnsOrder) {
			std::vector<char> cols(board.GetRowsCount());

			if (board.TryGetColumn(column, cols.begin())) {
				score += this->ScoreColumn(cols, players, cost.first, cost.second);
			}
		}

		// Horizontal check
		for (auto i = 0; i < board.GetRowsCount(); ++i) {
			std::vector<char> rows(board.GetColumnsCount());
			
			if (board.TryGetRow(i, rows.begin())) {
				for (auto j = 0; j < rows.size() - 3; ++j) {
					score += this->ScoreWindow({ rows.cbegin() + j, rows.cbegin() + j + 4 }, players);
				}
			}
		}

		//// Vertical check
		for (auto i = 0; i < board.GetColumnsCount(); ++i) {
			std::vector<char> cols(board.GetRowsCount());

			if (board.TryGetColumn(i, cols.begin())) {
				for (auto j = 0; j < cols.size() - 3; ++j) {
					score += this->ScoreWindow({ cols.cbegin() + j, cols.cbegin() + j + 4 }, players);
				}
			}
		}

		// Positive diagonal
		for (auto i = 0; i < board.GetRowsCount() - 3; ++i) {
			for (auto j = 0; j < board.GetColumnsCount() - 3; ++j) {
				std::vector<char> tempDiagonal(4u);

				for (auto k = 0u; k < tempDiagonal.size(); ++k) {
					tempDiagonal[k] = board.GetCell(i + k, j + k);
				}

				score += this->ScoreWindow(tempDiagonal, players);
			}
		}

		// Negative diagonal
		for (auto i = 0; i < board.GetRowsCount() - 3; ++i) {
			for (auto j = 0; j < board.GetColumnsCount() - 3; ++j) {
				std::vector<char> tempDiagonal(4u);
				
				for (auto k = 0; k < 4; ++k) {
					tempDiagonal[k] = board.GetCell(i - k + 3, j + k);
				}

				score += this->ScoreWindow(tempDiagonal, players);
			}
		}

		return score;
	}

	[[nodiscard]] int ScoreWindow(const std::vector<char>& window, const std::pair<Player, Player>& players) const {
		auto score = 0;

		const auto countPlayerSymbol
			= std::ranges::count(std::as_const(window), players.first.GetCharacter());
		const auto countOpponentSymbol
			= std::ranges::count(std::as_const(window), players.second.GetCharacter());
		const auto countBlankSymbol
			= std::ranges::count(std::as_const(window), ' ');

		// My influence
		if (countPlayerSymbol == 4) {
			score += PlayerScore::PLAYER_FOUR_IN_A_ROW;
		}
		else if (countPlayerSymbol == 3 && countBlankSymbol == 1) {
			score += PlayerScore::PLAYER_THREE_IN_A_ROW;
		}
		else if (countPlayerSymbol == 2 && countBlankSymbol == 2) {
			score += PlayerScore::PLAYER_TWO_IN_A_ROW;
		}

		// Opponent's influence
		if (countOpponentSymbol == 4) {
			score += OpponentScore::OPPONENT_FOUR_IN_A_ROW;
		}
		if (countOpponentSymbol == 3 && countBlankSymbol == 1) {
			score += OpponentScore::OPPONENT_THREE_IN_A_ROW;
		}
		else if (countOpponentSymbol == 2 && countBlankSymbol == 2) {
			score += OpponentScore::OPPONENT_TWO_IN_A_ROW;
		}

		return score;
	}

	[[nodiscard]] int ScoreColumn(const std::vector<char>& column, const std::pair<Player, Player>& players,
		const int playerBonus, const int opponentBonus) const {
		auto score = 0;

		const auto playerCount =
			std::ranges::count_if(std::as_const(column), [&players](const auto val) { return val == players.first.GetCharacter(); });
		const auto opponentCount =
			std::ranges::count_if(std::as_const(column), [&players](const auto val) { return val == players.second.GetCharacter(); });

		score += playerCount * playerBonus;
		score += opponentCount * opponentBonus;

		return score;
	}

	[[nodiscard]] std::pair<int, short> PrunedMiniMax(const Board& board, const std::pair<Player, Player>& players,
		int depth, const int alpha, const int beta, const bool isMax) {
		const auto winCode = board.GetWinnerCharacter();
		short bestMove = -1;

		if (winCode == players.first.GetCharacter()) {
			return { PlayerScore::PLAYER_FOUR_IN_A_ROW * (depth + 1), bestMove };
		}
		if (winCode == players.second.GetCharacter()) {
			return { OpponentScore::OPPONENT_FOUR_IN_A_ROW * (this->depth_ - depth + 1), bestMove };
		}

		std::vector<short> availableMoves(board.GetColumnsCount(), -1);
		board.TryGetAvailableMoves(availableMoves.begin());
		availableMoves.erase(std::ranges::remove(availableMoves, -1).begin(), availableMoves.cend());
		availableMoves.shrink_to_fit();

		bestMove = availableMoves.empty() ? bestMove : *(availableMoves.begin() + rand() % availableMoves.size());
		
		if (winCode == ' ') {
			return { 0, bestMove };
		}

		depth = board.GetSize() - board.GetNumberOfMoves() <= depth ? board.GetSize() - board.GetNumberOfMoves() : depth;
		
		if (depth <= 0) {
			return { this->ScoreBoard(board, players), bestMove };
		}

		Score temp{ .points = -1, .depth = depth, .bestMove = bestMove };
		if (this->table_.GetScore(board, temp) && temp.depth >= depth) {
			return { temp.points, temp.bestMove };
		}
		
		return this->PrunedMiniMaxWrapper(board, players, depth, alpha, beta, isMax);
	}

	[[nodiscard]] std::pair<int, short> MiniWrapper(const Board& board, const std::pair<Player, Player>& players,
		int depth, int alpha, int beta) {

		auto [bestScore, bestMove] = std::make_pair(std::numeric_limits<int>::max(), -1);
		
		for (const auto& column : this->columnsOrder | std::views::keys) {
			auto tempBoard = board;

			if (tempBoard.MakeMove(column, players.second.GetCharacter())) {
				const auto score = this->PrunedMiniMax(tempBoard, players, depth - 1, alpha, beta, true).first;

				if (score < bestScore) {
					bestScore = score;
					bestMove = column;
				}

				beta = std::min(beta, bestScore);

				if (alpha >= beta) {
					break;
				}
			}
		}

		return std::make_pair(bestScore, bestMove);
	}

	[[nodiscard]] std::pair<int, short> MaxWrapper(const Board& board, const std::pair<Player, Player>& players,
		int depth, int alpha, int beta) {

		auto [bestScore, bestMove] = std::make_pair(std::numeric_limits<int>::min(), -1);
		
		for (const auto& column : this->columnsOrder | std::views::keys) {
			auto tempBoard = board;

			if (tempBoard.MakeMove(column, players.first.GetCharacter())) {
				const auto score = this->PrunedMiniMax(tempBoard, players, depth - 1, alpha, beta, false).first;

				if (score > bestScore) {
					bestScore = score;
					bestMove = column;
				}

				alpha = std::max(alpha, bestScore);

				if (alpha >= beta) {
					break;
				}
			}
		}

		return std::make_pair(bestScore, bestMove);
	}

	[[nodiscard]] std::pair<int, short> PrunedMiniMaxWrapper(const Board& board, const std::pair<Player, Player>& players,
		int depth, int alpha, int beta, const bool isMax) {

		const auto [bestScore, bestMove] = isMax
			? this->MaxWrapper(board, players, depth, alpha, beta)
			: this->MiniWrapper(board, players, depth, alpha, beta);

		this->table_.Insert(board, { bestScore, depth, bestMove });
		
		return std::make_pair(bestScore, bestMove);
	}

	[[nodiscard]] short PickBestMove(const Board& board, const std::pair<Player, Player>& players) const {
		auto maxScore = std::numeric_limits<int>::min();
		auto bestMove = static_cast<short>(rand() % board.GetColumnsCount());

		for (const auto column : this->columnsOrder | std::views::keys) {
			auto tempBoard = board;
			if (tempBoard.MakeMove(column, players.first.GetCharacter())) {
				const auto score = this->ScoreBoard(tempBoard, players);
				if (score > maxScore) {
					maxScore = score;
					bestMove = column;
				}
			}
		}

		return bestMove;
	}
};
