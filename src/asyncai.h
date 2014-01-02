#ifndef ASYNCAI_H
#define ASYNCAI_H

#include <QThread>
#include <QSharedPointer>
#include <QScopedPointer>

#include "board.h"
#include "move.h"
#include "figure.h"

class AsyncAI : public QThread
{
    Q_OBJECT

    Board* m_sourceBoard;
    FigureSide m_side;
    int m_depth;

    void run(); // this function will be called in another thread
public:
    explicit AsyncAI(Board* board, QObject *parent = 0);

    void StartBestMoveSearch(FigureSide side, int depth);
signals:
    void BestMoveFinded(QString bestMove);
public slots:

};

#endif // ASYNCAI_H
