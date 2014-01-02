#include <QTime>

#include "asyncai.h"
#include "rules.h"
#include "ai.h"

AsyncAI::AsyncAI(Board* board, QObject *parent) :
    QThread(parent)
{    
    m_depth = -1;
    m_side = Figure::White;
    m_sourceBoard = board;
}

void AsyncAI::StartBestMoveSearch(Figure::FigureSide side, int depth)
{
    m_side = side;
    m_depth = depth;

    start();
}

void AsyncAI::run()
{
    // use source board deep copy
    Board board = Board(*m_sourceBoard);
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int bestEstimation;
    int analyzed;

    QTime timer;
    timer.start();

    Move bestMove = ai.BestMoveByAlphaBeta(m_side, m_depth, bestEstimation, analyzed);

    qDebug() << "AsyncAI::run(): analyzed" << analyzed << "positions, best estimation:" << bestEstimation
             << "[" << timer.elapsed() << "ms]";

    QString bestMoveString = ToString(bestMove.From) + " " + ToString(bestMove.To);

    emit BestMoveFinded(bestMoveString);
}
