#ifndef LIGHTFIGUREPOSITION_H
#define LIGHTFIGUREPOSITION_H

#include <QString>
#include <QDebug>

typedef unsigned char FigurePosition;

// uses 8-byte figure position representation
//
// 1 2 3 4 5 6 7 8
// ^ ^ ^ ^ ^ ^ ^ ^
// | | | | | |_|_|__y
// | | |_|_|__x
// | |__unused bit
// invalid indicator bit

int GetX(FigurePosition p);
int GetY(FigurePosition p);
int GetSerialNumber(FigurePosition p);
bool IsInvalid(FigurePosition p);
bool IsValidPosition(int x, int y);

FigurePosition CreateFigurePosition(int x, int y);
FigurePosition CreateFigurePosition();
FigurePosition Shift(FigurePosition position, int dx, int dy);

FigurePosition CreateFigurePosition(std::string str);
QString ToString(FigurePosition p);

static inline QDebug operator<<(QDebug debug, FigurePosition pos)
{
    debug.nospace() << ToString(pos);

    return debug.space();
}
#endif // LIGHTFIGUREPOSITION_H
