#ifndef FIGURE_H
#define FIGURE_H

#include <QString>
#include <QChar>
#include "lightfigureposition.h"

class Figure
{
public:
    enum FigureType
    {
        Pawn,
        Knight,
        Bishop,
        Rock,
        King,
        Queen
    };
    enum FigureSide
    {
        White,
        Black
    };

private:
    FigureType m_type;
    FigureSide m_side;
    FigurePosition m_position;
    int m_movesCounter;
public:

    Figure(FigureSide side, FigureType type, FigurePosition position);
    Figure(FigureSide side, FigureType type, const char position[]);
    Figure(const Figure& another);

    //void Promote(Figure::FigureType type = Figure::Queen);
    //void UnPromote();
    void SetFigureType(Figure::FigureType type);
    void IncreaseMovesCounter();
    void DecreaseMovesCounter();

    const FigureType& Type;
    const FigureSide& Side;
    const FigurePosition& Position;
    const int& MovesCount;

    // Note: To save integrity should be used only from board class
    void SetPosition(FigurePosition position);
    bool IsWhite() const;
    bool IsBlack() const;
    QChar GetUnicodeChessChar() const;
    QChar GetFENChar() const;
    QString GetName() const;
};

inline QDebug operator<<(QDebug debug, const Figure& f)
{
    debug.nospace() << "Figure("
        << f.GetName() << " at "
        << f.Position << ")";
    return debug.space();
}
#endif // FIGURE_H
