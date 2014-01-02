#ifndef BOARDSERIALIZER_H
#define BOARDSERIALIZER_H

#include "board.h"
#include "rules.h"

class BoardSerializer
{
public:

    static QString Save(const Board& board);
    static Board Load(QString boardString);
};

#endif // BOARDSERIALIZER_H
