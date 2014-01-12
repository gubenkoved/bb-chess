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

    int PawnPromotionYFor(FigureSide side) const;
    int PawnHomeYFor(FigureSide side) const;
    int EnPassantPawnYFor(FigureSide side) const;
    int FirstHorizonatalYFor(FigureSide side) const;

    POSITION ForwardFor(POSITION position, FigureSide side, int dx, int dy) const;

    bool IsUnderCheckImpl(FigureSide side) const;
    bool IsUnderCheckFastImpl(FigureSide side) const;

    // removes all moves which causes check to turning player;
    // this method is only used when legal moves generation requested
    void DeleteMovesToCheck(MoveCollection& moves);

    BITBOARD _GetPossibleDestinations(Figure* figure) const;
    BITBOARD _GetPawnPossibleDestinations(Figure* figure) const;
    BITBOARD _GetKnightPossibleDestinations(Figure* figure) const;
    BITBOARD _GetBishopPossibleDestinations(Figure* figure) const;
    BITBOARD _GetRockPossibleDestinations(Figure* figure) const;
    BITBOARD _GetQueenPossibleDestinations(Figure* figure) const;
    BITBOARD _GetKingPossibleDestinations(Figure* figure) const;

    MoveCollection CreateMoves(POSITION from, POSITION to);

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
    BITBOARD GetOnLinePositions(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit) const;
    Figure* GetObstacleInDirection(POSITION position, FigureSide side, int xMult, int yMult) const;

    // Returns positions that guarded for specified figure.
    // Position is guarded when figure can kill another figure on that position.
    // Bitboard implementation.
    BITBOARD GetGuardedPositions        (Figure* figure) const;
    BITBOARD GetPawnGuardedPositions    (Figure* figure) const;
    BITBOARD GetKinghtGuardedPositions  (Figure* figure) const;
    BITBOARD GetBishopGuardedPositions  (Figure* figure) const;
    BITBOARD GetRockGuardedPositions    (Figure* figure) const;
    BITBOARD GetQueenGuardedPositions   (Figure* figure) const;
    BITBOARD GetKingGuardedPositions    (Figure* figure) const;

    BITBOARD GetGuardedPositions(FigureSide side) const;
public:
    Rules(Board* board);

    FigureSide OpponentSide(FigureSide side) const;

    MoveCollection GetPossibleMoves(FigureSide side, bool generateLegalOnly = true);

    bool IsUnderCheck(FigureSide side) const;
    bool IsPassiveEndGame() const;

    // Because make move uses pointers to figure hence
    // if you have two different instances of boards (GUI) e.g.
    // you should use FindMove first, and then call this method with finded
    // Move instance - it will always contains correct references
    void MakeMove(Move move);
    void UnMakeMove(Move move);

    // !! FOLLOWING METHODS INTENDED TO USE IN GUI ONLY
    // Following methods is incredible slow to use it
    // in normal board processing situations
    // The only reason for these methods is ability to create
    // GUI interaction more easy.

    // !! USE IN GUI ONLY !!
    // It performs search for possible moves and find the move
    // that was intended to made.
    // Triple of arguments uniquly identifies turn.
    // PromotedTo argument resolves ambiguity with pawn promotions moves...
    QList<Move> FindMoves(POSITION from, POSITION to);

    // This implementation first of all performs FindMove and then, if move has been found makes it
    // or throws an exception
    void MakeMove(POSITION from, POSITION to, FigureType promotedTo = FigureType::Queen);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible moves for all figures and then extrude it for specified figure
    MoveCollection GetPossibleMoves(Figure* figure);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible destination for all figures and then extrude it for specified figure
    PositionList GetPossibleDestinations(Figure* figure);

    friend class Tests;
};

#endif // RULES_H
