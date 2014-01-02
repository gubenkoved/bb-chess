#ifndef RULES_H
#define RULES_H

#include <QList>

#include "typedefs.h"
#include "board.h"
#include "move.h"
#include "figureposition.h"
#include "figure.h"

class Rules
{
    Board* m_board;    

    POSITION ForwardFor(POSITION position, FigureSide side, int dx, int dy) const;

    int PawnPromotionYFor(FigureSide side) const;
    int EnPassantPawnYFor(FigureSide side) const;

    bool IsUnderCheckImpl(FigureSide side) const;
    bool IsUnderCheckFastImpl(FigureSide side) const;    

    // DeleteMovesToCheck
    // -------------------
    // Function erases all moves which causes check to turning player
    void DeleteMovesToCheck(MoveCollection& moves);
    // NOTE: ...PossibleDestinations function family returns NOT FULLY VALID destination yet!
    // because it SHOULD BE filtered by DeleteMovesToCheck and DeleteSelfCaptureDesination functions
    // to avoid moves, that causes check to turning side and self-capture turns
    // ONLY FOR PRIVATE USAGE!
    void DeleteSelfCaptureDesination(PositionCollection& destinations, FigureSide selfSide) const;
    BITBOARD _GetPossibleDestinations(Figure* figure) const;
    BITBOARD _GetPawnPossibleDestinations(Figure* figure) const;
    BITBOARD _GetKnightPossibleDestinations(Figure* figure) const;
    BITBOARD _GetBishopPossibleDestinations(Figure* figure) const;
    BITBOARD _GetRockPossibleDestinations(Figure* figure) const;
    BITBOARD _GetQueenPossibleDestinations(Figure* figure) const;
    BITBOARD _GetKingPossibleDestinations2(Figure* figure) const;

    Move CreateMove(POSITION from, POSITION to);

    // GetOnLinePositions function
    // ---------------------------
    // This function was written because many of figures can be described in general manner.
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
    BITBOARD GetOnLinePositions2(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit) const;
    Figure* GetObstacleInDirection(POSITION position, FigureSide side, int xMult, int yMult) const;

    // Returns positions that guarded for specified figure.
    // Position is guarded when figure can kill another figure on that position.
    // Bitboard implementation.
    BITBOARD GetGuardedPositions2        (Figure* figure) const;
    BITBOARD GetPawnGuardedPositions2    (Figure* figure) const;
    BITBOARD GetKinghtGuardedPositions2  (Figure* figure) const;
    BITBOARD GetBishopGuardedPositions2  (Figure* figure) const;
    BITBOARD GetRockGuardedPositions2    (Figure* figure) const;
    BITBOARD GetQueenGuardedPositions2   (Figure* figure) const;
    BITBOARD GetKingGuardedPositions2    (Figure* figure) const;

    BITBOARD GetGuardedPositions2(FigureSide side) const;
public:
    Rules(Board* board);

    FigureSide OpponentSide(FigureSide side) const;
    int FirstHorizonatalYFor(FigureSide side) const;

    MoveCollection GetPossibleMoves(FigureSide side);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible moves for all figures and then extrude it for specified figure
    MoveCollection GetPossibleMoves(Figure* figure);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible destination for all figures and then extrude it for specified figure
    PositionList GetPossibleDestinations(Figure* figure);

    bool IsUnderCheck(FigureSide side) const;
    bool IsPassiveEndGame() const;

    void MakeMove(Move move);
    void MakeMove(POSITION from, POSITION to);
    void UnMakeMove(Move move);

    friend class Tests;
};

#endif // RULES_H
