#ifndef FEN_H
#define FEN_H

#include "board.h"

class FEN
{
public:
    static QString Evaluate(Board* board);
};

#endif // FEN_H
