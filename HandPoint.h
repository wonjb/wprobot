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
