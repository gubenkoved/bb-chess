#ifndef POSITIONHASHCALCULATOR_H
#define POSITIONHASHCALCULATOR_H

#include <QString>
#include "figure.h"

class Board;

typedef QString PositionHash;

class PositionHashCalculator
{
public:
    static PositionHash Calculate(Board* board);
    static void Update(PositionHash &hash, Board* board, FigurePosition position);
};

#endif // POSITIONHASHCALCULATOR_H
