#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QVarLengthArray>
#include <QList>

// Simple types

typedef unsigned char POSITION;
typedef unsigned char UCHAR;
typedef signed char BYTE;
typedef signed short SHORT;
typedef int INT32;
typedef uint UINT32;
typedef bool BOOL;

typedef unsigned long long BITBOARD;

// Complex types

struct Move;
struct Figure;

//typedef QList<Move> MoveList;
//typedef QVarLengthArray<Figure*> FigureList;

typedef QVarLengthArray<POSITION, 64> PositionCollection;
typedef QVarLengthArray<Move, 32> MoveCollection;
typedef QList<Figure*> FigureList;
typedef QList<POSITION> PositionList;

#endif // TYPEDEFS_H
