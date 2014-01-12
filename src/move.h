#ifndef MOVE_H
#define MOVE_H

#include "figure.h"
#include "figureposition.h"
#include "exception.h"
#include "enumshelper.h"
#include "typedefs.h"

enum class MoveType
{
    Normal, // non-special ruled move with capture or not
    //Capture, // capture enemy figure
    LongPawn, // first pawn long step
    LongCastling, // long castling
    ShortCastling, // short castling
    EnPassant, // one pawn takes another, when it steps through trapped cell
    PawnPromotion, // when pawn reaches enemy home horizon
    Invalid // when default constructor used
};

struct Move
{
    POSITION From;
    POSITION To;
    MoveType Type;
    Figure* MovingFigure;
    Figure* CapturedFigure; // for Capture and EnPassant not NULL, and NULL otherwise
    FigureType PromotedTo; // for PawnPromotion only

    Move();
    Move(MoveType type, POSITION from, POSITION to, Figure* figure, Figure* captured);    
    Move(const Move& another);

    bool IsCastling() const;

    Move& operator=(const Move& another);
};

inline QDebug operator<<(QDebug debug, const Move& m)
{
    debug << "Move( by"
        << EnumsHelper::ToString(m.MovingFigure->Type)
        << EnumsHelper::ToString(m.Type)
        << (m.Type == MoveType::PawnPromotion ? EnumsHelper::ToString(m.PromotedTo) : "")
        << "from"
        << PositionHelper::ToString(m.From) << "to"
        << PositionHelper::ToString(m.To)
        << (m.CapturedFigure != NULL ? EnumsHelper::ToString(m.CapturedFigure->Type) + " captured" : "")
        << ")";

    return debug;
}
#endif // MOVE_H
