#include <algorithm>

#include "rules.h"
#include "typedefs.h"
#include "exception.h"
#include "helpers.h"
#include "bitboardhelper.h"

Rules::Rules(Board *board)
    :m_board(board)
{
}

FigureSide Rules::OpponentSide(FigureSide side) const
{
    return side == FigureSide::White ? FigureSide::Black : FigureSide::White;
}

int Rules::PawnPromotionYFor(FigureSide side) const
{
    return side == FigureSide::White ? 8 : 1;
}

int Rules::PawnHomeYFor(FigureSide side) const
{
    return side == FigureSide::White ? 2 : 7;
}

int Rules::EnPassantPawnYFor(FigureSide side) const
{
    return side == FigureSide::White ? 5 : 4;
}

int Rules::FirstHorizonatalYFor(FigureSide side) const
{
    return side == FigureSide::White ? 1 : 8;
}

BITBOARD Rules::GetOnLinePositions(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit = 7) const
{
#ifdef QT_DEBUG
    if ((xMult != -1 && xMult != 0 && xMult != 1)
            || (yMult != -1 && yMult != 0 && yMult != 1))
    {
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");
    }
#endif

    BITBOARD bitboard = BITBOARD_EMPTY;

    if (side == FigureSide::Black)
    {
        yMult *= -1;
    }

    for (int delta = 1; delta <= lenLimit; ++delta)
    {
        POSITION p = PositionHelper::Shift(position, delta * xMult, delta * yMult);

        if (PositionHelper::IsInvalid(p))
        {
            break;
        }

        bitboard = BitboardHelper::AddPosition(bitboard, p);

        if (m_board->FigureAt(p) != NULL) // obstacle -> stop searching
        {
            break;
        }
    }

    return bitboard;
}

Figure* Rules::GetObstacleInDirection(POSITION position, FigureSide side, int xMult, int yMult) const
{
#ifdef QT_DEBUG
    if ((xMult != -1 && xMult != 0 && xMult != 1)
            || (yMult != -1 && yMult != 0 && yMult != 1))
    {
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");
    }
#endif

    if (side == FigureSide::Black)
    {
        yMult *= -1;
    }

    for (int delta = 1; delta <= 7; ++delta)
    {
        POSITION p = PositionHelper::Shift(position, delta * xMult, delta * yMult);

        if (PositionHelper::IsInvalid(p))
        {
            break;
        }

        Figure* figure = m_board->FigureAt(p);

        if (figure != NULL) // obstacle -> return it
        {
            return figure;
        }
    }

    return NULL; // no obstacles finded in specified direction
}

BITBOARD Rules::GetPawnGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, ForwardFor(p, side, -1, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, ForwardFor(p, side, +1, +1));

    return bitboard;
}

BITBOARD Rules::GetKinghtGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +2, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, +2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, +2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -2, +1));

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -2, -1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, -2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, -2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +2, -1));

    return bitboard;
}

BITBOARD Rules::GetBishopGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    //bitboard = BitBoardHelper::AddPositionWhenValid(bitboard, )
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, -1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, -1, 7));

    return bitboard;
}

BITBOARD Rules::GetRockGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, 0, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, 0, -1, 7));

    return bitboard;
}

BITBOARD Rules::GetQueenGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, 0, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, 0, -1, 7));

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, +1, -1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions(p, side, -1, -1, 7));

    return bitboard;
}

BITBOARD Rules::GetKingGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, -1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, -1));

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, 0));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, 0));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, 0, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, 0, -1));

    return bitboard;
}

bool Rules::IsUnderCheck(FigureSide side) const
{
    //return IsUnderCheckImpl(side);
    return IsUnderCheckFastImpl(side);
}

bool Rules::IsPassiveEndGame() const
{
    // position repeadted three times
    if (m_board->GetCurrentPositionCount() >= 3)
        return true;

    // performance optimization
    if (m_board->GetPlyCount() >= 50)
    {
        // if 50 or more plys after last pawn move or capture passed
        if (m_board->GetAfterLastCaptureOrPawnMoveHalfMoveCount() >= 50)
        {
            return true;
        }
    }

    return false;
}

BITBOARD Rules::GetGuardedPositions(Figure *figure) const
{
    switch (figure->Type)
    {
        case FigureType::Pawn:      return GetPawnGuardedPositions(figure);
        case FigureType::Knight:    return GetKinghtGuardedPositions(figure);
        case FigureType::Bishop:    return GetBishopGuardedPositions(figure);
        case FigureType::Rock:      return GetRockGuardedPositions(figure);
        case FigureType::Queen:     return GetQueenGuardedPositions(figure);
        case FigureType::King:      return GetKingGuardedPositions(figure);

        default: throw Exception("Unknown figure type");
    }
}

POSITION Rules::ForwardFor(POSITION position, FigureSide side, int dx, int dy) const
{    
    return side == FigureSide::White ?
                PositionHelper::Shift(position, dx, dy)
              : PositionHelper::Shift(position, dx, -dy);
}

bool Rules::IsUnderCheckImpl(FigureSide side) const
{
    BITBOARD opponentGuarded = GetGuardedPositions(OpponentSide(side));

    Figure* king = m_board->KingAt(side);

    return BitboardHelper::Contains(opponentGuarded, king->Position);
}

bool Rules::IsUnderCheckFastImpl(FigureSide side) const
{
    Figure* king = m_board->KingAt(side);
    POSITION p = king->Position;
    FigureSide opponentSide = OpponentSide(side);

    // checks for knight threat
    QVarLengthArray<POSITION, 8> knightCheckPositions;

    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, +2, +1));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, +1, +2));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, -1, +2));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, -2, +1));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, -2, -1));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, -1, -2));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, +1, -2));
    Helpers::AppendIfValid(knightCheckPositions, PositionHelper::Shift(p, +2, -1));

    foreach(POSITION position, knightCheckPositions)
    {
        Figure* figure = m_board->FigureAt(position);

        if (figure != NULL && figure->Type == FigureType::Knight && figure->Side == opponentSide)
        {
            return true;
        }
    }

    // checks for long-range figures and pawns

    // check for enemy rocks and queen in non tilted directions
    QVarLengthArray<Figure*, 4> nonTiltedDirectionObstacles;

    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, +1)); // forward direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, -1)); // backward direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, 0)); // x incresing direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, 0)); // x decreasing direction

    foreach(Figure* figure, nonTiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == FigureType::Queen || figure->Type == FigureType::Rock)
            {
                return true;
            } else if (figure->Type == FigureType::King)
            {
                int dx = abs(PositionHelper::X(p) - PositionHelper::X(figure->Position));
                int dy = abs(PositionHelper::Y(p) - PositionHelper::Y(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                {
                    return true;
                }
            }
        }
    }

    // check for enemy bishops, queen and pawns in tilted direction
    QVarLengthArray<Figure*, 4> tiltedDirectionObstacles;

    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, +1)); // forward+ direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, -1)); // backward+ direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, +1)); // forward- direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, -1)); // backward- direction

    //PositionList pawnThreatPotentialyPositions;
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, +1, +1));
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, -1, +1));

    foreach(Figure* figure, tiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == FigureType::Queen || figure->Type == FigureType::Bishop)
            {
                return true;
            }
            else if (figure->Type == FigureType::Pawn)
            {
                bool isPawnForwardByOne = (PositionHelper::Y(figure->Position) - PositionHelper::Y(p)) == (side == FigureSide::White ? 1 : -1);

                if (isPawnForwardByOne && abs(PositionHelper::X(figure->Position) - PositionHelper::X(p)) == 1)
                {
                    return true;
                }
            } else if (figure->Type == FigureType::King)
            {
                int dx = abs(PositionHelper::X(p) - PositionHelper::X(figure->Position));
                int dy = abs(PositionHelper::Y(p) - PositionHelper::Y(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void Rules::DeleteMovesToCheck(MoveCollection& moves)
{
    MoveCollection::iterator it = moves.begin();

    while (it != moves.end())
    {
        Move move = *it;

        // temporary make move to verify that it doesn't causes check
        MakeMove(move);

        // when check after move
        if (IsUnderCheck(move.MovingFigure->Side))
        {
            it = moves.erase(it); // erase it
        } else
        {
            ++it;
        }

        // unmake temporary move
        UnMakeMove(move);
    }
}

BITBOARD Rules::_GetPossibleDestinations(Figure *figure) const
{
    BITBOARD dests = BITBOARD_EMPTY;

    switch (figure->Type)
    {
        case FigureType::Pawn:      dests = _GetPawnPossibleDestinations(figure);    break;
        case FigureType::Knight:    dests = _GetKnightPossibleDestinations(figure);  break;
        case FigureType::Bishop:    dests = _GetBishopPossibleDestinations(figure);  break;
        case FigureType::Rock:      dests = _GetRockPossibleDestinations(figure);    break;
        case FigureType::Queen:     dests = _GetQueenPossibleDestinations(figure);   break;
        case FigureType::King:      dests = _GetKingPossibleDestinations(figure);   break;

        default: throw Exception("Unknown figure type");
    }

    // remove self-capture destinations
    return BitboardHelper::Substract(dests, m_board->GetBitboardFor(figure->Side));
}

BITBOARD Rules::_GetPawnPossibleDestinations(Figure *figure) const
{
    BITBOARD destinations = BITBOARD_EMPTY;

    // captures handling
    POSITION pCaptureLeft = ForwardFor(figure->Position, figure->Side, -1, +1);
    POSITION pCaptureRight = ForwardFor(figure->Position, figure->Side, +1, +1);

    if (!PositionHelper::IsInvalid(pCaptureLeft)
            && m_board->HasFigureAt(pCaptureLeft, OpponentSide(figure->Side)))
    {
        destinations = BitboardHelper::AddPosition(destinations, pCaptureLeft);
    }
    if (!PositionHelper::IsInvalid(pCaptureRight)
            && m_board->HasFigureAt(pCaptureRight, OpponentSide(figure->Side)))
    {
        destinations = BitboardHelper::AddPosition(destinations, pCaptureRight);
    }

    // short and long moves
    POSITION pShort = ForwardFor(figure->Position, figure->Side, 0, 1);
    if (!m_board->HasFigureAt(pShort))
    {
        destinations = BitboardHelper::AddPosition(destinations, pShort);

        //if (figure->MovesCount == 0) // long first pawn turn handling
        if (PositionHelper::Y(figure->Position) == PawnHomeYFor(figure->Side))
        {
            POSITION pLong = ForwardFor(figure->Position, figure->Side, 0, 2);

            if (!m_board->HasFigureAt(pLong))
            {
                destinations = BitboardHelper::AddPosition(destinations, pLong);
            }
        }
    }

    // en passant handling
    // can be only on 5 horizontal for white, and only on 4 for black side
    // also en passant can be only immediately after opponent crossing pawn guarded cell
    int enPassantY = EnPassantPawnYFor(figure->Side);
    int figureX = PositionHelper::X(figure->Position);
    if (PositionHelper::Y(figure->Position) == enPassantY)
    {
        // this situation is possible when board setted from FEN explicitly
        // hence we should get last move when history is empty
        if (!m_board->IsHistoryEmpty())
        {
            Move opponentLastMove = m_board->GetLastMove();

            if (opponentLastMove.Type == MoveType::LongPawn)
            {
                POSITION opponentStartPawnPosition = opponentLastMove.From;
                int opponentPawnX = PositionHelper::X(opponentStartPawnPosition);
                FigureSide opponentSide = OpponentSide(figure->Side);

                if (opponentPawnX == figureX + 1 || opponentPawnX == figureX - 1)
                {
                    POSITION enPassantDestination = ForwardFor(opponentStartPawnPosition, opponentSide, 0, 1);
                    destinations = BitboardHelper::AddPosition(destinations, enPassantDestination);
                }
            }
        }
    }

    return destinations;
}

BITBOARD Rules::_GetKnightPossibleDestinations(Figure *figure) const
{
    // yeah! is matches with knight guarded positions
    return GetKinghtGuardedPositions(figure);
}

BITBOARD Rules::_GetBishopPossibleDestinations(Figure *figure) const
{
    // also matches bishop guarded positions
    return GetBishopGuardedPositions(figure);
}

BITBOARD Rules::_GetRockPossibleDestinations(Figure *figure) const
{
    // matches rock guarded positions
    return GetRockGuardedPositions(figure);
}

BITBOARD Rules::_GetQueenPossibleDestinations(Figure *figure) const
{
    // matches queen guarded positions о_О
    return GetQueenGuardedPositions(figure);
}

BITBOARD Rules::_GetKingPossibleDestinations(Figure *king) const
{
    BITBOARD destinations = GetKingGuardedPositions(king);

    POSITION kPosition = king->Position;
    FigureSide kSide = king->Side;

    // available castling check
    if (king->MovesCount == 0)
    {
        bool guardedPositionsCalculated = false;
        BITBOARD opponentGuarded; // it calculation is expensive, so we trying to caluclate it when it really needed

        // check for short castling
        POSITION r1 = PositionHelper::Shift(king->Position, 1, 0);
        POSITION r2 = PositionHelper::Shift(king->Position, 2, 0);
        Figure* rRock = m_board->FigureAt(PositionHelper::Create(8, FirstHorizonatalYFor(kSide)));

        if (rRock != NULL && rRock->MovesCount == 0 && !m_board->HasFigureAt(r1) && !m_board->HasFigureAt(r2)) // these checks is cheep
        {
            // short castling potentially possible, calcuate opponent's guarded positions
            opponentGuarded = GetGuardedPositions(OpponentSide(kSide));
            guardedPositionsCalculated = true;

            if (!BitboardHelper::Contains(opponentGuarded, kPosition) // castling from check is forbidden
                && !BitboardHelper::Contains(opponentGuarded, r1)   // castling through guarded cells is forbidden
                && !BitboardHelper::Contains(opponentGuarded, r2))  // also
            {
                destinations = BitboardHelper::AddPosition(destinations, r2);
            }
        }

        // long castling checks
        POSITION l1 = PositionHelper::Shift(king->Position, -1, 0);
        POSITION l2 = PositionHelper::Shift(king->Position, -2, 0);
        POSITION l3 = PositionHelper::Shift(king->Position, -3, 0);
        Figure* lRock = m_board->FigureAt(PositionHelper::Create(1, FirstHorizonatalYFor(king->Side)));

        if (lRock != NULL && lRock->MovesCount == 0 && !m_board->HasFigureAt(l1) && !m_board->HasFigureAt(l2) && !m_board->HasFigureAt(l3)) // cheap checks
        {
            // long castling potentially possible, calcuate opponent's guarded positions if it have not already done
            if (!guardedPositionsCalculated)
                opponentGuarded = GetGuardedPositions(OpponentSide(kSide));

            if (!BitboardHelper::Contains(opponentGuarded, kPosition) // castling from check is forbidden
                && !BitboardHelper::Contains(opponentGuarded, l1)   // castling through guarded cells is forbidden
                && !BitboardHelper::Contains(opponentGuarded, l2)   // also
                && !BitboardHelper::Contains(opponentGuarded, l3))  // also
            {
                destinations = BitboardHelper::AddPosition(destinations, l2);
            }
        }
    }

    return destinations;
}

MoveCollection Rules::CreateMoves(POSITION from, POSITION to)
{
    // collection instead of single instance
    // is used for only reason: when pawn promoted
    // there are several moves with same source and destination cells
    MoveCollection moves;

    MoveType moveType;
    Figure* captured = m_board->FigureAt(to);
    Figure* figure = m_board->FigureAt(from);
    FigureType figureType = figure->Type;

    // pawn promotion is special case - here there are several moves possbile
    if (figureType == FigureType::Pawn && PositionHelper::Y(to) == PawnPromotionYFor(figure->Side))
    {
        moveType = MoveType::PawnPromotion;

        Move pawnPromotionToQueen(moveType, from, to, figure, captured);
        pawnPromotionToQueen.PromotedTo = FigureType::Queen;

        Move pawnPromotionToKnight(moveType, from, to, figure, captured);
        pawnPromotionToKnight.PromotedTo = FigureType::Knight;

        Move pawnPromotionToRock(moveType, from, to, figure, captured);
        pawnPromotionToRock.PromotedTo = FigureType::Rock;

        Move pawnPromotionToBishop(moveType, from, to, figure, captured);
        pawnPromotionToBishop.PromotedTo = FigureType::Bishop;

        moves << pawnPromotionToQueen << pawnPromotionToKnight << pawnPromotionToRock << pawnPromotionToBishop;
    } else // single move generated in this section
    {
        // long king step -> castling
        if (figureType == FigureType::King && abs(PositionHelper::X(to) - PositionHelper::X(from)) == 2)
        {
            if (PositionHelper::X(to) > PositionHelper::X(from))
                moveType = MoveType::ShortCastling;
            else
                moveType = MoveType::LongCastling;
        }
        // long pawn turn handling
        else if (figureType == FigureType::Pawn && abs(PositionHelper::Y(to) - PositionHelper::Y(from)) == 2)
        {
            moveType = MoveType::LongPawn;
        }
        // pawn's position x changing and target cell is empty -> en passant
        else if (figureType == FigureType::Pawn && !m_board->HasFigureAt(to) && PositionHelper::X(to) != PositionHelper::X(from))
        {
            moveType = MoveType::EnPassant;
            captured = m_board->FigureAt(ForwardFor(to, figure->Side, 0, -1));
        }
        else // other, not special moves
        {
            moveType = MoveType::Normal;
        }

        moves << Move(moveType, from, to, figure, captured);
    }

    return moves;
}

BITBOARD Rules::GetGuardedPositions(FigureSide side) const
{
    FigureList figures = m_board->FiguresAt(side);

    BITBOARD bitboard = BITBOARD_EMPTY;

    foreach(Figure* figure, figures)
    {
        bitboard = BitboardHelper::Union(bitboard, GetGuardedPositions(figure));
    }

    return bitboard;
}

MoveCollection Rules::GetPossibleMoves(FigureSide side, bool generateLegalOnly)
{
    FigureList figures = m_board->FiguresAt(side);
    MoveCollection moves;

    if (m_board->GetTurningSide() != side)
    {
        return moves;
    }

    foreach(Figure* figure, figures)
    {
        PositionCollection currentFigurePossibleDestinations = BitboardHelper::GetPositions(_GetPossibleDestinations(figure));        

        foreach(POSITION curFigurePossiblePosition, currentFigurePossibleDestinations)
        {            
            MoveCollection currentMoves = CreateMoves(figure->Position, curFigurePossiblePosition);

            foreach(const Move& currentMove, currentMoves)
            {
                moves.append(currentMove);
            }
        }
    }

    if (generateLegalOnly)
    {
        DeleteMovesToCheck(moves);
    }

    return moves;
}

void Rules::MakeMove(Move move)
{
    // turn transition
    m_board->TurnTransition();

    // handle move count
    move.MovingFigure->MovesCount++;

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
        int y = PositionHelper::Y(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == MoveType::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(PositionHelper::Create(rockBeforeCastlingX, y));

        // move rock
        m_board->MoveFigure(rock, PositionHelper::Create(rockAfterCastringX, y));
    } else if (move.Type == MoveType::PawnPromotion)
    {
        //m_board->PromotePawn(move.MovingFigure, FigureType::Queen);
        m_board->PromotePawn(move.MovingFigure, move.PromotedTo);
    }

    // add to history
    m_board->PushToHistory(move);

    // position counting to detect draw game situations
    m_board->IncreaseCurrentPositionCount();
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
    move.MovingFigure->MovesCount--;

    // unmoving own figure
    m_board->MoveFigure(move.MovingFigure, move.From);

    // uncapture enemy figure if needed
    if (move.CapturedFigure != NULL)
    {
        m_board->ResurrectFigure(move.CapturedFigure);
    }    

    // if castling move rock back
    if (move.IsCastling())
    {
        int y = PositionHelper::Y(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == MoveType::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(PositionHelper::Create(rockAfterCastringX, y));

        // move rock
        m_board->MoveFigure(rock, PositionHelper::Create(rockBeforeCastlingX, y));
    } else if (move.Type == MoveType::PawnPromotion)
    {
        m_board->UnpromotePawn(move.MovingFigure);
    }
}

MoveCollection Rules::GetPossibleMoves(Figure *figure)
{
    MoveCollection moves = GetPossibleMoves(figure->Side);

    MoveCollection::iterator it = moves.begin();
    while (it != moves.end())
    {
        if ((*it).MovingFigure != figure)
        {
            it = moves.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return moves;
}

PositionList Rules::GetPossibleDestinations(Figure *figure)
{
    MoveCollection moves = GetPossibleMoves(figure);
    PositionList destinations;

    foreach(const Move& move, moves)
    {
        destinations.append(move.To);
    }

    return destinations;
}

void Rules::MakeMove(POSITION from, POSITION to, FigureType promotedTo)
{
    foreach(const Move& move, FindMoves(from, to))
    {
        if (move.From == from && move.To == to
                && (move.PromotedTo == promotedTo || move.Type != MoveType::PawnPromotion))
        {
            MakeMove(move);
            return;
        }
    }

    // if we get here because move was not found
    throw Exception(QString("Move from %1 to %2 is incorrect.").arg(PositionHelper::ToString(from), PositionHelper::ToString(to)).toStdString());
}

QList<Move> Rules::FindMoves(POSITION from, POSITION to)
{
    QList<Move> finded;

    Figure* f = m_board->FigureAt(from);
    if (f != NULL)
    {
        MoveCollection possible = GetPossibleMoves(f);
        foreach(const Move& move, possible)
        {
            if (move.From == from && move.To == to)
            {
                finded.append(move);
            }
        }
    } else
    {
        throw Exception("No figures was found in source position.");
    }

    return finded;
}
