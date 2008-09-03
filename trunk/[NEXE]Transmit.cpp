void CTransmit::setHandPointer(CHandPoint handPt)
{
	m_handPt = handPt;
	if(m_handPt.m_mode == m_pastPt.m_mode)
	{	m_pastPt = m_handPt;	return;	}


	m_color = (COLOR)((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.inColorRegn(handPt.m_nX, handPt.m_nY);

	setWindowRegn();
	setRobotRegn();

	m_pastPt = m_handPt;
}