#include <algorithm>
#include "rules.h"
#include "exception.h"

Rules::Rules(Board *board)
    :m_board(board)
{
}

template<class T> void Rules::AppendIfNotNull(QList<T>* list, T value) const
{
    if (value != NULL)
        list->append(value);
}

template<class T> void Rules::AppendIfNotExists(QList<T>* list, T value) const
{
    if (!list->contains(value))
        list->append(value);
}

template<class T> void Rules::AppendIfNotExists(QList<T>* list, QList<T> values) const
{
    foreach(T value, values)
    {
        AppendIfNotExists(list, value);
    }
}

void Rules::AppendIfValid(PositionList *list, PositionList positions) const
{
    foreach(FigurePosition p, positions)
    {
        AppendIfValid(list, p);
    }
}

void Rules::AppendIfValid(PositionList *list, FigurePosition position) const
{
    if (!IsInvalid(position))
    {
        list->append(position);
    }
}

void Rules::AppendIfValidAndNotExists(PositionList *list, PositionList positions) const
{
    foreach(FigurePosition p, positions)
    {
        AppendIfValidAndNotExists(list, p);
    }
}
void Rules::AppendIfValidAndNotExists(PositionList *list, FigurePosition position) const
{
    if (!IsInvalid(position))
    {
        AppendIfNotExists(list, position);
    }
}

PositionList Rules::GetOnLinePositions(FigurePosition position, Figure::FigureSide side, int xMult, int yMult, int lenLimit = 7) const
{
    if (xMult != -1 && xMult != 0 && xMult != 1 || yMult != -1 && yMult != 0 && yMult != 1)
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");

    PositionList guarded;    

    for (int delta = 1; delta <= lenLimit; ++delta)
    {
        FigurePosition p = ForwardFor(position, side, delta * xMult, delta * yMult);

        if (!IsInvalid(p))
        {
            guarded.append(p);

            if (m_board->FigureAt(p) != NULL) // obstacle
            {
                break;
            }
        } else
        {
            break;
        }
    }

    return guarded;
}

PositionList Rules::GetPawnGuardedPositions(Figure *figure) const
{
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 1));
    guarded.append(GetOnLinePositions(p, side, -1, +1, 1));

    return guarded;
}

PositionList Rules::GetKinghtGuardedPositions(Figure *figure) const
{
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    AppendIfValid(&guarded, ForwardFor(p, side, +2, +1));
    AppendIfValid(&guarded, ForwardFor(p, side, +1, +2));
    AppendIfValid(&guarded, ForwardFor(p, side, -1, +2));
    AppendIfValid(&guarded, ForwardFor(p, side, -2, +1));

    AppendIfValid(&guarded, ForwardFor(p, side, -2, -1));
    AppendIfValid(&guarded, ForwardFor(p, side, -1, -2));
    AppendIfValid(&guarded, ForwardFor(p, side, +1, -2));
    AppendIfValid(&guarded, ForwardFor(p, side, +2, -1));

    return guarded;
}

PositionList Rules::GetBishopGuardedPositions(Figure *figure) const
{
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, -1, 7)); // backward-right
    guarded.append(GetOnLinePositions(p, side, -1, +1, 7)); // forawrd-left
    guarded.append(GetOnLinePositions(p, side, -1, -1, 7)); // forward-right

    return guarded;
}

PositionList Rules::GetRockGuardedPositions(Figure *figure) const
{    
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, 0, 7)); // right
    guarded.append(GetOnLinePositions(p, side, -1, 0, 7)); // left
    guarded.append(GetOnLinePositions(p, side, 0, +1, 7)); // forward
    guarded.append(GetOnLinePositions(p, side, 0, -1, 7)); // backward

    return guarded;
}

PositionList Rules::GetQueenGuardedPositions(Figure *figure) const
{
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, -1, 7)); // backward-right
    guarded.append(GetOnLinePositions(p, side, -1, +1, 7)); // forawrd-left
    guarded.append(GetOnLinePositions(p, side, -1, -1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, 0, 7)); // right
    guarded.append(GetOnLinePositions(p, side, -1, 0, 7)); // left
    guarded.append(GetOnLinePositions(p, side, 0, +1, 7)); // forward
    guarded.append(GetOnLinePositions(p, side, 0, -1, 7)); // backward

    return guarded;
}

PositionList Rules::GetKingGuardedPositions(Figure *figure) const
{
    FigurePosition p = figure->Position;
    Figure::FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 1)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, -1, 1)); // backward-right
    guarded.append(GetOnLinePositions(p, side, -1, +1, 1)); // forawrd-left
    guarded.append(GetOnLinePositions(p, side, -1, -1, 1)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, 0, 1)); // right
    guarded.append(GetOnLinePositions(p, side, -1, 0, 1)); // left
    guarded.append(GetOnLinePositions(p, side, 0, +1, 1)); // forward
    guarded.append(GetOnLinePositions(p, side, 0, -1, 1)); // backward

    return guarded;
}

bool Rules::IsUnderCheck(Figure::FigureSide side) const
{
    //return IsUnderCheckImpl(side);
    return IsUnderCheckFastImpl(side);
}

bool Rules::IsPassiveEndGame() const
{
    return m_board->GetCurrentPositionCount() >= 3
            || m_board->GetAfterLastCaptureOrPawnMoveHalfMoveCount() >= 50;
}

PositionList Rules::GetGuardedPositions(Figure *figure) const
{
    switch (figure->Type)
    {
        case Figure::Pawn:
            return GetPawnGuardedPositions(figure);
        case Figure::Knight:
            return GetKinghtGuardedPositions(figure);
        case Figure::Bishop:
            return GetBishopGuardedPositions(figure);
        case Figure::Rock:
            return GetRockGuardedPositions(figure);
        case Figure::Queen:
            return GetQueenGuardedPositions(figure);
        case Figure::King:
            return GetKingGuardedPositions(figure);
        default:
            throw Exception("Unknown figure type");
    }
}

FigurePosition Rules::ForwardFor(FigurePosition position, Figure::FigureSide side, int dx, int dy) const
{    
    if (side == Figure::Black)
        dy *= -1;

    return Shift(position, dx, dy);
}

int Rules::FirstHorizonatalYFor(Figure::FigureSide side) const
{
    if (side == Figure::White)
        return 1;
    else
        return 8;
}

int Rules::PawnPromotionYFor(Figure::FigureSide side) const
{
    if (side == Figure::White)
        return 8;
    else
        return 1;
}

int Rules::EnPassantPawnYFor(Figure::FigureSide side) const
{
    if (side == Figure::White)
        return 5;
    else
        return 4;
}

bool Rules::IsUnderCheckImpl(Figure::FigureSide side) const
{
    PositionList opponentGuarded = GetGuardedPositions(OpponentSide(side));

    Figure* king = m_board->KingAt(side);

    return opponentGuarded.contains(king->Position);
}

Figure* Rules::GetObstacleInDirection(FigurePosition position, Figure::FigureSide side, int xMult, int yMult) const
{
    if (xMult != -1 && xMult != 0 && xMult != 1 || yMult != -1 && yMult != 0 && yMult != 1)
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");

    for (int delta = 1; delta <= 7; ++delta)
    {
        FigurePosition p = ForwardFor(position, side, delta * xMult, delta * yMult);

        if (!IsInvalid(p))
        {
            Figure* figure = m_board->FigureAt(p);

            if (figure != NULL) // obstacle finded
                return figure;
        } else
        {
            break;
        }
    }

    return NULL;
}

bool Rules::IsUnderCheckFastImpl(Figure::FigureSide side) const
{
    Figure* king = m_board->KingAt(side);
    FigurePosition p = king->Position;
    Figure::FigureSide opponentSide = OpponentSide(side);

    // checks for knight threat
    PositionList knightCheckPositions;
    knightCheckPositions.reserve(8); // to speed up

    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, +2, +1));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, +1, +2));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, -1, +2));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, -2, +1));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, -2, -1));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, -1, -2));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, +1, -2));
    AppendIfValid(&knightCheckPositions, ForwardFor(p, side, +2, -1));

    foreach(FigurePosition position, knightCheckPositions)
    {
        Figure* figure = m_board->FigureAt(position);

        if (figure != NULL && figure->Type == Figure::Knight && figure->Side == opponentSide)
            return true;
    }

    // checks for long-range figures and pawns

    // check for enemy rocks and queen in non tilted directions
    FigureList nonTiltedDirectionObstacles;
    nonTiltedDirectionObstacles.reserve(4); // to speed up

    AppendIfNotNull(&nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, +1)); // forward direction
    AppendIfNotNull(&nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, -1)); // backward direction
    AppendIfNotNull(&nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, 0)); // x incresing direction
    AppendIfNotNull(&nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, 0)); // x decreasing direction

    foreach(Figure* figure, nonTiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == Figure::Queen || figure->Type == Figure::Rock)
            {
                return true;
            } else if (figure->Type == Figure::King)
            {
                int dx = abs(GetX(p) - GetX(figure->Position));
                int dy = abs(GetY(p) - GetY(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                    return true;
            }
        }
    }

    // check for enemy bishops, queen and pawns in tilted direction
    FigureList tiltedDirectionObstacles;
    tiltedDirectionObstacles.reserve(4); // to speed up

    AppendIfNotNull(&tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, +1)); // forward+ direction
    AppendIfNotNull(&tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, -1)); // backward+ direction
    AppendIfNotNull(&tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, +1)); // forward- direction
    AppendIfNotNull(&tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, -1)); // backward- direction

    //PositionList pawnThreatPotentialyPositions;
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, +1, +1));
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, -1, +1));

    foreach(Figure* figure, tiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == Figure::Queen || figure->Type == Figure::Bishop)
            {
                return true;
            }
            else if (figure->Type == Figure::Pawn)
            {
                bool isPawnForwardByOne = (GetY(figure->Position) - GetY(p)) == (side == Figure::White ? 1 : -1);

                if (isPawnForwardByOne && abs(GetX(figure->Position) - GetX(p)) == 1)
                {
                    return true;
                }
            } else if (figure->Type == Figure::King)
            {
                int dx = abs(GetX(p) - GetX(figure->Position));
                int dy = abs(GetY(p) - GetY(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                    return true;
            }
        }
    }

    return false;
}

Figure::FigureSide Rules::OpponentSide(Figure::FigureSide side) const
{
    if (side == Figure::White)
        return Figure::Black;
    else
        return Figure::White;
}

MoveList* Rules::DeleteMovesToCheck(MoveList *moves)
{
    MoveList::iterator it = moves->begin();

    while (it != moves->end())
    {
        Move move = *it;

        // temporary make move to verify that it doesn't causes check
        MakeMove(move);

        // when check after move
        if (IsUnderCheck(move.MovingFigure->Side))
        {
            it = moves->erase(it); // erase it
        } else
        {
            ++it;
        }

        // unmake temporary check
        UnMakeMove(move);
    }

    return moves;
}

PositionList Rules::_GetPossibleDestinations(Figure *figure) const
{
    PositionList list;

    switch (figure->Type)
    {
        case Figure::Pawn:
            list = _GetPawnPossibleDestinations(figure);
            break;
        case Figure::Knight:
            list = _GetKnightPossibleDestinations(figure);
            break;
        case Figure::Bishop:
            list = _GetBishopPossibleDestinations(figure);
            break;
        case Figure::Rock:
            list = _GetRockPossibleDestinations(figure);
            break;
        case Figure::Queen:
            list = _GetQueenPossibleDestinations(figure);
            break;
        case Figure::King:
            list = _GetKingPossibleDestinations(figure);
            break;
        default:
            throw Exception("Unknown figure type");
    }

    DeleteSelfCaptureDesination(&list, figure->Side);

    return list;
}

void Rules::DeleteSelfCaptureDesination(PositionList *destinations, Figure::FigureSide selfSide) const
{
    PositionList::iterator it = destinations->begin();
    while (it != destinations->end()) {        
        if (m_board->HasFigureAt(*it, selfSide)) // if same side figure here
            it = destinations->erase(it); // erase it
        else
            ++it;
    }
}

PositionList Rules::_GetPawnPossibleDestinations(Figure *figure) const
{
    PositionList destinations;

    // captures handling
    PositionList guarded = GetPawnGuardedPositions(figure);
    foreach (FigurePosition p, guarded)
    {
        Figure* pFigure = m_board->FigureAt(p);

        // pawn captures another figure
        if (pFigure != NULL && pFigure->Side == OpponentSide(figure->Side))
        {
            destinations.append(p);
        }
    }

    FigurePosition pShort = ForwardFor(figure->Position, figure->Side, 0, 1);
    if (!m_board->HasFigureAt(pShort))
    {
        destinations.append(pShort);

        if (figure->MovesCount == 0) // long first pawn turn handling
        {
            FigurePosition pLong = ForwardFor(figure->Position, figure->Side, 0, 2);

            if (!m_board->HasFigureAt(pLong))
            {
                destinations.append(pLong);
            }
        }
    }

    // en passant handling
    // can be only on 5 horizontal for white, and only on 4 for black side
    // also en passant can be only immediately after opponent crossing pawn guarded cell
    int enPassantY = EnPassantPawnYFor(figure->Side);
    int figureX = GetX(figure->Position);
    if (GetY(figure->Position) == enPassantY)
    {
        Move opponentLastMove = m_board->GetLastMove();

        if (opponentLastMove.Type == Move::LongPawn)
        {
            FigurePosition opponentStartPawnPosition = opponentLastMove.From;
            int opponentPawnX = GetX(opponentStartPawnPosition);
            Figure::FigureSide opponentSide = OpponentSide(figure->Side);

            if (opponentPawnX == figureX + 1 || opponentPawnX == figureX - 1)
            {
                FigurePosition enPassantDestination = ForwardFor(opponentStartPawnPosition, opponentSide, 0, 1);
                destinations.append(enPassantDestination);
            }
        }
    }

    return destinations;
}

PositionList Rules::_GetKnightPossibleDestinations(Figure *figure) const
{
    // yeah! is matches with knight guarded positions
    return GetKinghtGuardedPositions(figure);
}

PositionList Rules::_GetBishopPossibleDestinations(Figure *figure) const
{
    // also matches bishop guarded positions
    return GetBishopGuardedPositions(figure);
}

PositionList Rules::_GetRockPossibleDestinations(Figure *figure) const
{
    // matches rock guarded positions
    return GetRockGuardedPositions(figure);
}

PositionList Rules::_GetQueenPossibleDestinations(Figure *figure) const
{
    // matches queen guarded positions î_Î
    return GetQueenGuardedPositions(figure);
}

PositionList Rules::_GetKingPossibleDestinations(Figure *king) const
{
    PositionList destinations = GetKingGuardedPositions(king);
    PositionList opponentGuarded = GetGuardedPositions(OpponentSide(king->Side));
    FigurePosition currentKingPosition = king->Position;

    // check for short castling
    FigurePosition r1 = Shift(king->Position, 1, 0);
    FigurePosition r2 = Shift(king->Position, 2, 0);
    Figure* rRock = m_board->FigureAt(CreateFigurePosition(8,FirstHorizonatalYFor(king->Side)));

    if (king->MovesCount == 0
            && rRock != NULL
            && rRock->MovesCount == 0
            && !m_board->HasFigureAt(r1) && !m_board->HasFigureAt(r2)
            && !opponentGuarded.contains(currentKingPosition) // castling from check is forbidden
            && !opponentGuarded.contains(r1)   // castling through guarded cells is forbidden
            && !opponentGuarded.contains(r2))  // also

    {
            destinations.append(r2);
    }

    // long castling checks
    FigurePosition l1 = Shift(king->Position, -1, 0);
    FigurePosition l2 = Shift(king->Position, -2, 0);
    FigurePosition l3 = Shift(king->Position, -3, 0);
    Figure* lRock = m_board->FigureAt(CreateFigurePosition(1, FirstHorizonatalYFor(king->Side)));

    if (king->MovesCount == 0
            && lRock != NULL
            && lRock->MovesCount == 0
            && !m_board->HasFigureAt(l1) && !m_board->HasFigureAt(l2) && !m_board->HasFigureAt(l3)
            && !opponentGuarded.contains(currentKingPosition) // castling from check is forbidden
            && !opponentGuarded.contains(l1)   // castling through guarded cells is forbidden
            && !opponentGuarded.contains(l2)   // also
            && !opponentGuarded.contains(l3))  // also

    {
            destinations.append(l2);
    }

    return destinations;
}

Move Rules::CreateMove(FigurePosition from, FigurePosition to)
{
    Move::MoveType moveType;
    Figure* captured = m_board->FigureAt(to);
    Figure* figure = m_board->FigureAt(from);
    Figure::FigureType figureType = figure->Type;

    // long king step -> castling
    if (figureType == Figure::King && abs(GetX(to) - GetX(from)) == 2)
    {
        if (GetX(to) > GetX(from))
            moveType = Move::ShortCastling;
        else
            moveType = Move::LongCastling;
    }
    // long pawn turn handling
    else if (figureType == Figure::Pawn && abs(GetY(to) - GetY(from)) == 2)
    {
        moveType = Move::LongPawn;
    }
    // pawn's position x changing and target cell is empty -> en passant
    else if (figureType == Figure::Pawn && !m_board->HasFigureAt(to) && GetX(to) != GetX(from))
    {
        moveType = Move::EnPassant;
        captured = m_board->FigureAt(ForwardFor(to, figure->Side, 0, -1));
    }
    // pawn promote
    else if (figureType == Figure::Pawn && GetY(to) == PawnPromotionYFor(figure->Side))
    {
        moveType = Move::PawnPromotion;
    } else // only Normal and Capture turns
    {
        if (captured != NULL)
        {
            moveType = Move::Capture;
        } else
        {
            moveType = Move::Normal;
        }
    }

    return Move(moveType, from, to, figure, captured);
}

PositionList Rules::GetGuardedPositions(Figure::FigureSide side) const
{
    FigureList figures = m_board->FiguresAt(side);

    PositionList guarded;

    foreach(Figure* figure, figures)
    {
        PositionList currentFigureGuarded = GetGuardedPositions(figure);

        AppendIfNotExists(&guarded, currentFigureGuarded);
    }

    return guarded;
}

MoveList Rules::GetPossibleMoves(Figure::FigureSide side)
{
    FigureList figures = m_board->FiguresAt(side);
    MoveList moves;

    if (m_board->GetTurningSide() != side)
        return moves;

    foreach(Figure* figure, figures)
    {
        PositionList currentFigurePossibleDestinations = _GetPossibleDestinations(figure);

        foreach(FigurePosition curFigurePossiblePosition, currentFigurePossibleDestinations)
        {
            Move currentMove = CreateMove(figure->Position, curFigurePossiblePosition);

            moves.append(currentMove);
        }
    }

    DeleteMovesToCheck(&moves);

    return moves;
}

MoveList Rules::GetPossibleMoves(Figure *figure)
{
    MoveList moves = GetPossibleMoves(figure->Side);

    MoveList::iterator it = moves.begin();

    while (it != moves.end())
    {
        if ((*it).MovingFigure != figure)
            it = moves.erase(it);
        else
            ++it;
    }

    return moves;
}

PositionList Rules::GetPossibleDestinations(Figure *figure)
{
    MoveList moves = GetPossibleMoves(figure);
    PositionList destinations;

    foreach(Move move, moves)
    {
        destinations.append(move.To);
    }

    return destinations;
}

void Rules::MakeMove(Move move)
{
    // turn transition
    m_board->TurnTransition();

    // handle move count
    move.MovingFigure->IncreaseMovesCounter();

    // capture enemy figure if needed
    if (move.CapturedFigure != NULL)
    {
        m_board->KillFigure(move.CapturedFigure);
    }

    // moving own figure
    m_board->MoveFigure(move.MovingFigure, move.To);

    // if castling move accordingly rock
    if (move.IsCastling())
    {
        int y = GetY(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == Move::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(CreateFigurePosition(rockBeforeCastlingX, y));

        // move rock
        m_board->MoveFigure(rock, CreateFigurePosition(rockAfterCastringX, y));
    } else if (move.Type == Move::PawnPromotion)
    {
        m_board->PromotePawn(move.MovingFigure, Figure::Queen);
    }

    // add to history
    m_board->PushToHistory(move);

    // position counting to detect draw game situations
    m_board->IncreaseCurrentPositionCount();
}

void Rules::MakeMove(FigurePosition from, FigurePosition to)
{
    Figure* f = m_board->FigureAt(from);

    if (f != NULL)
    {
        MoveList possible = GetPossibleMoves(f);
        bool isValidMove = false;

        foreach(Move move, possible)
        {
            if (move.From == from && move.To == to)
            {
                MakeMove(move);
                isValidMove = true;
                break;
            }
        }

        if (!isValidMove)
        {
            // if we get were then move not finded
            throw Exception("Incorrect move");
        }
    } else
    {
        throw Exception("Invalid source position");
    }
}

void Rules::UnMakeMove(Move move)
{
    // position counting to detect draw game situations
    m_board->DecreaseCurrentPositionCount();

    // turn transition
    m_board->TurnTransition();

    // remove from history
    m_board->PopHistory();

    // handle move count
    move.MovingFigure->DecreaseMovesCounter();

    // unmoving own figure
    m_board->MoveFigure(move.MovingFigure, move.From);

    // uncapture enemy figure if needed
    if (move.CapturedFigure != NULL)
    {
        m_board->ResurrectFigure(move.CapturedFigure);
    }    

    // if castling move accordingly rock
    if (move.IsCastling())
    {
        int y = GetY(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == Move::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(CreateFigurePosition(rockAfterCastringX, y));

        // move rock
        m_board->MoveFigure(rock, CreateFigurePosition(rockBeforeCastlingX, y));
    } else if (move.Type == Move::PawnPromotion)
    {
        m_board->UnpromotePawn(move.MovingFigure);
    }
}
