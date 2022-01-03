#include <string.h>

#include "TwoPlayersPentris.h"

#define SWIDTH	(12)
#define SHEIGHT	(5)
#define NWIDTH	(12)
#define NHEIGHT	(6)
#define MARGIN	(1)

#define BETWEEN_MARGIN	(2)
#define NUM_PLAYERS		(2)
#define KEY_UP2			('w')
#define KEY_UP2CAP 		('W')
#define KEY_LEFT2		('a')
#define KEY_LEFT2CAP	('A')
#define KEY_RIGHT2		('d')
#define KEY_RIGHT2CAP	('D')
#define KEY_DOWN2		('s')
#define KEY_DOWN2CAP	('S')
#define KEY_SPACE2		('\t')
#define KEY_SPACE		(' ')
#define KEY_ESC			(27)

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

#define INIT_INTERVAL				(700)
#define INTERVAL_DECREASE_RATE		(0.9)
#define SCORE_STEP					(150)
#define ANIMATION_INTERVAL			(100000000)

#define ARRAY_SIZE(x)    ((sizeof (x) / sizeof (x[0])))


TwoPlayersTetris::TwoPlayersTetris()
{
	openScreen();
	int    x = COLS / 2 + MARGIN, y = 1;
	int    bx[2] = { x, getCounterPartX(x, WIDTH*2) };
	int    sx[2] = { x + WIDTH*2 + BETWEEN_MARGIN,
				   getCounterPartX(x + WIDTH*2 + BETWEEN_MARGIN, SWIDTH) };
	int    nx[2] = { x + WIDTH*2 + BETWEEN_MARGIN,
				   getCounterPartX(x + WIDTH*2 + BETWEEN_MARGIN, NWIDTH) };

	m_Interval = INIT_INTERVAL;
	for (int id = 0; id < NUM_PLAYERS; ++id)
	{
		m_Score[id] = 0;
		m_oldScore[id] = 0;
		m_bAlive[id] = true;
		makeBoardWindow(id, bx[id], y);
		makeScoreWindow(id, sx[id], y);
		makeNextBrickWindow(id, nx[id], LINES - NHEIGHT - 1);
	}
}

TwoPlayersTetris::~TwoPlayersTetris()
{
	for (int id = 0; id < NUM_PLAYERS; ++id)
	{
		delwin(m_Win[id]);
		delwin(m_WinScore[id]);
		delwin(m_WinNextBrick[id]);
	}

	closeScreen();
}

void TwoPlayersTetris::openScreen()
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

void TwoPlayersTetris::closeScreen()
{
	echo();
	curs_set(1);
	keypad(stdscr, 0);
	timeout(-1);
	endwin();
}

void TwoPlayersTetris::makeBoardWindow(int playerID, int x, int y)
{
	m_Win[playerID] = newwin(HEIGHT, WIDTH*2, y, x);
	showBoard(playerID);
}

void TwoPlayersTetris::makeScoreWindow(int playerID, int x, int y)
{
	const char*    msg = u8" SCORE ";
	m_WinScore[playerID] = newwin(SHEIGHT, SWIDTH, y, x);
	box(m_WinScore[playerID], 0, 0);
	mvwprintw(m_WinScore[playerID], 0, (SWIDTH - strlen(msg)) / 2, msg);
	showScore(playerID);
}

void TwoPlayersTetris::makeNextBrickWindow(int playerID, int x, int y)
{
	const char*    msg = u8" NEXT ";
	m_WinNextBrick[playerID] = newwin(NHEIGHT, NWIDTH, y, x);
	box(m_WinNextBrick[playerID], 0, 0);
	mvwprintw(m_WinNextBrick[playerID], 0, (NWIDTH - strlen(msg)) / 2, msg);
	wrefresh(m_WinNextBrick[playerID]);
}

void TwoPlayersTetris::updateWindows()
{
	for (int id = 0; id < NUM_PLAYERS; ++id)
	{
		showBoard(id, false);
		showBrick(id);
		showScore(id);
		showNextBrick(id);
	}
}

void TwoPlayersTetris::play()
{
	m_oldTime = clock();
	for (int id = 0; id < NUM_PLAYERS; ++id)
		generateBrick(id);
	while (true)
	{
		for (int id = 0; id < NUM_PLAYERS; ++id)
		{
			if (!m_bAlive[id])
				continue;
			showBoard(id, false);
			showBrick(id);
		}
		switch (getch())
		{
		case KEY_UP:		if (m_bAlive[0]) rotate(0);		break;
		case KEY_UP2:
		case KEY_UP2CAP:	if (m_bAlive[1]) rotate(1);		break;
		case KEY_LEFT:		if (m_bAlive[0]) moveLeft(0);	break;
		case KEY_LEFT2:
		case KEY_LEFT2CAP:	if (m_bAlive[1]) moveLeft(1);	break;
		case KEY_RIGHT:		if (m_bAlive[0]) moveRight(0);	break;
		case KEY_RIGHT2:
		case KEY_RIGHT2CAP:	if (m_bAlive[1]) moveRight(1);	break;
		case KEY_DOWN:		if (m_bAlive[0]) moveDown(0);	break;
		case KEY_DOWN2:
		case KEY_DOWN2CAP:	if (m_bAlive[1]) moveDown(1);	break;
		case KEY_SPACE:
			if (!m_bAlive[0])
				break;
			if (drop(0))
				continue;
			break;
		case KEY_SPACE2:
			if (!m_bAlive[1])
				break;
			if (drop(1))
				continue;
			break;
		case KEY_ESC:		return;
		}
		if (!flow())
			return;
	}
}

bool TwoPlayersTetris::flow()
{
	clock_t    now = clock();
	if (now - m_oldTime > clocksInMillisecond(INIT_INTERVAL))
	{
		m_oldTime = now;
		bool    bRes = false;
		for (int id = 0; id < NUM_PLAYERS; ++id)
			bRes = m_bAlive[id] && (moveDown(id) || land(id)) || bRes;
		return bRes;
	}
	return true;
}


bool TwoPlayersTetris::generateBrick(int playerID)
{
	bool    bRes = m_Tetris[playerID].generateBrick();
	if (bRes)
	{
		showBrick(playerID);
		showNextBrick(playerID);
		if (m_Score[playerID] - m_oldScore[playerID] >= SCORE_STEP)
		{
			m_Interval *= INTERVAL_DECREASE_RATE;
			m_oldScore[playerID] = m_Score[playerID];
		}
	}
	else
	{
		m_bAlive[playerID] = false;
	}
	return bRes;
}

void TwoPlayersTetris::showBoard(int playerID, bool bRefresh)
{
	wmove(m_Win[playerID], 0, 0);
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			chtype	ch = getBoard(playerID, x, y);
			waddch(m_Win[playerID], (chtype) ch);
			waddch(m_Win[playerID], (chtype) ch);
		}
	}
	if (bRefresh)
		wrefresh(m_Win[playerID]);
}

void TwoPlayersTetris::showNextBrick(int playerID)
{
	chtype    old = ' ';
	chtype    ch = getNextColor(playerID);
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_WinNextBrick[playerID], getOldWindowY(playerID, i), getOldWindowX(playerID, i), old);
		waddch(m_WinNextBrick[playerID], old);
	}
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_WinNextBrick[playerID], getNextWindowY(playerID, i), getNextWindowX(playerID, i), ch);
		waddch(m_WinNextBrick[playerID], ch);
	}
	wrefresh(m_WinNextBrick[playerID]);
}

chtype TwoPlayersTetris::lookup(int8_t src)
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

void TwoPlayersTetris::showBrick(int playerID, bool bRefresh)
{
	chtype	ch = getColor(playerID);
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		mvwaddch(m_Win[playerID], getCurY(playerID, i), getCurX(playerID, i), ch);
		waddch(m_Win[playerID], ch);
	}
	if (bRefresh)
		wrefresh(m_Win[playerID]);
}

bool TwoPlayersTetris::drop(int playerID)
{
	m_Tetris[playerID].drop();
	return land(playerID);
}

bool TwoPlayersTetris::land(int playerID)
{
	m_Tetris[playerID].land();
	pullUpConditionaly(playerID);
	if (clearRows(playerID))
		pullDown(playerID);
	showScore(playerID);
	return generateBrick(playerID);
}

bool TwoPlayersTetris::clearRows(int playerID)
{
	int    row = getLowestFilledRow(playerID);
	if (-1 == row)
		return false;
	do
		clearRow(playerID, row);
	while (-1 != (row = getLowestFilledRow(playerID)));
	showBoard(playerID);
	return true;
}


void TwoPlayersTetris::pullDown(int playerID)
{
	int    y;
	for (y = 2; y < HEIGHT-1; ++y)
		if (!m_Tetris[playerID].isClear(y))
			break;
	for (; y < HEIGHT-1; ++y)
	{
		if (!m_Tetris[playerID].isClear(y))
			continue;
		timespec    t = { 0, ANIMATION_INTERVAL };
		nanosleep(&t, nullptr);
		pullDownTo(playerID, y);
		showBoard(playerID);
	}
}

void TwoPlayersTetris::showScore(int playerID)
{
	mvwprintw(m_WinScore[playerID], 2, SWIDTH / 2 - 2, "%4d", m_Score[playerID]);
	wrefresh(m_WinScore[playerID]);
}

int TwoPlayersTetris::getScore(int playerID)
{
	switch (m_Tetris[playerID].getScore())
	{
	case 1:   	 return 10;
	case 2:   	 return 20;
	case 3:   	 return 30;
	case 4:   	 return 100;
	}
	return 0;
}

void TwoPlayersTetris::pullUpConditionaly(int playerID)
{
	int    score = getScore(playerID);
	m_Score[playerID] += score;
	if (score > 10)
	pullUp(1-playerID, (100 == score) ? 3 : score / 15);
}

bool TwoPlayersTetris::pullUp(int playerID, int rows)
{
	if (0 >= rows || !m_Tetris[playerID].pullUp(rows))
		return false;
	showBoard(playerID);
	return true;
}

void TwoPlayersTetris::showEndMessage()
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
