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

int Board::GetPlyCount() const
{
    return m_history.count();
}

int Board::GetAfterLastCaptureOrPawnMoveHalfMoveCount() const
{
    int halfMoveCounter = 0;

    for (int i = m_history.count() - 1; i >= 0; --i)
    {
        if (m_history[i].CapturedFigure != NULL || m_history[i].MovingFigure->Type == FigureType::Pawn)
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
    //pawn->Type = FigureType::Queen;

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
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Queen, PositionHelper::FromString("d1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, PositionHelper::FromString("a1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, PositionHelper::FromString("h1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, PositionHelper::FromString("b1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, PositionHelper::FromString("g1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Bishop, PositionHelper::FromString("c1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Bishop, PositionHelper::FromString("f1")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::King, PositionHelper::FromString("e1")));

    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("a2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("b2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("c2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("d2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("e2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("f2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("g2")));
    AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("h2")));

    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Queen, PositionHelper::FromString("d8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Rock, PositionHelper::FromString("a8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Rock, PositionHelper::FromString("h8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Knight, PositionHelper::FromString("b8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Knight, PositionHelper::FromString("g8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Bishop, PositionHelper::FromString("f8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Bishop, PositionHelper::FromString("c8")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, PositionHelper::FromString("e8")));

    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("a7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("b7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("c7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("d7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("e7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("f7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("g7")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("h7")));
}

void Board::SetupKings()
{
    AddAliveFigure(new Figure(FigureSide::White, FigureType::King, PositionHelper::FromString("e1")));
    AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, PositionHelper::FromString("e8")));
}
