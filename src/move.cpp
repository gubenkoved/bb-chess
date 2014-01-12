#include "move.h"
#include "exception.h"

Move::Move()
{
    From = POSITION();
    To = POSITION();
    Type = MoveType::Invalid;
    MovingFigure = NULL;
    CapturedFigure = NULL;
    PromotedTo = FigureType::Invalid;
}

Move::Move(MoveType type, POSITION from, POSITION to, Figure* figure, Figure* captured)
{
    From = from;
    To = to;
    Type = type;
    MovingFigure = figure;
    CapturedFigure = captured;
    PromotedTo = FigureType::Invalid;
}

Move::Move(const Move &another)
{
    From = another.From;
    To = another.To;
    Type = another.Type;
    MovingFigure = another.MovingFigure;
    CapturedFigure = another.CapturedFigure;
    PromotedTo = another.PromotedTo;
}

bool Move::IsCastling() const
{
    return Type == MoveType::LongCastling || Type == MoveType::ShortCastling;
}

Move& Move::operator =(const Move& another)
{
    From = another.From;
    To = another.To;
    Type = another.Type;
    MovingFigure = another.MovingFigure;
    CapturedFigure = another.CapturedFigure;
    PromotedTo = another.PromotedTo;

    return *this;
}
