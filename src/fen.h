#ifndef FEN_H
#define FEN_H

#include "figure.h"
#include "board.h"

class FEN
{
public:
    static QChar GetFigureChar(const Figure& figure);
    static Figure FigureFromChar(QChar c);

    static QString Evaluate(const Board& board);

    // Restores figure's POSITION only from showrt FEN string.
    // Note: This made for puzzles and assumes that whites is turning.
    // Don't handles enpassants, passive moves counters, etc...
    // Example string: 2qN3B/2PN4/b4r2/rn1k1n2/pQ3P1p/6pK/4p3/b1RBR3
    static Board PositionFromFEN(QString fen);
};

#endif // FEN_H
