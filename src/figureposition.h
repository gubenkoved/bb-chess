#ifndef LIGHTFIGUREPOSITION_H
#define LIGHTFIGUREPOSITION_H

#include <QString>
#include <QDebug>

#include "typedefs.h"

// POSITION BYTE LAYOUT
// -----------------------------------------
// uses 8-byte figure position representation
//
// 1 2 3 4 5 6 7 8
// ^ ^ ^ ^ ^ ^ ^ ^
// | | | | | |_|_|__y
// | | |_|_|__x
// | |__unused bit
// invalid indicator bit

#define POS_INVALID 0x80 /* 1000 0000 */
#define POS_X       0x38 /* 0011 1000 */
#define POS_Y       0x07 /* 0000 0111 */

struct PositionHelper
{

    // returns x-coordinate from position
    // value in [1; 8]; 1 means a, ..., 8 means h
    //#define PositionHelper::X(p) (((p & 0x38) >> 3) + 1)
    static INT32 X(POSITION pos);

    // returns y-coordinate position
    // value in [1; 8];
    static INT32 Y(POSITION pos);

    // returns position's serial number
    // value in [0; 63]
    // 0 for a1
    // 1 for b1,
    // ...,
    // 63 for h8
    static INT32 Serial(POSITION pos);

    static POSITION FromSerial(INT32 serial);

    // checks bit that is invalid position indicator
    // returns not 0 when position is invalid
    static BOOL IsInvalid(POSITION pos);

    // returns true when x in [1; 8] and y in [1; 8]
    static BOOL IsValidPosition(int x, int y);

    static POSITION Create(int x, int y);

    static POSITION Shift(POSITION position, int dx, int dy);

    // Returns position from it's string representation
    static POSITION FromString(std::string str);

    // Returns string position representation
    // e.g.: a1, h8
    static QString ToString(POSITION p);
};

inline INT32 PositionHelper::X(POSITION pos)
{
    return ((pos & POS_X) >> 3) + 1;
}

inline INT32 PositionHelper::Y(POSITION pos)
{
    return (pos & POS_Y) + 1;
}

inline INT32 PositionHelper::Serial(POSITION pos)
{
    return pos;
}

inline POSITION PositionHelper::FromSerial(INT32 serial)
{
    return (POSITION)serial;
}

inline BOOL PositionHelper::IsInvalid(POSITION pos)
{
    return pos & POS_INVALID;
}

inline BOOL PositionHelper::IsValidPosition(int x, int y)
{
    return x >= 1 && x <= 8 && y >= 1 && y <= 8;
}

static inline QDebug operator<<(QDebug debug, POSITION pos)
{
    debug.nospace() << PositionHelper::ToString(pos);

    return debug.space();
}
#endif // LIGHTFIGUREPOSITION_H
