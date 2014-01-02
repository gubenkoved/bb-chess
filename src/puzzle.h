#ifndef PUZZLE_H
#define PUZZLE_H

#include <QList>
#include <QString>
#include <QDate>
#include <QDebug>

enum PuzzleGoal
{
    MateInTwo // whites begins and should checkmate in two steps
};

struct Puzzle
{
    int         Id;
    PuzzleGoal  Goal;
    QString     Authors;
    QString     Source;
    QDate       Date;
    QString     PositionFEN;

    Puzzle();

    static QList<Puzzle> Parse(QString filename);

private:
    static Puzzle Read(QString line);
    static QList<QString> TokenizeLine(QString line);
};

inline QDebug operator<<(QDebug debug, const Puzzle& p)
{
    debug << "Puzzle("
        << p.Id
        << p.Authors
        << p.Source
        << p.Goal
        << p.Date.toString("dd.MM.yyyy")
        << p.PositionFEN
        << ")";

    return debug;
}

#endif // PUZZLE_H
