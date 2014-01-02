#include "transpositiontable.h"

TranspositionTableEntry::TranspositionTableEntry(PositionHash hash, int estimation, int depth)
    :Hash(m_hash), Estimation(m_estimation), Depth(m_depth)
{
    m_hash = hash;
    m_estimation = estimation;
    m_depth = depth;
}

TranspositionTableEntry::TranspositionTableEntry(const TranspositionTableEntry &another)
    :Hash(m_hash), Estimation(m_estimation), Depth(m_depth)
{
    m_hash = another.m_hash;
    m_estimation = another.m_estimation;
    m_depth = another.m_depth;
}

TranspositionTable::TranspositionTable()
{
}


TranspositionTable::~TranspositionTable()
{
    foreach (TranspositionTableEntry* entry, m_table.values())
    {
        delete entry;
    }
}

void TranspositionTable::Store(TranspositionTableEntry entry)
{
    TranspositionTableEntry* old = m_table[entry.Hash];

    if (old == NULL)
    {
        m_table[entry.Hash] = new TranspositionTableEntry(entry);
    } else if (entry.Depth > old->Depth) // old != NULL
    {
        delete old;

        m_table[entry.Hash] = new TranspositionTableEntry(entry);
    }
}

void TranspositionTable::Store(PositionHash hash, int estimation, int depth)
{
    Store(TranspositionTableEntry(hash, estimation, depth));
}

int TranspositionTable::Count()
{
    return m_table.count();
}

void TranspositionTable::Clear()
{
    m_table.clear();
}

const TranspositionTableEntry* TranspositionTable::FindEntry(const PositionHash &hash) const
{
    return m_table[hash];
}


