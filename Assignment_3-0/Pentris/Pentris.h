#pragma once

#include <stdint.h>

#define WIDTH	(15)
#define HEIGHT	(24)

#define EMPTY	(0)
#define WALL	(-1)
#define COLOR1	(1)
#define COLOR2	(2)
#define COLOR3	(3)
#define COLOR4	(4)
#define COLOR5	(5)
#define COLOR6	(6)
#define COLOR7	(7)

#define NUM_SHAPES		(7)
#define NUM_ROTATIONS	(4)
#define NUM_BLOCKS		(5)
#define NUM_COORDINATES	(2)
#define NUM_HOLES		(2)		// number of holes to attack


typedef const int8_t (*PBrick)[NUM_BLOCKS][NUM_COORDINATES];
typedef const int8_t (*PCBrick)[NUM_COORDINATES];


class Pentris
{
	enum SHAPE { I, T, O, Z, S, J, L };

	const static int8_t	s_Brick[NUM_SHAPES][NUM_ROTATIONS][NUM_BLOCKS][NUM_COORDINATES];

	const uint8_t  m_NumColors;

	int8_t	m_Board[WIDTH][HEIGHT];
	PBrick	m_CurBrick, m_NextBrick;
	int8_t	m_CurRot;
	int8_t	m_CurX, m_CurY;
	int8_t	m_Color, m_NextColor;


public:
    Pentris(int nColors = 7);

	void initBoard();
	bool generateBrick();

	bool rotate();
	bool moveLeft();
	bool moveRight();
	bool moveDown();
	void drop();
	void land();
	bool isTouched();
	int getLowestFilledRow();
	void clearRow(int row);
	void pullDownTo(int row);
	bool isClear(int row);
	int getScore();
	bool pullUp(int rows);

	inline void setBoard(int x, int y, int8_t ch) { m_Board[x][y] = ch; }
	inline int8_t getBoard(int x, int y) { return m_Board[x][y]; }
	inline int8_t getCurX() { return m_CurX; }
	inline int8_t getCurY() { return m_CurY; }
	inline int8_t getCurX(int block) { return m_CurX + m_CurBrick[m_CurRot][block][0]; }
	inline int8_t getCurY(int block) { return m_CurY + m_CurBrick[m_CurRot][block][1]; }
	inline int8_t getCurRot() { return m_CurRot; }
	inline PBrick getCurBrick() { return m_CurBrick; }
	inline int8_t getColor() { return m_Color; }
	inline int getNextX(int block) { return m_NextBrick[0][block][0]; }
	inline int getNextY(int block) { return m_NextBrick[0][block][1]; }
	inline int getOldX(int block) { return m_CurBrick[m_CurRot][block][0]; }
	inline int getOldY(int block) { return m_CurBrick[m_CurRot][block][1]; }
	inline int8_t getNextColor() { return m_NextColor; }

protected:
	bool isFilled(int row);
	inline PBrick getBricks() { return m_CurBrick; }
	inline PCBrick getBricks(int block) { return m_CurBrick[block]; }
	inline PBrick getBrick(SHAPE shape)    { return s_Brick[shape]; }
	inline int getRotatedX(int block) { return m_CurX +	m_CurBrick[getNextRot()][block][0]; }
	inline int getRotatedY(int block) { return m_CurY +	m_CurBrick[getNextRot()][block][1]; }
	inline int getNextRot() { return (m_CurRot + 1) % 4; }
	inline PCBrick getCurRotBrick()  { return m_CurBrick[m_CurRot]; }
	inline PCBrick getRotatedBrick() { return m_CurBrick[(m_CurRot + 1) % 4 ]; }
	inline PBrick getNextBrick()    { return m_NextBrick; }
};
