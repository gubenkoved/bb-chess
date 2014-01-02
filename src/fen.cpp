#include "fen.h"

QString FEN::Evaluate(Board *board)
{
    QString fen;

    // figures location
    for (int y = 8; y >= 1; y--)
    {
        int emptyCounter = 0;
        for (int x = 1; x <= 8; ++x)
        {
            FigurePosition p = CreateFigurePosition(x, y);
            Figure* figure = board->FigureAt(p);

            if (figure != NULL)
            {
                if (emptyCounter != 0)
                {
                    fen += QString::number(emptyCounter);
                    emptyCounter = 0;
                }

                fen += figure->GetFENChar();
            } else
            {
                emptyCounter ++;
            }
        }

        if (emptyCounter != 0)
        {
            fen += QString::number(emptyCounter);
        }

        if (y != 1)
            fen += '/';
    }

    fen += ' ';

    // turning side
    fen += board->GetTurningSide() == Figure::White ? 'w' : 'b';

    fen += ' ';

    // castlings possibilities

    bool noCastlingsAtAll = true;

    // white
    Figure* wKing = board->KingAt(Figure::White);
    Figure* wRock = board->FigureAt(CreateFigurePosition(8, 1));
    Figure* wlRock = board->FigureAt(CreateFigurePosition(1, 1));
    // short white castling
    if (wKing->MovesCount == 0 && wRock != NULL && wRock->MovesCount == 0)
    {
        noCastlingsAtAll = false;
        fen += 'K';
    }
    // long white castling
    if (wKing->MovesCount == 0 && wlRock != NULL && wlRock->MovesCount == 0)
    {
        noCastlingsAtAll = false;
        fen += 'Q';
    }

    // black
    Figure* bKing = board->KingAt(Figure::Black);
    Figure* bRock = board->FigureAt(CreateFigurePosition(8, 8));
    Figure* blRock = board->FigureAt(CreateFigurePosition(1, 8));
    // short black castling
    if (bKing->MovesCount == 0 && bRock != NULL && bRock->MovesCount == 0)
    {
        noCastlingsAtAll = false;
        fen += 'k';
    }
    // long black castling
    if (bKing->MovesCount == 0 && blRock != NULL && blRock->MovesCount == 0)
    {
        noCastlingsAtAll = false;
        fen += 'q';
    }

    if (noCastlingsAtAll)
        fen += '-';

    fen += ' ';

    // En passant target square
    if (!board->IsHistoryEmpty())
    {
        Move lastMove = board->GetLastMove();
        Figure::FigureSide side = lastMove.MovingFigure->Side;

        if (lastMove.Type == Move::LongPawn)
        {
            fen += ToString(CreateFigurePosition(GetX(lastMove.From), GetY(lastMove.From) + (side == Figure::White ? 1 : -1)));
        } else
        {
            fen += '-';
        }
    } else
    {
        fen += '-';
    }

    fen += ' ';

    // half and full move counters
    int halfMoveCount = board->GetAfterLastCaptureOrPawnMoveHalfMoveCount();
    int fullMoveCount = board->GetFullMoveCount();

    fen += QString::number(halfMoveCount);

    fen += ' ';

    fen += QString::number(fullMoveCount);

    return fen;
}
