CPattern::MOTION CPattern::getMotion()
{
	switch(m_ptList.size())
	{
	case 1: return MOVE;
	case 2: return CLICK;
	default: return NOTHING;
	}
}