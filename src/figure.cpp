#include "figure.h"
#include "exception.h"

Figure::Figure(FigureSide side, FigureType type, POSITION position)
{
    Side = side;
    Type = type;
    MovesCount = 0;
    Position = position;
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
        case FigureType::Pawn:
            return Side == FigureSide::White ? L'\u2659' : L'\u265F';
        case FigureType::Bishop:
            return Side == FigureSide::White ? L'\u2657' : L'\u265D';
        case FigureType::Knight:
            return Side == FigureSide::White ? L'\u2658' : L'\u265E';
        case FigureType::Rock:
            return Side == FigureSide::White ? L'\u2656' : L'\u265C';
        case FigureType::Queen:
            return Side == FigureSide::White ? L'\u2655' : L'\u265B';
        case FigureType::King:
            return Side == FigureSide::White ? L'\u2654' : L'\u265A';
        default:
            throw Exception("Invalid figure type");
    }
}
