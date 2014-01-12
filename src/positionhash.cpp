#include "positionhash.h"
#include "board.h"

// values doesn't matter - only requirements:
//  1. it must be different
//  2. must be less than 16 (2^4 to occupy 4 bit)
//  3. must be not 0 (0 is occupied by empty square)
UINT32 PositionHash::m_hashElements[SIDE_COUNT][FIGURE_TYPE_COUNT] =
{
    {1, 2, 3, 4, 5, 6}, // for whites
    {7, 8, 9, 10, 11, 12} // for blacks
};

PositionHash::PositionHash()
{
    for(int i = 0; i < 8; ++i)
    {
        m_data[i] = 0;
    }
}

bool PositionHash::operator==(const PositionHash& another) const
{
    for(int i = 0; i < 8; ++i)
    {
        if (m_data[i] != another.m_data[i])
        {
            return false;
        }
    }

    return true;
}

uint PositionHash::qHash() const
{
    uint result = 0;

    for(int i = 0; i < 8; ++i)
    {
        result ^= m_data[i];
    }

    return result;
}

PositionHash PositionHash::Calculate(const Board* board)
{
    PositionHash hash = PositionHash();

    foreach (Figure* figure, board->GetAllAliveFigures())
    {
        hash.Update(board, figure->Position);
    }

    return hash;
}

void PositionHash::Update(const Board* board, POSITION position)
{
    int serial = PositionHelper::Serial(position); // [0; 63]
    int groupNum = serial / 8; // [0; 7]
    int offset = 4 * (serial % 8); // in {0, 4, 8, 12, 16, 20, 24, 28}

    m_data[groupNum] &= ~(0xF << offset); // zero hash element for specified position

    Figure* figureAtPosition = board->FigureAt(position);
    if (figureAtPosition != NULL)
    {
        // fill element when there is figure at specified position
        UINT32 hashElement = m_hashElements[(int)figureAtPosition->Side][(int)figureAtPosition->Type];
        m_data[groupNum] |= hashElement << offset;
    }
}


