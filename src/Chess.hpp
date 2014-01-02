// Navigation pane project template
#ifndef Chess_HPP_
#define Chess_HPP_

#include <bb/cascades/Container>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/Control>
#include <bb/cascades/Label>
#include <QObject>
#include <QString>
#include <QMap>
#include <QMultiMap>
#include <QDataStream>
#include <QSettings>

#include "board.h"
#include "rules.h"
#include "asyncai.h"


namespace bb { namespace cascades { class Application; }}

using namespace bb::cascades;

struct GameSave
{
	QString BoardString;
	bool IsPVC;
	int Depth;
	int UserSide;
};
Q_DECLARE_METATYPE(GameSave)


class Chess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isGameEnded READ getIsGameEnded)
    Q_PROPERTY(bool isFlipped READ getIsFlipped WRITE setIsFlipped)
    Q_PROPERTY(bool aiThinking READ getAiThinking NOTIFY aiThinkingChanged)

    Board* m_board;
	Rules* m_rules;
	AsyncAI* m_asyncAI;

    bool m_aiThinking;
    bool m_gameEnded;
    bool m_boardFlipped; // true when white on top
    bool m_isPVC; // is player vs. computer game

    Figure* m_selectedFigure;

    int m_depth;
    Figure::FigureSide m_userSide;

    NavigationPane* m_navigationPane;
    Page* m_gamePage;
    Container* m_boardContainer;
    Container* m_topKilledDockContainer;
    Container* m_bottomKilledDockContainer;
    Label* m_turnInfoLabel;
    QMap<Figure*, Control*> m_figureToUIPieceMapping;

    QSettings m_settings;

    void Initialize();
    Control* CreateUIPiece();
    Control* FindCell(int x, int y);
    void ResetCellsHighlighting(bool resetPrevMoveHighlight);
    bool CheckForEndGame();
    void HighlightMove(FigurePosition from, FigurePosition to);
    void ShowEndGameNotification(QString text);
    void UpdateTurnNotification();

private slots:
    void HandleFindedBestMove(QString bestMoveString);
public:
    Chess(bb::cascades::Application *app);
    virtual ~Chess() {}

    Q_INVOKABLE void HandleTap(int x, int y);
    Q_INVOKABLE void Draw();
    Q_INVOKABLE void TurnBack();
    Q_INVOKABLE void NewGame();
    Q_INVOKABLE void StartPVC(int depth);
    Q_INVOKABLE void StartPVP();
    Q_INVOKABLE void Continue();
    Q_INVOKABLE void Save();

    Q_INVOKABLE bool GetConfirmation(QString message);
    Q_INVOKABLE void ShowToast(QString message);
    Q_INVOKABLE void ShowNotification(QString title, QString message);

    Q_INVOKABLE bool getIsGameEnded() { return m_gameEnded; }
    Q_INVOKABLE bool getIsFlipped() { return m_boardFlipped; }
    Q_INVOKABLE void setIsFlipped(bool value) { m_boardFlipped = value; }
    Q_INVOKABLE bool getAiThinking() { return m_aiThinking; }

signals:
	void aiThinkingChanged();
};

#endif /* Chess_HPP_ */
