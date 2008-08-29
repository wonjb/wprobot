void CMSPaint::clickPointer(int x, int y)
{
	CClientDC dc(this);

	x *= (m_region.Width()/320.f);
	y *= (m_region.Height()/240.f);

	dc.MoveTo(m_nX,m_nY);
	dc.LineTo(x,y);

	//	dc.SetPixel(x,y,RGB(0,0,0));

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, TRUE);

	m_nX = x, m_nY = y;
	//	this->Invalidate();	
}