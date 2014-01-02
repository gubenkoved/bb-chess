#include <string>
#include "board.h"
#include "exception.h"

Board::Board()
{
    m_turningSide = Figure::White;

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
            AddAliveFigure(copy);
        else
            AddDeadFigure(copy);
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
        throw Exception("It not this board figure");

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
        if (m_history[i].Type == Move::Capture || m_history[i].MovingFigure->Type == Figure::Pawn)
            return halfMoveCounter;
        else
            ++halfMoveCounter;
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
    m_allFigures.append(figure);

    Figure::FigureSide side = figure->Side;

    m_aliveFigures[side].append(figure);

    if (figure->Type == Figure::King)
        m_kings[side] = figure;

    int key = GetSerialNumber(figure->Position) - 1;

    m_aliveFiguresVector[key] = figure;

    m_positionHash = PositionHashCalculator::Calculate(this);
}

void Board::MoveFigure(Figure *figure, FigurePosition newPosition)
{
    int oldKey = GetSerialNumber(figure->Position) - 1;
    int newKey = GetSerialNumber(newPosition) - 1;

    if (m_aliveFiguresVector[newKey] != NULL)
        throw Exception("Can not move figure because cell is not empty");

    m_aliveFiguresVector[oldKey] = NULL;
    m_aliveFiguresVector[newKey] = figure;

    PositionHashCalculator::Update(m_positionHash, this, figure->Position);
    PositionHashCalculator::Update(m_positionHash, this, newPosition);

    figure->SetPosition(newPosition);
}

void Board::KillFigure(Figure *figure)
{
    m_aliveFigures[figure->Side].removeOne(figure);    

    int key = GetSerialNumber(figure->Position) - 1;

    m_aliveFiguresVector[key] = NULL;

    PositionHashCalculator::Update(m_positionHash, this, figure->Position);
}

void Board::ResurrectFigure(Figure *figure)
{
    int key = GetSerialNumber(figure->Position) - 1;

    if (m_aliveFiguresVector[key] != NULL)
        throw Exception("Can not reserrect because cell is empty");

    m_aliveFigures[figure->Side].append(figure);    
    m_aliveFiguresVector[key] = figure;

    PositionHashCalculator::Update(m_positionHash, this, figure->Position);
}

void Board::PromotePawn(Figure *pawn, Figure::FigureType type)
{
    if (type == Figure::Pawn)
        throw Exception("Not allowed promotion to pawn");

    pawn->SetFigureType(type);

    PositionHashCalculator::Update(m_positionHash, this, pawn->Position);
}

void Board::UnpromotePawn(Figure *pawn)
{
    pawn->SetFigureType(Figure::Pawn);

    PositionHashCalculator::Update(m_positionHash, this, pawn->Position);
}

Figure* Board::FigureAt(FigurePosition position) const
{
    int key = GetSerialNumber(position) - 1;

    return m_aliveFiguresVector[key];
}

FigureList Board::FiguresAt(Figure::FigureSide side) const
{
    return m_aliveFigures[side];
}

FigureList Board::GetAllAliveFigures() const
{
    FigureList alive = m_aliveFigures[Figure::White];
    alive.append(m_aliveFigures[Figure::Black]);

    return alive;
}

bool Board::HasFigureAt(FigurePosition position) const
{
    return FigureAt(position) != NULL;
}

bool Board::HasFigureAt(FigurePosition position, Figure::FigureSide side) const
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
    m_turningSide = m_turningSide == Figure::White ? Figure::Black : Figure::White;
}

void Board::IncreaseCurrentPositionCount()
{
    m_positionHashHistory.push_back(m_positionHash);
//    if (m_positionCounter.contains(m_positionHash))
//        m_positionCounter[m_positionHash] += 1;
//    else
//        m_positionCounter[m_positionHash] = 1;
}

void Board::DecreaseCurrentPositionCount()
{
    if (m_positionHashHistory.isEmpty())
        throw Exception("Invalid operation. Position history is empty");

    m_positionHashHistory.pop_back();
//    int count = --m_positionCounter[m_positionHash];

//    if (count < 0)
//        throw Exception("Invalid operation. Position count can not be negative");

//    if (count == 0)
//        m_positionCounter.remove(m_positionHash);
}

int Board::GetCurrentPositionCount()
{
    int count = 0;

    foreach (QString s, m_positionHashHistory)
    {
        if (s == m_positionHash)
            ++count;
    }

    return count;
    //return m_positionCounter[m_positionHash];
}

Move Board::GetLastMove() const
{
    if (IsHistoryEmpty())
        throw Exception("History is empty");

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

Figure *Board::KingAt(Figure::FigureSide side) const
{
    return m_kings[side];
}

Figure::FigureSide Board::GetTurningSide() const
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
    m_turningSide = Figure::White;

    AddAliveFigure(new Figure(Figure::White, Figure::Queen, "d1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Rock, "a1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Rock, "h1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Knight, "b1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Knight, "g1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Bishop, "c1"));
    AddAliveFigure(new Figure(Figure::White, Figure::Bishop, "f1"));
    AddAliveFigure(new Figure(Figure::White, Figure::King, "e1"));

    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "a2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "b2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "c2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "d2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "e2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "f2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "g2"));
    AddAliveFigure(new Figure(Figure::White, Figure::Pawn, "h2"));

    AddAliveFigure(new Figure(Figure::Black, Figure::Queen, "d8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Rock, "a8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Rock, "h8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Knight, "b8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Knight, "g8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Bishop, "f8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Bishop, "c8"));
    AddAliveFigure(new Figure(Figure::Black, Figure::King, "e8"));

    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "a7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "b7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "c7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "d7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "e7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "f7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "g7"));
    AddAliveFigure(new Figure(Figure::Black, Figure::Pawn, "h7"));
}

void Board::SetupKings()
{
    AddAliveFigure(new Figure(Figure::White, Figure::King, "e1"));
    AddAliveFigure(new Figure(Figure::Black, Figure::King, "e8"));
}
