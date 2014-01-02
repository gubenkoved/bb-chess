#include "move.h"
#include "exception.h"

Move::Move()
:From(m_from), To(m_to), MovingFigure(m_figure), CapturedFigure(m_captured), Type(m_type)
{
    m_from = FigurePosition();
    m_to = FigurePosition();
    m_type = Invalid;
    m_figure = NULL;
    m_captured = NULL;
}

Move::Move(MoveType type, FigurePosition from, FigurePosition to, Figure* figure, Figure* captured)
    :From(m_from), To(m_to), MovingFigure(m_figure), CapturedFigure(m_captured), Type(m_type)
{
    m_from = from;
    m_to = to;
    m_type = type;
    m_figure = figure;
    m_captured = captured;    
}

Move::Move(const Move &another)
    :From(m_from), To(m_to), MovingFigure(m_figure), CapturedFigure(m_captured), Type(m_type)
{
    m_from = another.m_from;
    m_to = another.m_to;
    m_type = another.m_type;
    m_figure = another.m_figure;
    m_captured = another.m_captured;    
}

Move::~Move()
{

}

QString Move::GetTypeName() const
{
    switch (m_type)
    {
        case Normal: return "Normal";
        case Capture: return "Capture";
        case EnPassant: return "EnPassant";
        case LongCastling: return "LongCastling";
        case ShortCastling: return "ShortCastling";
        case PawnPromotion: return "PawnPromotion";
        case LongPawn: return "LongPawn";
        default: throw Exception("Invalid type");
    }
}

bool Move::IsCastling() const
{
    return m_type == Move::LongCastling || m_type == Move::ShortCastling;
}

Move &Move::operator =(const Move &another)
{
    m_from = another.m_from;
    m_to = another.m_to;
    m_type = another.m_type;
    m_figure = another.m_figure;
    m_captured = another.m_captured;

    return *this;
}
