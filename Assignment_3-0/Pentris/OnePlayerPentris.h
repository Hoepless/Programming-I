#pragma once

#include <stdint.h>
#include <ncurses.h>
#include <time.h>

#include "Pentris.h"
#include "IPentrisPlayer.h"


class OnePlayerTetris : public IPentrisPlayer
{
    Pentris		m_Tetris;
	WINDOW*		m_Win;
	WINDOW*		m_WinScore;
	WINDOW*		m_WinNextBrick;
	clock_t		m_oldTime;
	int			m_Score, m_oldScore;
	int			m_Interval;

public:
	OnePlayerTetris(int x, int y);
	~OnePlayerTetris();

	void play();
	void showEndMessage();

protected:
	void openScreen();	// Preparation for nCurses
	void closeScreen();	// Cleaning up nCurses
	void updateWindows();
	void makeBoardWindow(int x, int y);
	void makeScoreWindow(int x, int y);
	void makeNextBrickWindow(int x, int y);
	void showBoard(bool bRefresh = true);		// Showing Tetris window
	void showBrick(bool bRefresh = true);
	void showScore();
	void showNextBrick();

	bool generateBrick();
	chtype lookup(int8_t src);	// Transforming from Tetris::m_board cells into representing letters
	bool drop();
	bool land();
	bool flow();
	bool clearRows();
	void pullDown();
	int getScore();

	inline chtype getBoard(int x, int y) { return lookup(m_Tetris.getBoard(x, y)); }
	inline int getCurX() { return m_Tetris.getCurX() * 2; }// Two columns for 1 block
	inline int getCurY() { return m_Tetris.getCurY(); }
	inline int getCurX(int block) { return m_Tetris.getCurX(block) * 2; }
	// Two columns for 1 block
	inline int getCurY(int block) { return m_Tetris.getCurY(block); }
	inline int getColor() { return lookup(m_Tetris.getColor()); }
	inline int clocksInMillisecond(int milliseconds) { return CLOCKS_PER_SEC * milliseconds / 1000; }
	inline void clearRow(int row) { m_Tetris.clearRow(row); }

	inline bool moveLeft() { return m_Tetris.moveLeft(); }
	inline bool moveRight() { return m_Tetris.moveRight(); }
	inline bool moveDown() { return m_Tetris.moveDown(); }
	inline bool rotate() { return m_Tetris.rotate(); }
	inline void pullDownTo(int row) { m_Tetris.pullDownTo(row); }
	inline int getLowestFilledRow() { return m_Tetris.getLowestFilledRow(); }
	inline int getNextWindowX(int block) { return m_Tetris.getNextX(block) * 2 + 6; }
	inline int getNextWindowY(int block) { return m_Tetris.getNextY(block) + 2; }
	inline int getOldWindowX(int block) { return m_Tetris.getOldX(block) * 2 + 6; }
	inline int getOldWindowY(int block) { return m_Tetris.getOldY(block) + 2; }
	inline int getNextColor() { return lookup(m_Tetris.getNextColor()); }
};

