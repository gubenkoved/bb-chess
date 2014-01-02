#include "move.h"
#include "exception.h"

Move::Move()
{
    From = POSITION();
    To = POSITION();
    Type = Invalid;
    MovingFigure = NULL;
    CapturedFigure = NULL;

#ifdef QT_DEBUG
    //m_stringRep = ToString(m_from) + " to " + ToString(m_to);
#endif
}

Move::Move(MoveType type, POSITION from, POSITION to, Figure* figure, Figure* captured)
{
    From = from;
    To = to;
    Type = type;
    MovingFigure = figure;
    CapturedFigure = captured;

#ifdef QT_DEBUG
    //m_stringRep = ToString(m_from) //+ " to " + ToString(m_to);
#endif
}

Move::Move(const Move &another)
{
    From = another.From;
    To = another.To;
    Type = another.Type;
    MovingFigure = another.MovingFigure;
    CapturedFigure = another.CapturedFigure;

#ifdef QT_DEBUG
    //m_stringRep = ToString(m_from) + " to " + ToString(m_to);
#endif
}

QString Move::GetTypeName() const
{
    switch (Type)
    {
        case Normal:        return "Normal";
        case Capture:       return "Capture";
        case EnPassant:     return "EnPassant";
        case LongCastling:  return "LongCastling";
        case ShortCastling: return "ShortCastling";
        case PawnPromotion: return "PawnPromotion";
        case LongPawn:      return "LongPawn";

        default: throw Exception("Invalid type");
    }
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

#ifdef QT_DEBUG
    //m_stringRep = another.m_stringRep;
#endif

    return *this;
}
