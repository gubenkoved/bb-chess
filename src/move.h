#ifndef MOVE_H
#define MOVE_H

#include "typedefs.h"
#include "figure.h"
#include "figureposition.h"

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

struct Move
{
#ifdef QT_DEBUG
    //QString m_stringRep;
#endif

    POSITION From;
    POSITION To;
    MoveType Type;
    Figure* MovingFigure;
    Figure* CapturedFigure; // actual only when is capture move, otherwise NULL

    Move();
    Move(MoveType type, POSITION from, POSITION to, Figure* figure, Figure* captured);
    Move(const Move& another);

    QString GetTypeName() const;
    bool IsCastling() const;

    Move& operator=(const Move& another);
};

inline QDebug operator<<(QDebug debug, const Move& m)
{
    debug << "Move("
        << m.MovingFigure->GetName().toStdString().c_str()
        << m.GetTypeName() << "turn from"
        << PositionHelper::ToString(m.From) << "to"
        << PositionHelper::ToString(m.To) << ")";

    return debug;
}
#endif // MOVE_H
