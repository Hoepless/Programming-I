#pragma once
#include <stdint.h>
#include <ncurses.h>
#include <time.h>

#include "Pentris.h"
#include "IPentrisPlayer.h"

#define NUM_PLAYERS	(2)

class TwoPlayersTetris : public IPentrisPlayer
{
    Pentris		m_Tetris[NUM_PLAYERS];
	WINDOW*		m_Win[NUM_PLAYERS];
	WINDOW*		m_WinScore[NUM_PLAYERS];
	WINDOW*		m_WinNextBrick[NUM_PLAYERS];
	clock_t		m_oldTime;
	int			m_Score[NUM_PLAYERS], m_oldScore[NUM_PLAYERS];
	int			m_Color[NUM_PLAYERS];
	int			m_NextColor[NUM_PLAYERS];
	int			m_Interval;
	bool		m_bAlive[NUM_PLAYERS];

public:
	TwoPlayersTetris();
	~TwoPlayersTetris();

	void play();
	void showEndMessage();

protected:
	void openScreen();	// Preparation for nCurses
	void closeScreen();	// Cleaning up nCurses
	void updateWindows();
	void makeBoardWindow(int playerID, int x, int y);
	void makeScoreWindow(int playerID, int x, int y);
	void makeNextBrickWindow(int playerID, int x, int y);
	void showBoard(int playerID, bool bRefresh = true);		// Showing Tetris window
	void showBrick(int playerID, bool bRefresh = true);
	void showScore(int playerID);
	void showNextBrick(int playerID);

	bool generateBrick(int playerID);
	chtype lookup(int8_t src);	// Transforming from Tetris::m_board cells into representing letters
	bool drop(int playerID);
	bool land(int playerID);
	bool flow();
	bool clearRows(int playerID);
	void pullDown(int playerID);
	int getScore(int playerID);
	bool pullUp(int playerID, int rows);
	void pullUpConditionaly(int playerID);

	inline chtype getBoard(int playerID, int x, int y) { return lookup(m_Tetris[playerID].getBoard(x, y)); }
	inline int getCurX(int playerID) { return m_Tetris[playerID].getCurX() * 2; }// Two columns for 1 block
	inline int getCurY(int playerID) { return m_Tetris[playerID].getCurY(); }
	inline int getCurX(int playerID, int block) { return m_Tetris[playerID].getCurX(block) * 2; }
	// Two columns for 1 block
	inline int getCurY(int playerID, int block) { return m_Tetris[playerID].getCurY(block); }
	inline int getColor(int playerID) { return lookup(m_Tetris[playerID].getColor()); }
	inline int clocksInMillisecond(int milliseconds) { return CLOCKS_PER_SEC * milliseconds / 1000; }
	inline void clearRow(int playerID, int row) { m_Tetris[playerID].clearRow(row); }

	inline bool moveLeft(int playerID) { return m_Tetris[playerID].moveLeft(); }
	inline bool moveRight(int playerID) { return m_Tetris[playerID].moveRight(); }
	inline bool moveDown(int playerID) { return m_Tetris[playerID].moveDown(); }
	inline bool rotate(int playerID) { return m_Tetris[playerID].rotate(); }
	inline void pullDownTo(int playerID, int row) { m_Tetris[playerID].pullDownTo(row); }
	inline int getLowestFilledRow(int playerID) { return m_Tetris[playerID].getLowestFilledRow(); }
	inline int getNextWindowX(int playerID, int block) { return m_Tetris[playerID].getNextX(block) * 2 + 5; }
	inline int getNextWindowY(int playerID, int block) { return m_Tetris[playerID].getNextY(block) + 2; }
	inline int getOldWindowX(int playerID, int block) { return m_Tetris[playerID].getOldX(block) * 2 + 5; }
	inline int getOldWindowY(int playerID, int block) { return m_Tetris[playerID].getOldY(block) + 2; }
	inline int getNextColor(int playerID) { return lookup(m_Tetris[playerID].getNextColor()); }
	inline int getCounterPartX(int x, int width) { return COLS - x - width; }
};

