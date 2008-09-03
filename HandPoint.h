#pragma once

class CHandPoint
{
public:
	enum MODE { SETTING, MOVE, DRAW, CLEAR, CIRCLE, TRIANGE, RECT, STAR, NOTHING };

public:
	CHandPoint() : m_mode(NOTHING), m_nX(0), m_nY(0) {}
	CHandPoint(MODE mode) : m_mode(mode), m_nX(0), m_nY(0) {}
	CHandPoint(MODE mode, int x, int y) : m_mode(mode), m_nX(x), m_nY(y) {}

public:
	MODE m_mode;
	unsigned short m_nX, m_nY;
};

enum COLOR { RED = 1, BLUE = 2, PUPLE = 4, BLACK = 8, NOTHING = 0 };
//   0001	  0010		0100	   1000

typedef struct _sendData
{
	COLOR _color;
	unsigned short _x, _y;		// 0 ~ 65535
} SENDDATA;

// sizeof (SENDDATA) == 8
