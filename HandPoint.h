#pragma once

class CHandPoint
{
public:
	CHandPoint() : m_nX(0), m_nY(0), m_bClick(FALSE), m_bWheel(FALSE) {}
	CHandPoint(int _x, int _y, BOOL _click = FALSE, BOOL _wheel = FALSE) 
		: m_nX(_x), m_nY(_y), m_bClick(_click), m_bWheel(_wheel) {}

public:
	int  m_nX, m_nY;
	BOOL m_bClick;
	BOOL m_bWheel;
};

enum COLOR { RED = 1, BLUE = 2, PUPLE = 4, BLACK = 8, NOTHING = 0 };
		//   0001	  0010		0100	   1000

typedef struct _sendData
{
	COLOR _color;
	unsigned short _x, _y;		// 0 ~ 65535
} SENDDATA;

// sizeof (SENDDATA) == 8
