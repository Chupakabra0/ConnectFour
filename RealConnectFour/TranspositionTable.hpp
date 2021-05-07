#pragma once
#include <map>

#include "Board.hpp"

struct Score {
	int points;
	int depth;
	short bestMove;
};

class TranspositionTable {
	using ScoreType = Score;
	using Key = uint64_t;
public:
	//------------------------------------------------- CTOR SECTION -------------------------------------------------//

	explicit TranspositionTable() = default;
	TranspositionTable(const TranspositionTable&) = default;
	TranspositionTable(TranspositionTable&&) noexcept = default;

	//----------------------------------------------- DTOR SECTION ---------------------------------------------------//

	~TranspositionTable() noexcept = default;
	
	//--------------------------------------------- OPERATOR SECTION -------------------------------------------------//

	TranspositionTable& operator=(const TranspositionTable&) = default;
	TranspositionTable& operator=(TranspositionTable&&) noexcept = default;
	
	//-------------------------------------------- ACCESSOR SECTION --------------------------------------------------//

	bool GetScore(const Board& key, ScoreType& valueToSave) const {
		const auto find = this->table_.find(key.ToKey());

		if (find == this->table_.cend()) {
			return false;
		}

		valueToSave = find->second;
		return true;
	}
	
	//--------------------------------------------- METHOD SECTION ---------------------------------------------------//

	void Clear() {
		this->table_.clear();
	}
	
	void Insert(const Board& key, const ScoreType& value) {
		const auto keyValue = key.ToKey();
		const auto find     = this->table_.find(keyValue);
		if (find != this->table_.cend() && value.depth > find->second.depth) {
			this->table_.erase(find);
		}
		
		return this->Insert({ keyValue, value });
	}

private:
	std::map<Key, ScoreType> table_;

	void Insert(const std::pair<Key, ScoreType>& pair) {
		this->table_.insert(pair);
	}
};
