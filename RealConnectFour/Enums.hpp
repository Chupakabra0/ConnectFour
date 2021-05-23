#pragma once

enum class Row : short {
	FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH
};

enum class Column : short {
	FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH
};

enum class WinCode : short {
	NONE, TIE, WIN
};

enum PlayerSymbol : char {
	NONE = ' ',
	FIRST = 'X',
	SECOND = 'O'
};
