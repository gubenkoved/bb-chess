#ifndef BOARD_H
#define BOARD_H

#include <QList>
#include <QHash>
#include <QVector>
#include <QPair>
#include <QStack>

#include "figure.h"
#include "move.h"
#include "positionhashcalculator.h"

typedef QList<Figure*> FigureList;


// Board representation:
//      _______________
// 8   |  %   %   %   %|
// 7   |%   %   %   %  |
// 6   |  %   %   %   %|
// 5   |%   %   %   %  |
// 4   |  %   %   %   %|
// 3   |%   %   %   %  |
// 2   |  %   %   %   %|
// 1   |%   %   %   %  |
//     ^^^^^^^^^^^^^^^^^
//      a b c d e f g h

class Board
{
    PositionHash m_positionHash;
    QMap<PositionHash, int> m_positionCounter; // amount of every position which was detected
    //QList<QPair<PositionHash, int> > m_positionCounter; // amount of every position which was detected
    QStack<PositionHash> m_positionHashHistory;

    //MoveList m_history;
    QStack<Move> m_history;
    Figure::FigureSide m_turningSide;

    FigureList m_allFigures; // contains all figures, black and white, dead and alive
    QVector<Figure*> m_aliveFiguresVector; // contains only alive figures, where key is LightFigurePosition generated unique key
    QMap<Figure::FigureSide, FigureList> m_aliveFigures; // fast access to alive figures on side
    QMap<Figure::FigureSide, Figure*> m_kings; // fast access to kings

    bool IsDead(Figure* figure) const;
    void AddDeadFigure(Figure* figure); // for deep copy
public:
    Board();

    Board(const Board& another); // returns fully independent deep copy
    ~Board();

    void SetupStartPosition();
    void SetupKings();

    void AddAliveFigure(Figure* figure);
    FigureList GetAllAliveFigures() const;

    void MoveFigure(Figure* figure, FigurePosition newPosition);
    void KillFigure(Figure* figure);
    void ResurrectFigure(Figure* figure);
    void PromotePawn(Figure* pawn, Figure::FigureType type);
    void UnpromotePawn(Figure* pawn);

    void PushToHistory(Move move);
    void PopHistory();
    bool IsHistoryEmpty() const;
    Move GetLastMove() const;
    QStack<Move> GetMoveHistory() const;

    void TurnTransition();
    Figure::FigureSide GetTurningSide() const;

    void IncreaseCurrentPositionCount();
    void DecreaseCurrentPositionCount();
    int GetCurrentPositionCount();

    Figure* FigureAt(FigurePosition position) const;
    Figure* KingAt(Figure::FigureSide side) const;
    FigureList FiguresAt(Figure::FigureSide side) const;
    bool HasFigureAt(FigurePosition position) const;
    bool HasFigureAt(FigurePosition position, Figure::FigureSide side) const;

    int GetAfterLastCaptureOrPawnMoveHalfMoveCount() const; // returns number of halfmoves since the last pawn advance or capture
    int GetFullMoveCount() const; // returns number of the full move. It starts at 1, and is incremented after Black's move

    PositionHash GetCurrentPositionHash() const;

    static Board StartPosition();
};

#endif // BOARD_H
