#ifndef POSITIONHASH_H
#define POSITIONHASH_H

#include "figure.h"
#include "typedefs.h"

class Board;

// 13 possible square states (2x 6 figure types + 1 empty) -> 4 bit per square
// 4x 64 = 256 bit = 8x 32
// i.e. each posistion can be represented by eight 32-bit numbers

class PositionHash
{
    UINT32 m_data[8];

    // array that in first 4 bits of each value contains
    // hash for some figure side and figure type
    // Note: hash's data combined from this hash elements and binary 0000 when square is empty
    static UINT32 m_hashElements[SIDE_COUNT][FIGURE_TYPE_COUNT];
public:
    PositionHash();

    bool operator==(const PositionHash& another) const;
    uint qHash() const;

    void Update(const Board* board, POSITION position);
    static PositionHash Calculate(const Board* board);
};

// implement this to be used as key in QT's QHash container
inline uint qHash(const PositionHash& key)
{
    return key.qHash();
}

#endif // POSITIONHASH_H
