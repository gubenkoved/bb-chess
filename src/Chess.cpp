// Navigation pane project template
#include "Chess.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/system/SystemToast>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/SystemUiResult>
#include <bb/system/SystemUiButton>


#include "boardserializer.h"
#include "fen.h"
#include "puzzle.h"

using namespace bb::cascades;
using namespace bb::system;

QDataStream &operator<<(QDataStream &out, const GameSave &obj)
{
     out << obj.BoardString << obj.IsPVC << obj.Depth << obj.UserSide;
     return out;
}
QDataStream &operator>>(QDataStream &in, GameSave &obj)
{
	in >> obj.BoardString;
	in >> obj.IsPVC;
	in >> obj.Depth;
	in >> obj.UserSide;

    return in;
}

Chess::Chess(bb::cascades::Application *app)
: QObject(app)
{
	qRegisterMetaType<GameSave>("GameSave");
	qRegisterMetaTypeStreamOperators<GameSave>("GameSave");

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("app", this);

    // create root object for the UI
    m_navigationPane = qml->createRootObject<NavigationPane>();

    // set created root object as a scene
    app->setScene(m_navigationPane);

    Initialize();
}

void Chess::Initialize()
{
	qDebug() << "Chess::Initialize";

	QmlDocument *gameQml = QmlDocument::create("asset:///game.qml").parent(this);
	gameQml->setContextProperty("app", this);
	gameQml->setContextProperty("navigationPane", m_navigationPane);

	m_gamePage = gameQml->createRootObject<Page>();

	m_userSide = FigureSide::White;
	m_selectedFigure = NULL;
	m_boardFlipped = false;

	m_aiThinking = false;
	emit aiThinkingChanged();

	m_boardContainer = m_gamePage->findChild<Container*>("board");
	m_topKilledDockContainer = m_gamePage->findChild<Container*>("topKilledPiecesDock");
	m_bottomKilledDockContainer = m_gamePage->findChild<Container*>("bottomKilledPiecesDock");
	m_turnInfoLabel = m_gamePage->findChild<Label*>("turnInfoLabel");

	m_board = new Board();
	m_board->SetupStartPosition();
	m_rules = new Rules(m_board);
	m_asyncAI = new AsyncAI(m_board);

	connect(m_asyncAI, SIGNAL(BestMoveFinded(QString)), this, SLOT(HandleFindedBestMove(QString)), Qt::QueuedConnection);

	Draw();
}

void Chess::StartPVC(int depth)
{
	//ShowNotification("test", "hi");

	m_isPVC = true;
	m_depth = depth;

	NewGame();
	Draw();
	UpdateTurnNotification();

	m_navigationPane->push(m_gamePage);
}

void Chess::StartPVP()
{
	m_isPVC = false;

	NewGame();
	Draw();
	UpdateTurnNotification();

	m_navigationPane->push(m_gamePage);
}

void Chess::Continue()
{
	QVariant saveQVar = m_settings.value("save");

	if (!saveQVar.isNull())
	{
		GameSave save = saveQVar.value<GameSave>();

		delete m_board;

		m_board = new Board(BoardSerializer::Load(save.BoardString));
		m_isPVC = save.IsPVC;
		m_depth = save.Depth;
		m_userSide = (FigureSide)save.UserSide;

		foreach(Control* c, m_figureToUIPieceMapping.values())
		{
			m_boardContainer->remove(c);
			m_topKilledDockContainer->remove(c);
			m_bottomKilledDockContainer->remove(c);
		}
		m_figureToUIPieceMapping.clear();

		UpdateTurnNotification();

		ResetCellsHighlighting(true);

		Draw();

		m_navigationPane->push(m_gamePage);
	} else
	{
		ShowToast("Save slot is empty");
	}
}

void Chess::Save()
{
	if (m_isPVC && m_aiThinking)
	{
		ShowToast(tr("Please, wait while AI is thinking"));
	} else
	{

		if (!m_settings.value("save").isNull())
		{
			if (GetConfirmation(tr("Existing save will be overriden")) == false)
				return;
		}

		GameSave save;

		save.BoardString = BoardSerializer::Save(*m_board);
		save.Depth = m_depth;
		save.IsPVC = m_isPVC;
		save.UserSide = (int)m_userSide;

		m_settings.setValue("save", qVariantFromValue(save));

		ShowToast("Saved");
	}
}


void Chess::HandleFindedBestMove(QString bestMoveString)
{
	qDebug() << "Chess::HandleFindedBestMove BEGIN";

	QStringList tmp = bestMoveString.split(' ');

	POSITION from = PositionHelper::FromString(tmp[0].toStdString());
	POSITION to = PositionHelper::FromString(tmp[1].toStdString());

	m_rules->MakeMove(from, to);

	ResetCellsHighlighting(true);

	HighlightMove(from, to);

	Draw();

	CheckForEndGame();

	m_aiThinking = false;
	emit aiThinkingChanged();

	UpdateTurnNotification();

	qDebug() << "Chess::HandleFindedBestMove END";
}

void Chess::UpdateTurnNotification()
{
	if (!m_isPVC)
	{
		if (m_board->GetTurningSide() == FigureSide::White)
			m_turnInfoLabel->setText(tr("White's turn"));
		else
			m_turnInfoLabel->setText(tr("Black's turn"));
	} else
	{
		if (m_aiThinking)
			m_turnInfoLabel->setText(tr("Blackberry's turn"));
		else
			m_turnInfoLabel->setText(tr("Your turn"));
	}

}

void Chess::ResetCellsHighlighting(bool resetPrevMoveHighlight)
{
	qDebug() << "Chess::ResetCellsHighlighting";

	for (int x = 1; x <= 8; ++x)
	{
		for (int y = 1; y <= 8; ++y)
		{
			Control* cell = FindCell(x, y);

			if (resetPrevMoveHighlight)
			{
				cell->setProperty("isHighlightAsPreviousMove", false);
			}

			cell->setProperty("isHighlightAsPossibleDestination", false);
			cell->setProperty("isHighlightAsSelected", false);
		}
	}
}

void Chess::HandleTap(int x, int y)
{
	qDebug() << "Chess::HandleTap";

	POSITION p = PositionHelper::Create(x, y);

	if (m_selectedFigure == NULL)
	{
		ResetCellsHighlighting(false);

		m_selectedFigure = m_board->FigureAt(p);

		if (m_selectedFigure != NULL) // figure now selected
		{
			FindCell(x, y)->setProperty("isHighlightAsSelected", true);

			// show possible destination for it
			PositionList dests = m_rules->GetPossibleDestinations(m_selectedFigure);

			foreach(POSITION pos, dests)
			{
				Control* cell = FindCell(PositionHelper::X(pos), PositionHelper::Y(pos));

				qDebug() << "cell:" << cell;

				cell->setProperty("isHighlightAsPossibleDestination", true);
			}
		}
	} else
	{
		bool isPossibleStep = m_rules->GetPossibleDestinations(m_selectedFigure).contains(p);

		if (!m_gameEnded
				&& isPossibleStep
				&& ((m_isPVC && m_board->GetTurningSide() == m_userSide) ||!m_isPVC))
		{
			ResetCellsHighlighting(true);

			// do step
			m_rules->MakeMove(m_selectedFigure->Position, p);

			Draw();

			bool gameOver = CheckForEndGame();

			if (!gameOver && m_isPVC)
			{
				qDebug() << "AI thinking start";

				m_aiThinking = true;
				emit aiThinkingChanged();

				m_asyncAI->StartBestMoveSearch(m_board->GetTurningSide(), m_depth);
			}

			UpdateTurnNotification();
		} else
		{
			ResetCellsHighlighting(false);

			m_selectedFigure = m_board->FigureAt(p);

			if (m_selectedFigure != NULL)
			{
				FindCell(PositionHelper::X(p), PositionHelper::Y(p))->setProperty("isHighlightAsSelected", true);

				// show possible destination for it
				PositionList dests = m_rules->GetPossibleDestinations(m_selectedFigure);

				foreach(POSITION pos, dests)
				{
					Control* cell = FindCell(PositionHelper::X(pos), PositionHelper::Y(pos));

					qDebug() << "cell:" << cell;

					cell->setProperty("isHighlightAsPossibleDestination", true);
				}
			}
		}
	}
}

bool Chess::CheckForEndGame()
{
	qDebug() << "Chess::CheckForEndGame";

	if (m_rules->GetPossibleMoves(m_board->GetTurningSide()).count() == 0)
	{
		if (m_rules->IsUnderCheck(m_board->GetTurningSide()))
		{
			if (m_isPVC)
			{
				if (m_board->GetTurningSide() == m_userSide)
				{
					qDebug() << "User lose";
					ShowEndGameNotification(tr("You lose"));
					//ShowToast("Sorry, you lose");
				} else
				{
					qDebug() << "User win";
					ShowEndGameNotification(tr("You win!"));
					//ShowToast("You win!");
				}
			} else
			{
				if (m_board->GetTurningSide() == FigureSide::White)
				{
					qDebug() << "Black win";
					ShowEndGameNotification(tr("Black's win!"));
					//ShowToast("Black's win!");
				} else
				{
					qDebug() << "White win";
					ShowEndGameNotification(tr("White's win!"));
					//ShowToast("White's win!");
				}
			}
		} else
		{
			qDebug() << "Draw game";
			//ShowToast("Draw game");
			ShowEndGameNotification(tr("Draw game"));
		}

		m_gameEnded = true;

		return true;
	}

	if (m_rules->IsPassiveEndGame())
	{
		m_gameEnded = true;

		qDebug() << "Draw game";
		ShowEndGameNotification(tr("Passive draw game"));

		return true;
	}

	return false;
}

void Chess::HighlightMove(POSITION from, POSITION to)
{
	FindCell(PositionHelper::X(from), PositionHelper::Y(from))->setProperty("isHighlightAsPreviousMove", true);
	FindCell(PositionHelper::X(to), PositionHelper::Y(to))->setProperty("isHighlightAsPreviousMove", true);
}

void Chess::ShowEndGameNotification(QString text)
{
	QMetaObject::invokeMethod(m_gamePage, "showEndGameNotification", Q_ARG(QVariant, text));
}

Control* Chess::CreateUIPiece()
{
	qDebug() << "Chess::CreateUIPiece";

	QVariant returned;
	QObject* control;

	QMetaObject::invokeMethod(m_boardContainer, "createPiece", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returned));

	control = qvariant_cast<QObject*>(returned);

	return dynamic_cast<Control*>(control);
}

Control* Chess::FindCell(int x, int y)
{
	//qDebug() << "Chess::FindCell" << x << y;

	QVariant returned;
	bool res = QMetaObject::invokeMethod(m_boardContainer, "findCell", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returned), Q_ARG(QVariant, x), Q_ARG(QVariant, y));

	//qDebug() << "invokedMethod" << res;

	QObject* control = qvariant_cast<QObject*>(returned);

	//qDebug() << "qvariant_casted" << control;

	Control* c = dynamic_cast<Control*>(control);

	//qDebug() << c;

	return c;
}

void Chess::Draw()
{
	qDebug() << "Chess::Draw";

	if (!m_board->IsHistoryEmpty())
	{
		Move lastMove = m_board->GetLastMove();

		HighlightMove(lastMove.From, lastMove.To);
	}

	QList<Figure*> alive = m_board->GetAllAliveFigures();

	// delete ui piece when needed
	foreach(Figure* figure, m_figureToUIPieceMapping.keys())
	{
		Control* uiPiece = m_figureToUIPieceMapping[figure];

		if (!alive.contains(figure)) // when figure was killed
		{
			m_boardContainer->remove(uiPiece);

			uiPiece->setScaleY(1);

			if ((!m_boardFlipped && figure->Side == FigureSide::White)
					|| (m_boardFlipped && figure->Side == FigureSide::Black)) // white in down
			{
				m_bottomKilledDockContainer->add(uiPiece);
			} else
			{
				m_topKilledDockContainer->add(uiPiece);
			}
		}
	}

	// create ui peice when needed
	foreach(Figure* figure, alive)
	{
		if (!m_figureToUIPieceMapping.contains(figure))
		{
			Control* piece = CreateUIPiece();

			piece->setScaleY(m_boardFlipped ? -1 : 1);

			piece->setProperty("pieceFENChar", FEN::GetFigureChar(*figure));
			piece->setProperty("x", PositionHelper::X(figure->Position));
			piece->setProperty("y", PositionHelper::Y(figure->Position));

			m_figureToUIPieceMapping[figure] = piece;
		} else
		{
			Control* piece = m_figureToUIPieceMapping[figure];

			piece->setProperty("x", PositionHelper::X(figure->Position));
			piece->setProperty("y", PositionHelper::Y(figure->Position));
			piece->setProperty("pieceFENChar", FEN::GetFigureChar(*figure));

			if (!m_boardContainer->children().contains(piece))
			{
				piece->setScaleY(m_boardFlipped ? -1 : 1);

				m_bottomKilledDockContainer->remove(piece);
				m_topKilledDockContainer->remove(piece);
				m_boardContainer->add(piece);
			}
		}
	}
}
void Chess::TurnBack()
{
	ResetCellsHighlighting(true);

	if (!m_board->IsHistoryEmpty())
	{
		m_gameEnded = false;

		if (m_isPVC)
		{
			if (m_aiThinking)
			{
				ShowToast(tr("Please, wait while AI is thinking"));
			} else
			{
				// turn back AI move and user move
				do
				{
					m_rules->UnMakeMove(m_board->GetLastMove());
				} while (m_board->GetTurningSide() != m_userSide);
			}
		} else
		{
			// turns black only last move
			m_rules->UnMakeMove(m_board->GetLastMove());
		}

		UpdateTurnNotification();
	}
}

void Chess::NewGame()
{
	if (!m_aiThinking)
	{
		m_gameEnded = false;

		ResetCellsHighlighting(true);

		while(!m_board->IsHistoryEmpty())
		{
			m_rules->UnMakeMove(m_board->GetLastMove());
		}

		UpdateTurnNotification();
	} else
	{
		ShowToast(tr("Please, wait while AI is thinking"));
	}
}

void Chess::ShowToast(QString message)
{
	SystemToast *toast = new SystemToast(this);
	toast->setBody(message);
	// optional position MiddleCenter = 0, TopCenter = 1, BottomCenter = 2
	// toast->setPosition(bb::system::SystemUiPosition::Type(2));
	toast->show();
}

bool Chess::GetConfirmation(QString message)
{
	SystemDialog* prompt = new SystemDialog(this);

	prompt->setTitle(tr("Confirmation"));
	prompt->setBody(message);

	if (prompt->exec() == SystemUiResult::ConfirmButtonSelection)
		return true;
	else
		return false;
}

void Chess::ShowNotification(QString title, QString message)
{
	SystemDialog* notification = new SystemDialog(this);

	notification->setTitle(title);
	notification->setBody(message);

	notification->clearButtons();

	notification->show();
}

QVariant Chess::GetPuzzlesDataModel()
{
	QVariantList items;
	QList<Puzzle> puzzles = Puzzle::Parse("app/native/assets/mate-in-two.csv");
	foreach(const Puzzle& puzzle, puzzles)
	{
		QVariantMap itemMap;

		itemMap["id"] = puzzle.Id;
		itemMap["authors"] = puzzle.Authors;
		itemMap["source"] = puzzle.Source;
		itemMap["date"] = puzzle.Date.toString("MMM, yyyy");
		itemMap["positionFen"] = puzzle.PositionFEN;
		itemMap["solved"] = puzzle.Id % 2 == 0;

		items.append(itemMap);
	}

	return items;
}
