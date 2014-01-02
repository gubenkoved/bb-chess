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
    Figure::FigureSide m_side;
    int m_depth;

    void run(); // this function will be called in another thread
public:
    explicit AsyncAI(Board* board, QObject *parent = 0);
    
    void StartBestMoveSearch(Figure::FigureSide side, int depth);
signals:
    void BestMoveFinded(QString move);
public slots:
    
};

#endif // ASYNCAI_H
