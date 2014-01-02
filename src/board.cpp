#include <string>
#include "board.h"
#include "exception.h"
#include "figureposition.h"
#include "bitboardhelper.h"

Board::Board()
{
    m_turningSide = FigureSide::White;

    m_aliveFiguresVector = QVector<Figure*>(64);
}

Board::Board(const Board &another)
{
    m_turningSide = another.m_turningSide;

    m_positionHash = another.m_positionHash;
    m_positionCounter = another.m_positionCounter;

    m_aliveFiguresVector = QVector<Figure*>(64);

    m_positionHashHistory = another.m_positionHashHistory;

    // needs to make right history copy (replace move's history pointers)
    QMap<Figure*, Figure*> oldToNewFiguresMapping;

    foreach(Figure* f, another.m_allFigures)
    {
        // figures stored as pointer, because we have to copy it by value
        Figure* copy = new Figure(*f);

        oldToNewFiguresMapping[f] = copy;

        if (!another.IsDead(f))
        {
            AddAliveFigure(copy);
        }
        else
        {
            AddDeadFigure(copy);
        }
    }

    foreach(Move move, another.m_history)
    {
        // we are created figures by-value copies, we need to refresh move's figure pointers
        Move copy = Move(move.Type, move.From, move.To,
                         oldToNewFiguresMapping[move.MovingFigure],
                         oldToNewFiguresMapping[move.CapturedFigure]);

        m_history.append(copy);
    }
}

Board::~Board()
{
    foreach(Figure* f, m_allFigures)
    {
        delete f;
    }
}

bool Board::IsDead(Figure *figure) const
{
    if (!m_allFigures.contains(figure))
    {
        throw Exception("It not this board figure");
    }

    return !m_aliveFigures[figure->Side].contains(figure);
}

void Board::AddDeadFigure(Figure *figure)
{
    m_allFigures.append(figure);
    //m_deadFiguresMap[figure->Position.GetSerialNumber()].append(figure);
}

int Board::GetAfterLastCaptureOrPawnMoveHalfMoveCount() const
{
    int halfMoveCounter = 0;

    for (int i = m_history.count() - 1; i >= 0; --i)
    {
        if (m_history[i].Type == MoveType::Capture || m_history[i].MovingFigure->Type == FigureType::Pawn)
        {
            return halfMoveCounter;
        }
        else
        {
            ++halfMoveCounter;
        }
    }

    return halfMoveCounter;
}

int Board::GetFullMoveCount() const
{
    return 1 + m_history.count() / 2;
}

PositionHash Board::GetCurrentPositionHash() const
{
    return m_positionHash;
}

void Board::AddAliveFigure(Figure *figure)
{
    FigureSide side = figure->Side;

    m_allFigures.append(figure);
    m_aliveFigures[side].append(figure);

    m_aliveBitboard[side] = BitboardHelper::AddPosition(m_aliveBitboard[side], figure->Position);

    if (figure->Type == FigureType::King)
    {
        m_kings[side] = figure;
    }

    int key = PositionHelper::Serial(figure->Position);
    m_aliveFiguresVector[key] = figure;

    m_positionHash = PositionHash::Calculate(this);
}

void Board::MoveFigure(Figure *figure, POSITION newPosition)
{
    FigureSide side = figure->Side;

    int oldKey = PositionHelper::Serial(figure->Position);
    int newKey = PositionHelper::Serial(newPosition);

    m_aliveBitboard[side] = BitboardHelper::RemovePosition(m_aliveBitboard[side], figure->Position);
    m_aliveBitboard[side] = BitboardHelper::AddPosition(m_aliveBitboard[side], newPosition);

    if (m_aliveFiguresVector[newKey] != NULL)
    {
        throw Exception("Can not move figure because cell is not empty");
    }

    m_aliveFiguresVector[oldKey] = NULL;
    m_aliveFiguresVector[newKey] = figure;

    m_positionHash.Update(this, figure->Position);
    m_positionHash.Update(this, newPosition);

    figure->Position = newPosition;
}

void Board::KillFigure(Figure *figure)
{
    m_aliveBitboard[figure->Side] = BitboardHelper::RemovePosition(m_aliveBitboard[figure->Side], figure->Position);

    m_aliveFigures[figure->Side].removeOne(figure);

    int key = PositionHelper::Serial(figure->Position);

    m_aliveFiguresVector[key] = NULL;

    m_positionHash.Update(this, figure->Position);
}

void Board::ResurrectFigure(Figure *figure)
{
    m_aliveBitboard[figure->Side] = BitboardHelper::AddPosition(m_aliveBitboard[figure->Side], figure->Position);

    int key = PositionHelper::Serial(figure->Position);

    if (m_aliveFiguresVector[key] != NULL)
    {
        throw Exception("Can not reserrect because cell is empty");
    }

    m_aliveFigures[figure->Side].append(figure);
    m_aliveFiguresVector[key] = figure;

    m_positionHash.Update(this, figure->Position);
}

void Board::PromotePawn(Figure *pawn, FigureType type)
{
    if (type == FigureType::Pawn)
    {
        throw Exception("Not allowed promotion to pawn");
    }

    pawn->Type = type;

    m_positionHash.Update(this, pawn->Position);
}

void Board::UnpromotePawn(Figure *pawn)
{
    pawn->Type = FigureType::Pawn;

    m_positionHash.Update(this, pawn->Position);
}

Figure* Board::FigureAt(POSITION position) const
{
    int key = PositionHelper::Serial(position);

    return m_aliveFiguresVector[key];
}

FigureList Board::FiguresAt(FigureSide side) const
{
    return m_aliveFigures[side];
}

FigureList Board::GetAllAliveFigures() const
{
    FigureList alive = m_aliveFigures[FigureSide::White];
    alive.append(m_aliveFigures[FigureSide::Black]);

    return alive;
}

BITBOARD Board::GetBitboardFor(FigureSide side) const
{
    return m_aliveBitboard[side];
}

bool Board::HasFigureAt(POSITION position) const
{
    return FigureAt(position) != NULL;
}

bool Board::HasFigureAt(POSITION position, FigureSide side) const
{
    Figure* f = FigureAt(position);

    return f != NULL && f->Side == side;
}

void Board::PushToHistory(Move move)
{
    m_history.push_back(move);
}

void Board::PopHistory()
{
    m_history.pop_back();
}

void Board::TurnTransition()
{
    m_turningSide =
            m_turningSide == FigureSide::White ?
                FigureSide::Black
              : FigureSide::White;
}

void Board::IncreaseCurrentPositionCount()
{
    m_positionHashHistory.append(m_positionHash);
}

void Board::DecreaseCurrentPositionCount()
{
    if (m_positionHashHistory.isEmpty())
    {
        throw Exception("Invalid operation. Position history is empty");
    }

    m_positionHashHistory.pop_back();
}

int Board::GetCurrentPositionCount()
{
    int count = 0;

    foreach (const PositionHash& h, m_positionHashHistory)
    {
        if (h == m_positionHash)
        {
            ++count;
        }
    }

    return count;
}

Move Board::GetLastMove() const
{
    if (IsHistoryEmpty())
    {
        throw Exception("History is empty");
    }

    return m_history.last();
}

QStack<Move> Board::GetMoveHistory() const
{
    return m_history;
}

bool Board::IsHistoryEmpty() const
{
    return m_history.count() == 0;
}

Figure *Board::KingAt(FigureSide side) const
{
    return m_kings[side];
}

FigureSide Board::GetTurningSide() const
{
    return m_turningSide;
}

Board Board::StartPosition()
{
    Board board = Board();

    board.SetupStartPosition();

    return board;
}

void Board::SetupStartPosition()
{
    m_turningSide = FigureSide::White;

    AddAliveFigure(new Figure(FigureSide::White, FigureType::Queen, "d1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, "a1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, "h1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, "b1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, "g1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Bishop, "c1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Bishop, "f1"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::King, "e1"));

    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "a2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "b2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "c2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "d2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "e2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "f2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "g2"));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, "h2"));

    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Queen, "d8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Rock, "a8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Rock, "h8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Knight, "b8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Knight, "g8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Bishop, "f8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Bishop, "c8"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, "e8"));

    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "a7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "b7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "c7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "d7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "e7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "f7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "g7"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, "h7"));
}

void Board::SetupKings()
{
    AddAliveFigure(new Figure(FigureSide::White, FigureType::King, "e1"));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, "e8"));
}
