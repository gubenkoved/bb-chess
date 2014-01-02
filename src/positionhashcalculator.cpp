#include "positionhashcalculator.h"
#include "board.h"

PositionHash PositionHashCalculator::Calculate(Board* board)
{
    PositionHash hash = PositionHash(64, '-');

    foreach (Figure* figure, board->GetAllAliveFigures())
    {
        hash.replace(GetSerialNumber(figure->Position) - 1, 1, figure->GetFENChar());
    }

    return hash;
}

void PositionHashCalculator::Update(PositionHash &hash, Board *board, FigurePosition position)
{
    QChar newChar = '-';

    Figure* figureAtPosition = board->FigureAt(position);

    if (figureAtPosition != NULL)
        newChar = figureAtPosition->GetFENChar();

    hash.replace(GetSerialNumber(position) - 1, 1, newChar);
}
