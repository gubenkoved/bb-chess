#include "figure.h"
#include "exception.h"

Figure::Figure(FigureSide side, FigureType type, POSITION position)
{
    Side = side;
    Type = type;
    MovesCount = 0;
    Position = position;
}

Figure::Figure(FigureSide side, FigureType type, std::string position)
{
    Position = PositionHelper::FromString(position);
    Side = side;
    Type = type;
    MovesCount = 0;
}

Figure::Figure(const Figure &another)    
{
    Position = another.Position;
    Side = another.Side;
    Type = another.Type;
    MovesCount = another.MovesCount;
}

QChar Figure::GetUnicodeChessChar() const
{
    switch (Type)
    {
        case Pawn:
            return Side == White ? L'\u2659' : L'\u265F';
        case Bishop:
            return Side == White ? L'\u2657' : L'\u265D';
        case Knight:
            return Side == White ? L'\u2658' : L'\u265E';
        case Rock:
            return Side == White ? L'\u2656' : L'\u265C';
        case Queen:
            return Side == White ? L'\u2655' : L'\u265B';
        case King:
            return Side == White ? L'\u2654' : L'\u265A';
        default:
            throw Exception("Invalid figure type");
    }
}

QString Figure::GetName() const
{
    switch (Type)
    {
        case Pawn:
            return "Pawn";
        case Bishop:
            return "Bishop";
        case Knight:
            return "Knight";
        case Rock:
            return "Rock";
        case Queen:
            return "Queen";
        case King:
            return "King";
        default:
            throw Exception("Invalid figure type");
    }
}
