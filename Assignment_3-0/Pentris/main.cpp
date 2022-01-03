#include <string.h>
#include <ncurses.h>

#include "OnePlayerPentris.h"
#include "TwoPlayersPentris.h"

#define MARGIN    (10)

#define STR_HELP	u8"Usage: tetris [<option>]\n"    \
					u8"Option\n"	\
					u8"-1: Tetris game for one player\n"    \
					u8"-2: Tetris game for two players\n"    \
					u8"-h: Show this help\n\n"    \
					u8"<No option>: Tetris game for one player\n"


int showHelp();

int main(int argc, char* argv[])
{
	IPentrisPlayer*		tp = nullptr;

	switch (argc)
	{
	case 1:	tp = new OnePlayerTetris(MARGIN, 1);	break;
	case 2:
		if (!strcmp(argv[1], u8"-2"))
			tp = new TwoPlayersTetris;
		else if (!strcmp(argv[1], u8"-1"))
			tp = new OnePlayerTetris(MARGIN, 1);
	default:
		return showHelp();
	}

	tp->play();
	tp->showEndMessage();
	flushinp();
	timeout(-1);
	getch();
	delete tp;
	return 0;
}

int showHelp()
{
	printf(STR_HELP);
	return 0;
}
