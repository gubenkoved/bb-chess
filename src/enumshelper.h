#ifndef ENUMSHELPER_H
#define ENUMSHELPER_H

#include <QString>

enum class FigureSide;
enum class FigureType;
enum class MoveType;
enum class PuzzleGoal;

class EnumsHelper
{
public:
    static QString ToString(FigureSide figureSide);
    static QString ToString(FigureType figureType);
    static QString ToString(MoveType moveType);
    static QString ToString(PuzzleGoal puzzleGoal);
};

#endif // ENUMSHELPER_H
