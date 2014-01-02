#ifndef AI_H
#define AI_H

#include <QSharedPointer>

#include "rules.h"
#include "move.h"
#include "figure.h"
#include "typedefs.h"
#include "transpositiontable.h"

// This class contains algorithms to find best move
class AI
{
private:    
    static const int WIN_ESTIMATION     = +1000000;
    static const int LOOSE_ESTIMATION   = -WIN_ESTIMATION;

    Board* m_board;
    Rules* m_rules;

    TranspositionTable* m_transpositionTable;

    struct PrioritizedMove
    {
        UCHAR PriorityClass;
        SHORT PriorityValue;
        Move UnderlyingMove;
    };

    static PrioritizedMove CalculatePriority(const Move &move);
    static bool MoveByPriorityGreaterThan(const PrioritizedMove& pm1, const PrioritizedMove& pm2);

    // Stores position estimations for each figure in each possible position.
    // Usage example:
    // m_staticFigurePositionEstimation[FigureSide::White][FigureType::/1][Serial(PositionHelper::CreateFigurePosition(1, 1))]
    static int m_staticFigurePositionEstimation[SIDE_COUNT][FIGURE_TYPE_COUNT][8 * 8];
    static void InitStaticFigurePositionEstimations();

    // Returns material estimation for figure with specified type.
    static int GetFigureWeight(FigureType type);
    static int GetFigurePositionEstimation(FigureType type, POSITION position, FigureSide side);
    static int CalculateFigurePositionEstimation(FigureType type, POSITION position, FigureSide side);

    // Calcualtes estimation (material and position) for specified side.
    int GetFiguresEstimation(FigureSide side) const;

    // Returns positive value if current side has benefits compared with the opponent
    int GetRelativeEstimationFor(FigureSide side) const;

    // Returns terminal position estimation for specified side.
    // Position is terminal for specified side when there is no allowed moves by it side - draw game or lose.
    int GetTerminalPositionEstimation(FigureSide side, int depth) const;

    int Negamax(FigureSide side, int depth, int &analyzed);

    // !! Do not use INT_MIN as alpha - it provokes overflow !!
    // Call with (-INT_MAX to INT_MAX) alpha beta window to avoid int overflow issues when alpha and beta will be switched.
    // BestMove is only filled by "top level" functions (that was called with isTopLevel = true).
    int AlphaBetaNegamax(FigureSide side, int depth, int alpha, int beta, int& analyzed, Move*& bestMove, bool isTopLevel);

public:
    AI(Board* board, Rules* rules);
    ~AI();

    // Flag which indicates that cpatures should be "thinked" thought fully.
    // (i.e. search can't stop when current move is Capture).
    bool ExtendSearchDepthOnCaptures;

    // !! following is wrong, need to rewrite !! (when =1 -> captures to end considering)
    //
    // Maximal value of search depth when search can be extended.
    // Only works when ExtendSearchDepthOnCaptures = true.
    // When it equals to 0 capture chains will be considered to end,
    // When it equals to 1 capture chains will be considered to it end plus one step after chain is ended, etc...
    int MaxCurrentDepthToExtendSearchOnCaptures;

    // Indicates that AI should sort moves by priority before proccesing it.
    bool UseMovesOrdering;

    // Indicates that AI should use transposition table.
    bool UseTranspositionTable;

    Move BestMoveByAlphaBeta(FigureSide side, int depth, int& bestEstimation, int& analyzed);
    Move NegamaxSearch(FigureSide side, int depth, int& bestEstimation, int& analyzed);

    //friend class Tests;
};

#endif // AI_H
