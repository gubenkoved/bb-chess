#include "enumshelper.h"

#include "figure.h"
#include "move.h"
#include "puzzle.h"
#include "exception.h"

QString EnumsHelper::ToString(FigureSide figureSide)
{
    switch (figureSide)
    {
        case FigureSide::White:     return "White";
        case FigureSide::Black:     return "Black";
        case FigureSide::Invalid:   return "Invalid";

        default: throw Exception("Unknown enum value");
    }
}

QString EnumsHelper::ToString(FigureType figureType)
{
    switch (figureType)
    {
        case FigureType::Pawn:      return "Pawn";
        case FigureType::Bishop:    return "Bishop";
        case FigureType::Knight:    return "Knight";
        case FigureType::Rock:      return "Rock";
        case FigureType::Queen:     return "Queen";
        case FigureType::King:      return "King";
        case FigureType::Invalid:   return "Invalid";

        default: throw Exception("Unknown enum value");
    }
}

QString EnumsHelper::ToString(MoveType moveType)
{
    switch(moveType)
    {
        case MoveType::Normal:          return "Normal";
        case MoveType::LongPawn:        return "LongPawn";
        case MoveType::ShortCastling:   return "ShortCastling";
        case MoveType::LongCastling:    return "LongCastling";
        case MoveType::PawnPromotion:   return "PawnPromotion";
        case MoveType::EnPassant:       return "EnPassant";
        case MoveType::Invalid:         return "Invalid";

        default: throw Exception("Unknown enum value");
    }
}

QString EnumsHelper::ToString(PuzzleGoal puzzleGoal)
{
    switch(puzzleGoal)
    {
        case PuzzleGoal::MateInTwo: return "MateInTwo";

        default: throw Exception("Unknown enum value");
    }
}
