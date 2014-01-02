#ifndef MOVE_H
#define MOVE_H

#include "figure.h"
#include "lightfigureposition.h"

class Move
{
public:
    enum MoveType
    {
        Normal, // just move from one to another position
        Capture, // capture enemy figure
        LongPawn, // first pawn long step
        LongCastling, // long castling
        ShortCastling, // short castling
        EnPassant, // one pawn takes another, when it steps through trapped cell
        PawnPromotion, // when pawn reaches enemy home horizon
        Invalid // when default constructor used
    };

private:
    Figure* m_figure;
    MoveType m_type;
    FigurePosition m_from;
    FigurePosition m_to;
    Figure* m_captured; // actual only when is capture move, otherwise NULL    

public:
    Move();
    Move(MoveType type, FigurePosition from, FigurePosition to, Figure* figure, Figure* captured);
    Move(const Move& another);
    ~Move();

    const FigurePosition& From;
    const FigurePosition& To;
    Figure* const& MovingFigure;
    Figure* const& CapturedFigure;
    const MoveType& Type;

    QString GetTypeName() const;
    bool IsCastling() const;

    Move& operator=(const Move& another);
};

inline QDebug operator<<(QDebug debug, const Move& m)
{
    debug << "Move("
        << m.MovingFigure->GetName().toStdString().c_str()
        << m.GetTypeName() << "turn from"
        << ToString(m.From) << "to"
        << ToString(m.To) << ")";

    return debug;
}
#endif // MOVE_H
