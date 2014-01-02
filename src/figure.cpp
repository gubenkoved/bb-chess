#include "figure.h"
#include "exception.h"

Figure::Figure(FigureSide side, FigureType type, FigurePosition position)
    :Position(m_position), Side(m_side), MovesCount(m_movesCounter), Type(m_type)
{
    m_side = side;
    m_type = type;
    m_movesCounter = 0;
    m_position = position;
}

Figure::Figure(FigureSide side, FigureType type, const char position[])
    :Position(m_position), Side(m_side), MovesCount(m_movesCounter), Type(m_type)
{
    m_position = CreateFigurePosition(position);
    m_side = side;
    m_type = type;
    m_movesCounter = 0;
}

Figure::Figure(const Figure &another)
    :Position(m_position), Side(m_side), MovesCount(m_movesCounter), Type(m_type)
{
    m_position = another.m_position;
    m_side = another.m_side;
    m_type = another.m_type;
    m_movesCounter = another.m_movesCounter;
}

void Figure::SetFigureType(Figure::FigureType type)
{
    m_type = type;
}

void Figure::SetPosition(FigurePosition position)
{
    m_position = position;
}

void Figure::IncreaseMovesCounter()
{
    ++m_movesCounter;
}

void Figure::DecreaseMovesCounter()
{
    if (m_movesCounter == 0)
        throw Exception("Moves counter can not be negative");

    --m_movesCounter;
}

bool Figure::IsWhite() const
{
    return m_side == Figure::White;
}

bool Figure::IsBlack() const
{
    return m_side == Figure::Black;
}

QChar Figure::GetUnicodeChessChar() const
{
    switch (m_type)
    {
        case Pawn:
            return IsWhite() ? L'\u2659' : L'\u265F';
        case Bishop:
            return IsWhite() ? L'\u2657' : L'\u265D';
        case Knight:
            return IsWhite() ? L'\u2658' : L'\u265E';
        case Rock:
            return IsWhite() ? L'\u2656' : L'\u265C';
        case Queen:
            return IsWhite() ? L'\u2655' : L'\u265B';
        case King:
            return IsWhite() ? L'\u2654' : L'\u265A';
        default:
            throw Exception("Invalid figure type");
    }
}

QChar Figure::GetFENChar() const
{
    switch (m_type)
    {
        case Pawn:
            return IsWhite() ? 'P' : 'p';
        case Bishop:
            return IsWhite() ? 'B' : 'b';
        case Knight:
            return IsWhite() ? 'N' : 'n';
        case Rock:
            return IsWhite() ? 'R' : 'r';
        case Queen:
            return IsWhite() ? 'Q' : 'q';
        case King:
            return IsWhite() ? 'K' : 'k';
        default:
            throw Exception("Invalid figure type");
    }
}

QString Figure::GetName() const
{
    switch (m_type)
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
