#pragma once


class cSTACK
{
public:
	cSTACK() : m_nTop(-1), m_nSize(0) 	{}
	~cSTACK() { delete[] m_stack; }

	BOOL push(short u, short v)
	{
		if(++m_nTop == m_nSize) 
			return FALSE;

		m_stack[m_nTop].x = u;
		m_stack[m_nTop].y = v;

		return TRUE;
	}

	BOOL pop(short* u, short* v)
	{
		if(m_nTop == -1) 
			return FALSE;

		*u = m_stack[m_nTop].x;
		*v = m_stack[m_nTop].y;

		m_stack[m_nTop].x = m_stack[m_nTop].y = 0;

		--m_nTop;
		return TRUE;
	}

	BOOL isEmpty()	{ return m_nTop == -1; }
	void setEmpty() { memset(m_stack, 0, m_nSize*sizeof(cPoint)); m_nTop = -1; }
	void setSize(int size)	{ m_nSize = size; m_stack = new cPoint[m_nSize]; memset(m_stack, 0, m_nSize*sizeof(cPoint)); }

private:
	int m_nTop;
	int m_nSize;
	struct cPoint
	{
		short x, y;
	}* m_stack;
};