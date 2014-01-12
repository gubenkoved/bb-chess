#ifndef FIGURE_H
#define FIGURE_H

#include <QString>
#include <QChar>

#include "figureposition.h"
#include "enumshelper.h"

#define FIGURE_TYPE_COUNT 6
#define SIDE_COUNT 2

enum class FigureType
{
    Pawn    = 0,
    Knight  = 1,
    Bishop  = 2,
    Rock    = 3,
    King    = 4,
    Queen   = 5,
    Invalid = -1
};

enum class FigureSide
{
    White = 0,
    Black = 1,
    Invalid = -1
};

struct Figure
{
    FigureType Type;
    FigureSide Side;
    POSITION Position;
    int MovesCount;

    Figure(FigureSide side, FigureType type, POSITION position);
    Figure(const Figure& another);

    QChar GetUnicodeChessChar() const;
};

inline QDebug operator<<(QDebug debug, const Figure& f)
{
    debug.nospace() << "Figure("
        << EnumsHelper::ToString(f.Type) << " at "
        << PositionHelper::ToString(f.Position) << ")";
    return debug.space();
}
#endif // FIGURE_H
