#ifndef FIGURE_H
#define FIGURE_H

#include <QString>
#include <QChar>
#include <string.h>

#include "figureposition.h"

#define FIGURE_TYPE_COUNT 6
#define SIDE_COUNT 2

enum FigureType
{
    Pawn    = 0,
    Knight  = 1,
    Bishop  = 2,
    Rock    = 3,
    King    = 4,
    Queen   = 5
};

enum FigureSide
{
    White = 0,
    Black = 1
};

struct Figure
{
    FigureType Type;
    FigureSide Side;
    POSITION Position;
    int MovesCount;

    Figure(FigureSide side, FigureType type, POSITION position);
    Figure(FigureSide side, FigureType type, std::string position);
    Figure(const Figure& another);

    QChar GetUnicodeChessChar() const;
    QString GetName() const;
};

inline QDebug operator<<(QDebug debug, const Figure& f)
{
    debug.nospace() << "Figure("
        << f.GetName() << " at "
        << f.Position << ")";
    return debug.space();
}
#endif // FIGURE_H
