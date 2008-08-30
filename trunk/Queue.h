#pragma once


class cQUEUE
{
public:
	enum { QUEUESIZE = 1000 };
public:
	cQUEUE() : m_nFront(0), m_nRear(0) 
	{
		memset(m_queue, 0, QUEUESIZE*sizeof(cPoint));
	}

	BOOL push(short u, short v)
	{
		int temp = (m_nRear+1)%QUEUESIZE;

		if(temp == m_nFront) 
			return FALSE;

		m_queue[temp].x = u;
		m_queue[temp].y = v;

		m_nRear = temp;
		return TRUE;
	}

	BOOL pop(short* u, short* v)
	{
		if(m_nFront == m_nRear) 
			return FALSE;

		int temp = (m_nFront+1)%QUEUESIZE;

		*u = m_queue[temp].x;
		*v = m_queue[temp].y;

		m_nFront = temp;
		return TRUE;
	}

	BOOL isEmpty()	{ return m_nFront == m_nRear; }

private:
	int m_nFront, m_nRear;	
	struct cPoint
	{
		short x, y;
	} m_queue[1000];
};