#ifndef RULES_H
#define RULES_H

#include <QList>

#include "board.h"
#include "move.h"
#include "lightfigureposition.h"
#include "figure.h"

typedef QList<FigurePosition> PositionList;
typedef QList<Move> MoveList;

class Rules
{
    Board* m_board;

    template<class T> void AppendIfNotNull(QList<T>* list, T value) const;
    template<class T> void AppendIfNotExists(QList<T>* list, T value) const;
    template<class T> void AppendIfNotExists(QList<T>* list, QList<T> values) const;
    void AppendIfValid(PositionList* list, FigurePosition position) const;
    void AppendIfValid(PositionList* list, PositionList positions) const;
    void AppendIfValidAndNotExists(PositionList* list, FigurePosition position) const;
    void AppendIfValidAndNotExists(PositionList* list, PositionList positions) const;

    FigurePosition ForwardFor(FigurePosition position, Figure::FigureSide side, int dx, int dy) const;

    int PawnPromotionYFor(Figure::FigureSide side) const;
    int EnPassantPawnYFor(Figure::FigureSide side) const;

    bool IsUnderCheckImpl(Figure::FigureSide side) const;
    Figure* GetObstacleInDirection(FigurePosition position, Figure::FigureSide side, int xMult, int yMult) const;
    bool IsUnderCheckFastImpl(Figure::FigureSide side) const;

    // DeleteMovesToCheck
    // -------------------
    // Function erases all moves which causes check to turning player
    MoveList* DeleteMovesToCheck(MoveList* moves);
    // NOTE: ...PossibleDestinations function family returns NOT FULLY VALID destination yet!
    // because it SHOULD BE filtered by DeleteMovesToCheck and DeleteSelfCaptureDesination functions
    // to avoid moves, that causes check to turning side and self-capture turns
    // ONLY FOR PRIVATE USAGE!
    void DeleteSelfCaptureDesination(PositionList* destinations, Figure::FigureSide selfSide) const;
    PositionList _GetPossibleDestinations(Figure* figure) const;    
    PositionList _GetPawnPossibleDestinations(Figure* figure) const;
    PositionList _GetKnightPossibleDestinations(Figure* figure) const;
    PositionList _GetBishopPossibleDestinations(Figure* figure) const;
    PositionList _GetRockPossibleDestinations(Figure* figure) const;
    PositionList _GetQueenPossibleDestinations(Figure* figure) const;
    PositionList _GetKingPossibleDestinations(Figure* figure) const;

    Move CreateMove(FigurePosition from, FigurePosition to);

    // GetOnLinePositions function
    // ---------------------------
    // This is some code optimization written because many of figures can be described in general manner.
    // e.g.
    //  - bishop can guard in 4 diagonal lines (forward-right, forward-left, backward-right, backward-left) to
    //     the end of board, and guard line interrupted by obstacle (figure)
    //  - rock have simular situation, but have anothe line definition
    //  - queen is actually bishop + rock
    //  - pawn, yes even pawn have obly 2 guard lines (forward-left and forward-right) but it len limited by one
    //
    // RETURNS: Function returns positions on some line before obstacle (figure) but inclusive it
    //
    // NOTE: valid xMult and yMult values in set {-1, 0, +1}!!!
    //
    // yMult = 1 means what line has forward direction, -1 - backward
    // xMult = 1 means what line has direction which x are increasing
    //
    // EXAMPLE: function runned with xMult = 0, yMult = 1 and lenLimit = 2 (...,0,1,2) returns
    // pawn first move possibilities;
    PositionList GetOnLinePositions(FigurePosition position, Figure::FigureSide side, int xMult, int yMult, int lenLimit) const;
    PositionList GetGuardedPositions(Figure* figure) const;
    PositionList GetPawnGuardedPositions(Figure* figure) const;
    PositionList GetKinghtGuardedPositions(Figure* figure) const;
    PositionList GetBishopGuardedPositions(Figure* figure) const;
    PositionList GetRockGuardedPositions(Figure* figure) const;
    PositionList GetQueenGuardedPositions(Figure* figure) const;
    PositionList GetKingGuardedPositions(Figure* figure) const;    
public:
    Rules(Board* board);

    Figure::FigureSide OpponentSide(Figure::FigureSide side) const;
    int FirstHorizonatalYFor(Figure::FigureSide side) const;

    PositionList GetGuardedPositions(Figure::FigureSide side) const;
    MoveList GetPossibleMoves(Figure::FigureSide side);
    MoveList GetPossibleMoves(Figure* figure);
    PositionList GetPossibleDestinations(Figure* figure);

    bool IsUnderCheck(Figure::FigureSide side) const;
    bool IsPassiveEndGame() const;

    void MakeMove(Move move);
    void MakeMove(FigurePosition from, FigurePosition to);
    void UnMakeMove(Move move);
};

#endif // RULES_H
