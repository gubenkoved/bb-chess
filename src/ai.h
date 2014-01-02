#ifndef AI_H
#define AI_H

#include <QSharedPointer>

#include "rules.h"
#include "move.h"
#include "figure.h"
#include "transpositiontable.h"

// This class contains algorithms to find best move
class AI
{
private:
    static const int LOOSE_ESTIMATION = -1000000;
    static const int WIN_ESTIMATION = +1000000;

    Board* m_board;
    Rules* m_rules;
    TranspositionTable* m_transpositionTable;


    static int GetFigureWeight(Figure::FigureType type);
    static int GetFigurePositionEstimation(Figure::FigureType type, FigurePosition position, Figure::FigureSide side);

    int GetFiguresEstimation(Figure::FigureSide side) const;
    // Returns posibive value if current side has benefits compared with the opponent
    int GetRelativeEstimationFor(Figure::FigureSide side) const;
    int GetTerminalPositionEstimation(Figure::FigureSide side, int depth) const;    

    int Negamax(Figure::FigureSide side, int depth, int &analyzed);

    static int MovePriority(Move::MoveType type);
    static bool MoveComparator(const Move& m1, const Move& m2);
    // NOTE:
    // Call with (-INT_MAX to INT_MAX) alpha beta window to avoid int overflow issues
    int AlphaBetaNegamax(Figure::FigureSide side, int depth, int alpha, int beta, int& analyzed);

public:
    AI(Board* board, Rules* rules);
    ~AI();

    bool UseTranspositionTable;

    Move BestMoveByAlphaBeta(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed);
    Move NegamaxSearch(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed);

    //friend class Tests;
};

#endif // AI_H
