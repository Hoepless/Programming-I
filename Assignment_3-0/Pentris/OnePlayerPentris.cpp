#include <string.h>

#include "OnePlayerPentris.h"


#define SWIDTH	(19)
#define SHEIGHT	(5)
#define NWIDTH	(16)
#define NHEIGHT	(6)
#define MARGIN	(10)

#define COLOR_WALL		(1)
#define COLOR_BRICK1	(2)
#define COLOR_BRICK2	(3)
#define COLOR_BRICK3	(4)
#define COLOR_BRICK4	(5)
#define COLOR_BRICK5	(6)
#define COLOR_BRICK6	(7)
#define COLOR_BRICK7	(8)
#define NUM_COLORS		(8)
#define FENCE		(ACS_PLUS | COLOR_PAIR(COLOR_WALL))
#define BLOCK(col)	(ACS_CKBOARD | COLOR_PAIR(col))

#define KEY_SPACE		(' ')
#define KEY_ESC			(27)

#define INIT_INTERVAL				(700)
#define INTERVAL_DECREASE_RATE		(0.9)
#define SCORE_STEP					(150)
#define ANIMATION_INTERVAL			(100000000)

#define ARRAY_SIZE(x)    ((sizeof (x) / sizeof (x[0])))


OnePlayerTetris::OnePlayerTetris(int x, int y)
{
	openScreen();
	m_Score = 0;
	m_oldScore = 0;
	m_Interval = INIT_INTERVAL;
	makeBoardWindow(x, y);
	makeScoreWindow(COLS - SWIDTH - MARGIN, y);
	makeNextBrickWindow(COLS - NWIDTH - MARGIN, LINES - NHEIGHT - 1);
}

OnePlayerTetris::~OnePlayerTetris()
{
	delwin(m_Win);
	delwin(m_WinScore);
	delwin(m_WinNextBrick);
	closeScreen();
}

void OnePlayerTetris::openScreen()
{
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, 1);
	timeout(0);
	if (has_colors())
	{
		start_color();
		init_color(COLOR_RED, 1000, 0, 0);
		init_color(COLOR_GREEN, 0, 1000, 0);
		init_color(COLOR_BLUE, 0, 0, 1000);
		init_color(COLOR_CYAN, 0, 1000, 1000);
		init_color(COLOR_MAGENTA, 1000, 0, 1000);
		init_color(COLOR_YELLOW, 1000, 1000, 0);
		init_color(COLOR_WHITE, 1000, 1000, 1000);

		init_pair(COLOR_WALL, COLOR_BLUE, COLOR_GREEN);
		init_pair(COLOR_BRICK1, COLOR_BLUE, COLOR_BLUE);
		init_pair(COLOR_BRICK2, COLOR_YELLOW, COLOR_YELLOW);
		init_pair(COLOR_BRICK3, COLOR_GREEN, COLOR_GREEN);
		init_pair(COLOR_BRICK4, COLOR_RED, COLOR_RED);
		init_pair(COLOR_BRICK5, COLOR_MAGENTA, COLOR_MAGENTA);
		init_pair(COLOR_BRICK6, COLOR_CYAN, COLOR_CYAN);
		init_pair(COLOR_BRICK7, COLOR_WHITE, COLOR_WHITE);

	}
	refresh();
}

void OnePlayerTetris::closeScreen()
{
	echo();
	curs_set(1);
	keypad(stdscr, 0);
	timeout(-1);
	endwin();
}

void OnePlayerTetris::makeBoardWindow(int x, int y)
{
	m_Win = newwin(HEIGHT, WIDTH*2, y, x);
	showBoard();
}

void OnePlayerTetris::makeScoreWindow(int x, int y)
{
	const char*    msg = u8" SCORE ";
	m_WinScore = newwin(SHEIGHT, SWIDTH, y, x);
	box(m_WinScore, 0, 0);
	mvwprintw(m_WinScore, 0, (SWIDTH - strlen(msg)) / 2, msg);
	showScore();
}

void OnePlayerTetris::makeNextBrickWindow(int x, int y)
{
	const char*    msg = u8" NEXT ";
	m_WinNextBrick = newwin(NHEIGHT, NWIDTH, y, x);
	box(m_WinNextBrick, 0, 0);
	mvwprintw(m_WinNextBrick, 0, (NWIDTH - strlen(msg)) / 2, msg);
	wrefresh(m_WinNextBrick);
}

void OnePlayerTetris::updateWindows()
{
	showBoard(false);
	showBrick();
	showScore();
	showNextBrick();
}

void OnePlayerTetris::play()
{
	m_oldTime = clock();
	generateBrick();
	while (true)
	{
		showBoard(false);
		showBrick();
		switch (getch())
		{
		case KEY_UP:		rotate();		break;
		case KEY_LEFT:		moveLeft();		break;
		case KEY_RIGHT:		moveRight();	break;
		case KEY_DOWN:		moveDown();		break;
		case KEY_SPACE:
			if (drop())
				continue;
			else
				return;

		case KEY_ESC:		return;
		}
		if (!flow())
			return;
	}
}

bool OnePlayerTetris::flow()
{
	clock_t    now = clock();
	if (now - m_oldTime > clocksInMillisecond(INIT_INTERVAL))
	{
		m_oldTime = now;
		return moveDown() || land();
	}
	return true;
}


bool OnePlayerTetris::generateBrick()
{
	bool    bRes = m_Tetris.generateBrick();
	if (bRes)
	{
		showBrick();
		showNextBrick();
		if (m_Score - m_oldScore >= SCORE_STEP)
		{
			m_Interval *= INTERVAL_DECREASE_RATE;
			m_oldScore = m_Score;
		}
	}
	return bRes;
}

void OnePlayerTetris::showBoard(bool bRefresh)
{
	wmove(m_Win, 0, 0);
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			chtype	ch = getBoard(x, y);
			waddch(m_Win, (chtype) ch);
			waddch(m_Win, (chtype) ch);
		}
	}
	if (bRefresh)
		wrefresh(m_Win);
}

void OnePlayerTetris::showNextBrick()
{
	chtype    old = ' ';
	chtype    ch = getNextColor();
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_WinNextBrick, getOldWindowY(i), getOldWindowX(i), old);
		waddch(m_WinNextBrick, old);
	}
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_WinNextBrick, getNextWindowY(i), getNextWindowX(i), ch);
		waddch(m_WinNextBrick, ch);
	}
	wrefresh(m_WinNextBrick);
}

chtype OnePlayerTetris::lookup(int8_t src)
{
	switch (src)
	{
	case EMPTY:		return (chtype) ' ';
	case WALL:		return (chtype) FENCE;
	case COLOR1:	return (chtype) BLOCK(COLOR_BRICK1);
	case COLOR2:	return (chtype) BLOCK(COLOR_BRICK2);
	case COLOR3:	return (chtype) BLOCK(COLOR_BRICK3);
	case COLOR4:	return (chtype) BLOCK(COLOR_BRICK4);
	case COLOR5:	return (chtype) BLOCK(COLOR_BRICK5);
	case COLOR6:	return (chtype) BLOCK(COLOR_BRICK6);
	case COLOR7:	return (chtype) BLOCK(COLOR_BRICK7);
	}
	return (chtype) ' ';
}

void OnePlayerTetris::showBrick(bool bRefresh)
{
	chtype	ch = getColor();
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_Win, getCurY(i), getCurX(i), ch);
		waddch(m_Win, ch);
	}
	if (bRefresh)
		wrefresh(m_Win);
}

bool OnePlayerTetris::drop()
{
	m_Tetris.drop();
	return land();
}

bool OnePlayerTetris::land()
{
	m_Tetris.land();
	m_Score += getScore();
	if (clearRows())
		pullDown();
	showScore();
	return generateBrick();
}

bool OnePlayerTetris::clearRows()
{
	int    row = getLowestFilledRow();
	if (-1 == row)
		return false;
	do
		clearRow(row);
	while (-1 != (row = getLowestFilledRow()));
	showBoard();
	return true;
}

void OnePlayerTetris::pullDown()
{
	int    y;
	for (y = 2; y < HEIGHT-1; ++y)
		if (!m_Tetris.isClear(y))
			break;
	for (; y < HEIGHT-1; ++y)
	{
		if (!m_Tetris.isClear(y))
			continue;
		timespec    t = { 0, ANIMATION_INTERVAL };
		nanosleep(&t, nullptr);
		pullDownTo(y);
		showBoard();
	}
}

void OnePlayerTetris::showScore()
{
	mvwprintw(m_WinScore, 2, SWIDTH / 2 - 2, "%4d", m_Score);
	wrefresh(m_WinScore);
}

int OnePlayerTetris::getScore()
{
	switch (m_Tetris.getScore())
	{
	case 1:   	 return 10;
	case 2:   	 return 20;
	case 3:   	 return 30;
	case 4:   	 return 100;
	}
	return 0;
}

void OnePlayerTetris::showEndMessage()
{
	const char*	msg = u8"Good Bye !!!";
	int			width = strlen(msg) + 4;
	const int	height = 5;
	int			x = (COLS - width) / 2;
	int			y = (LINES - height) / 2;
	WINDOW*	win = newwin(height, width, y, x);
	box(win, 0, 0);
	wattron(win, A_BLINK);
	mvwprintw(win, 2, 2, msg);
	wattroff(win, A_BLINK);
	wrefresh(win);
	flushinp();
	delwin(win);
}
