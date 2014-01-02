#include "fen.h"
#include "exception.h"

QChar FEN::GetFigureChar(const Figure& figure)
{
    bool isWhite = (figure.Side == FigureSide::White);

    QChar c;
    switch (figure.Type)
    {
        case FigureType::Pawn:      c = isWhite ? 'P' : 'p'; break;
        case FigureType::Bishop:    c = isWhite ? 'B' : 'b'; break;
        case FigureType::Knight:    c = isWhite ? 'N' : 'n'; break;
        case FigureType::Rock:      c = isWhite ? 'R' : 'r'; break;
        case FigureType::Queen:     c = isWhite ? 'Q' : 'q'; break;
        case FigureType::King:      c = isWhite ? 'K' : 'k'; break;

        default: throw Exception("Invalid figure type. How did we get here?");
    }

    return c;
}

Figure FEN::FigureFromChar(QChar c)
{
    FigureSide side;
    FigureType type;

    QChar lc = c.toLower();

    if (lc == 'p') type = FigureType::Pawn;
    else if (lc == 'b') type = FigureType::Bishop;
    else if (lc == 'n') type = FigureType::Knight;
    else if (lc == 'r') type = FigureType::Rock;
    else if (lc == 'q') type = FigureType::Queen;
    else if (lc == 'k') type = FigureType::King;
    else throw Exception(QString("'%1' is invalid FEN char for figure").arg(QString(c)).toStdString());

    side = (lc == c) ? FigureSide::Black : FigureSide::White;

    return Figure(side, type, PositionHelper::Create(0, 0));
}

QString FEN::Evaluate(const Board& board)
{
    QString fen;

    // figures location
    for (int y = 8; y >= 1; y--)
    {
        int emptyCounter = 0;
        for (int x = 1; x <= 8; ++x)
        {
            POSITION p = PositionHelper::Create(x, y);
            Figure* figure = board.FigureAt(p);

            if (figure != NULL)
            {
                if (emptyCounter != 0)
                {
                    fen += QString::number(emptyCounter);
                    emptyCounter = 0;
                }

                fen += GetFigureChar(*figure);
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
        {
            fen += '/';
        }
    }

    fen += ' ';

    // turning side
    fen += board.GetTurningSide() == FigureSide::White ? 'w' : 'b';

    fen += ' ';

    // castlings possibilities

    bool noCastlingsAtAll = true;

    // white
    Figure* wKing = board.KingAt(FigureSide::White);
    Figure* wRock = board.FigureAt(PositionHelper::Create(8, 1));
    Figure* wlRock = board.FigureAt(PositionHelper::Create(1, 1));
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
    Figure* bKing = board.KingAt(FigureSide::Black);
    Figure* bRock = board.FigureAt(PositionHelper::Create(8, 8));
    Figure* blRock = board.FigureAt(PositionHelper::Create(1, 8));
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
    if (!board.IsHistoryEmpty())
    {
        Move lastMove = board.GetLastMove();
        FigureSide side = lastMove.MovingFigure->Side;

        if (lastMove.Type == MoveType::LongPawn)
        {
            fen += PositionHelper::ToString(
                        PositionHelper::Create(
                            PositionHelper::X(lastMove.From),
                            PositionHelper::Y(lastMove.From) + (side == FigureSide::White ? 1 : -1)));
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
    int halfMoveCount = board.GetAfterLastCaptureOrPawnMoveHalfMoveCount();
    int fullMoveCount = board.GetFullMoveCount();

    fen += QString::number(halfMoveCount);

    fen += ' ';

    fen += QString::number(fullMoveCount);

    return fen;
}

Board FEN::PositionFromFEN(QString fen)
{
    try
    {
        Board board;
        int cIdx = -1;

        int y = 8;
        int x = 1;

        while(++cIdx < fen.length())
        {
            if (y == 1 && x == 9) // filfilled
            {
                break;
            }

            QChar c = fen.at(cIdx);

            if (c == '/') // new line separator
            {
                --y;

                if (x != 9)
                {
                    throw Exception(QString("Fen separator was not expected in position: %1 (%2)").arg(QString::number(cIdx)).arg(fen).toStdString());
                }

                x = 1;
                continue;
            }

            if (c.isDigit())
            {
                x += c.digitValue(); // skip specified number of cells
            } else
            {
                Figure f = FigureFromChar(c);
                f.Position = PositionHelper::Create(x, y);

                ++x; // go to next cell

                board.AddAliveFigure(new Figure(f));
            }
        }

        // string is over, do the last check
        if (x != 9 || y != 1)
        {
            throw Exception("Invalid final state. Board in not filled completely or overfilled: " + fen.toStdString());
        }

        return board;
    } catch (Exception e)
    {
        // rethrow our Exception with FEN
        throw Exception(e.GetMessage() + "(" + fen.toStdString() + ")");
    }
    catch (...) // compose our Exception from any other type
    {
        throw Exception("Loading failed. FEN string is invalid: " + fen.toStdString());
    }
}
