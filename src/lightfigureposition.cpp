#include "lightfigureposition.h"

int GetX(FigurePosition p)
{
    return ((p & 0x38) >> 3) + 1;
}

int GetY(FigurePosition p)
{
    return (p & 0x07) + 1;
}

int GetSerialNumber(FigurePosition p)
{
    return (GetY(p) - 1) * 8 + GetX(p);
}

bool IsInvalid(FigurePosition p)
{
    return p & 0x80;
}

bool IsValidPosition(int x, int y)
{
    return x >= 1 && x <= 8 && y >= 1 && y <= 8;
}

FigurePosition CreateFigurePosition(int x, int y)
{
    if (IsValidPosition(x, y))
    {
        return (y - 1) | ((x - 1) << 3);
    } else
    {
        return 0x80;
    }
}

FigurePosition CreateFigurePosition()
{
    return 0x80;
}

FigurePosition Shift(FigurePosition position, int dx, int dy)
{
    return CreateFigurePosition(GetX(position) + dx, GetY(position) + dy);
}

FigurePosition CreateFigurePosition(std::string str)
{
    char xChar = str[0];
    char yChar = str[1];

    int x = ((int)xChar - (int)'a') + 1;
    int y = ((int)yChar - (int)'1') + 1;

    return CreateFigurePosition(x, y);
}

QString ToString(FigurePosition p)
{
    QChar c1 = QChar::fromAscii('a' + GetX(p) - 1);
    QChar c2 = QChar::fromAscii('1' + GetY(p) - 1);

    QString s = QString(c1) + QString(c2);

    return s;
}
