#include "bitboardhelper.h"
#include "typedefs.h"
#include "figureposition.h"

PositionCollection BitboardHelper::GetPositions(BITBOARD bitboard)
{
    PositionCollection positions;

    for(int idx = 0; idx < 64; ++idx)
    {
        if (bitboard & (1ULL << idx))
        {
            positions.append(PositionHelper::FromSerial(idx));
        }
    }

    return positions;
}

BITBOARD BitboardHelper::AddPosition(BITBOARD bitboard, POSITION p)
{
    return bitboard | (1ULL << PositionHelper::Serial(p));
}

BITBOARD BitboardHelper::RemovePosition(BITBOARD bitboard, POSITION p)
{
    return bitboard & (~ (1ULL << PositionHelper::Serial(p)));
}

BITBOARD BitboardHelper::AddPositionWhenValid(BITBOARD bitboard, POSITION p)
{
    if (!PositionHelper::IsInvalid(p))
    {
        return bitboard | (1ULL << PositionHelper::Serial(p));
    } else
    {
        return bitboard;
    }
}

BITBOARD BitboardHelper::Union(BITBOARD bitboard1, BITBOARD bitboard2)
{
    return bitboard1 | bitboard2;
}

BITBOARD BitboardHelper::Substract(BITBOARD bitboard1, BITBOARD bitboard2)
{
    return bitboard1 & (~bitboard2);
}

bool BitboardHelper::Contains(BITBOARD bitboard, POSITION p)
{
    return bitboard & (1ULL << PositionHelper::Serial(p));
}
