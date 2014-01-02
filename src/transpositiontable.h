#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <QHash>

#include "positionhash.h"

class TranspositionTableEntry
{
    PositionHash m_hash;
    int m_estimation;
    int m_depth;
public:
    TranspositionTableEntry(PositionHash hash, int estimation, int depth);
    TranspositionTableEntry(const TranspositionTableEntry& another);    

    const PositionHash& Hash;
    const int& Estimation;
    const int& Depth;
};

class TranspositionTable
{
    //int m_maxEntryAmount;
    QHash<PositionHash, TranspositionTableEntry*> m_table;

public:
    TranspositionTable();
    ~TranspositionTable();

    void Store(TranspositionTableEntry entry);
    void Store(PositionHash hash, int estimation, int depth);
    int Count();
    void Clear();

    const TranspositionTableEntry* FindEntry(const PositionHash& hash) const;
};

#endif // TRANSPOSITIONTABLE_H
