class Expansion250NFrontLight extends CarLightBase
{
	void Expansion250NFrontLight()
	{
		m_SegregatedBrightness = 6;
		m_SegregatedRadius = 60;
		m_SegregatedAngle = 100;
		m_SegregatedColorRGB = Vector(1.0, 0.8, 0.8);
		
		m_AggregatedBrightness = 11;
		m_AggregatedRadius = 80;
		m_AggregatedAngle = 110;
		m_AggregatedColorRGB = Vector(1.0, 0.8, 0.8);
		
		FadeIn(0.3);
		SetFadeOutTime(0.25);
		
		SegregateLight();
	}
}
