#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Pentris.h"

const int8_t Pentris::s_Brick[NUM_SHAPES][NUM_ROTATIONS][NUM_BLOCKS][NUM_COORDINATES] = {
    {    // I-shaped bricknePlayerPetris::
        { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },    // initial position
        { { 0, 0 }, { 0, -1 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },    // 180 degree rotated
        { { 0, 0 }, { 0, -1 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },    // 270 degrees rotated clockwise
    },
    {    // T-shaped brick
        { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, 2 } },    // initial position
        { { 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { -2, 0 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { 1, 0 }, { -1, 0 }, { 0, -1 }, { 0, -2 } },    // 180 degree rotated
        { { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, 0 }, { 2, 0 } },    // 270 degrees rotated clockwise
    },
    {    // O-shaped brick
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 }},        // initial position
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, -1 } },    // 180 degree rotated
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { -1, 0} },    // 270 degrees rotated clockwise
    },
    {    // J-shaped brick
        { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },    // initial position
        { { 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, 1 }, { -2, 1 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { 1, 0 }, { -1, 0 }, { -1, -1 }, { -1, -2 } },    // 180 degree rotated
        { { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, -1 }, { 2, -1 } },    // 270 degrees rotated clockwise
    },
    {    // L-shape};d brick
        { { 0, 0 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 1, 1} },    // initial position
        { { 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { 1, 0 }, { -1, 0 }, { 1, -1 }, { -1, -1 } },    // 180 degree rotated
        { { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 } },    // 270 degrees rotated clockwise
    },
    {    // Z-shaped brick
        { { 0, 0 }, { -1, 0 }, { 0, 1 }, { 1, 1 }, { -2, 0 } },    // initial position
        { { 0, 0 }, { 0, -1 }, { -1, 0 }, { -1, 1 }, { 0, -2 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { -1, 0 }, { 0, 1 }, { 1, 1 }, { -2, 0 } },    // 180 degree rotated
        { { 0, 0 }, { 0, -1 }, { -1, 0 }, { -1, 1 }, { 0, -2 } },    // 270 degrees rotated clockwise
    },
    {    // S-shaped brick
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { -1, 1 }, { 2, 0 } },    // initial position
        { { 0, 0 }, { 0, 1 }, { -1, 0 }, { -1, -1 }, { 0, 2 } },    // 90 degrees rotated clockwise
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { -1, 1 }, { 2, 0 } },    // 180 degree rotated
        { { 0, 0 }, { 0, 1 }, { -1, 0 }, { -1, -1 }, { 0, 2 } },    // 270 degrees rotated clockwise
    }
};

Pentris::Pentris(int nColors) : m_NumColors(nColors)
{
	static bool bSowed = false;
	if (!bSowed)
	{
		srand(time(NULL));
		bSowed = true;
	}

    m_NextBrick = getBrick((Pentris::SHAPE) (rand() % NUM_SHAPES));
	m_NextColor = (rand() % m_NumColors) + 1;
	m_CurRot = 0;
	initBoard();
}

void Pentris::initBoard()
{
	memset(m_Board, 0, sizeof m_Board);
	for (int x = 0; x < WIDTH; ++x)
	{
		m_Board[x][0]			= WALL;
		m_Board[x][HEIGHT-1]	= WALL;
	}
	for (int y = 1; y < HEIGHT-1; ++y)
	{
		m_Board[0][y]		= WALL;
		m_Board[WIDTH-1][y]	= WALL;
	}
}

bool Pentris::generateBrick()
{
	m_CurBrick = m_NextBrick;
    m_NextBrick = getBrick((Pentris::SHAPE) (rand() % NUM_SHAPES));
	m_CurX = WIDTH / 2;
	m_CurY = 1;
	m_CurRot = 0;
	m_Color = m_NextColor;
	m_NextColor = (rand() % m_NumColors) + 1;
	for (int i = 0; i < NUM_BLOCKS; ++i)
		if (EMPTY != getBoard(getCurX(i), getCurY(i)))
			return false;
	return true;
}

bool Pentris::moveLeft()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
	 if ((getCurX(i) <= 1)
		|| (EMPTY != getBoard(getCurX(i)-1, getCurY(i))))
		 return false;
	--m_CurX;
	return true;
}

bool Pentris::moveRight()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
		if ((getCurX(i) >= WIDTH-2)
			|| (EMPTY != getBoard(getCurX(i)+1, getCurY(i))))
		 return false;
	++m_CurX;
	return true;
}

bool Pentris::moveDown()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
		if (isTouched())   // When it touches the bottom wall
			return false;
	++m_CurY;
	return true;
}

bool Pentris::rotate()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
		if ((getRotatedX(i) <= 0) || (getRotatedX(i) >= WIDTH-1)
			|| (getRotatedY(i) <= 0) || (getRotatedY(i) >= HEIGHT-1)
			|| (EMPTY != m_Board[getRotatedX(i)][getRotatedY(i)]))
		 return false;
	++m_CurRot;
	m_CurRot %= 4;
	return true;
}

void Pentris::land()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
		setBoard(getCurX(i), getCurY(i), m_Color);
}

void Pentris::drop()
{
	while(moveDown());	// Pull down the brick to the end
}

bool Pentris::isTouched()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
	 if ((getCurY(i) >= HEIGHT-2)
		 || (EMPTY != m_Board[getCurX(i)][getCurY(i)+1]))
		 return true;
	}
	return false;
}

bool Pentris::isFilled(int row)
{
	for (int x = 1; x < WIDTH-1; ++x)
		if (EMPTY == m_Board[x][row])
			return false;
	return true;
}

bool Pentris::isClear(int row)
{
	for (int x = 1; x < WIDTH-1; ++x)
		if (EMPTY != m_Board[x][row])
			return false;
	return true;
}

int Pentris::getLowestFilledRow()
{
	for (int y = HEIGHT-2; y > 0; --y)
	 if (isFilled(y))
		 return y;
	return -1;
}

void Pentris::clearRow(int row)
{
	for (int x = 1; x < WIDTH-1; ++x)
		setBoard(x, row, EMPTY);
}

void Pentris::pullDownTo(int row)
{
	for (int y = row; y > 1; --y)
		for (int x = 1; x < WIDTH-1; ++x)
			setBoard(x, y, getBoard(x, y-1));
}

int Pentris::getScore()
{
	int	sum = 0;
	for (int row = 1; row < HEIGHT-1; ++row)
		sum += (int) isFilled(row);
	return sum;
}

bool Pentris::pullUp(int rows)
{
	for (int y = 1; y < rows; ++y)
		if (!isClear(y))
			return false;

	for (int y = 1; y < HEIGHT-1-rows ; ++y)
		for (int x = 1; x < WIDTH-1; ++x)
			setBoard(x, y, getBoard(x, y+rows));

	for (int y = HEIGHT-2; y > HEIGHT-2-rows; --y)
	{
		for (int x = 1; x < WIDTH-1; ++x)
			setBoard(x, y, m_Color);
		for (int i = 0; i < NUM_HOLES; ++i)
			setBoard((rand() % (WIDTH-2)) + 1, y, EMPTY);
	}
	return true;
}
