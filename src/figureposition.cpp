#include "figureposition.h"

POSITION PositionHelper::Create(int x, int y)
{
    if (IsValidPosition(x, y))
    {
        return (y - 1) | ((x - 1) << 3);
    } else
    {
        return POS_INVALID;
    }
}

POSITION PositionHelper::Shift(POSITION position, int dx, int dy)
{
    return PositionHelper::Create(PositionHelper::X(position) + dx, PositionHelper::Y(position) + dy);
}

POSITION PositionHelper::FromString(std::string str)
{
    char xChar = str[0];
    char yChar = str[1];

    int x = (xChar - 'a') + 1;
    int y = (yChar - '1') + 1;

    return PositionHelper::Create(x, y);
}

QString PositionHelper::ToString(POSITION p)
{
    QChar c1 = 'a' + PositionHelper::X(p) - 1; //QChar::fromAscii('a' + PositionHelper::X(p) - 1);
    QChar c2 = '1' + PositionHelper::Y(p) - 1; //QChar::fromAscii('1' + PositionHelper::Y(p) - 1);

    QString s = QString(c1) + QString(c2);

    return s;
}
