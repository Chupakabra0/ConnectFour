#pragma once

enum class MoveCharacters : char {
    NONE =
    #if defined(DEBUG)
    '&',
    #else
    ' ',
    #endif
    FIRST_PLAYER = 'X',
    SECOND_PLAYER = 'O',
};
