#ifndef BITBOARDHELPER_H
#define BITBOARDHELPER_H

#include "typedefs.h"
#include "figureposition.h"

#define BITBOARD_EMPTY 0ULL

class BitboardHelper
{
public:
    static PositionCollection GetPositions(BITBOARD bitboard);

    static BITBOARD AddPosition(BITBOARD bitboard, POSITION p);
    static BITBOARD AddPositionWhenValid(BITBOARD bitboard, POSITION p);

    static BITBOARD RemovePosition(BITBOARD bitboard, POSITION p);

    static BITBOARD Union(BITBOARD bitboard1, BITBOARD bitboard2);
    static BITBOARD Substract(BITBOARD bitboard1, BITBOARD bitboard2);

    static bool Contains(BITBOARD bitboard, POSITION position);
};

#endif // BITBOARDHELPER_H
