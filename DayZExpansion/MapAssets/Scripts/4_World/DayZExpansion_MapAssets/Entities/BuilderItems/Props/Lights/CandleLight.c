class CandleLight: PointLightBase
{
	void CandleLight()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo(2.5);
		SetBrightnessTo(2.0);
		SetCastShadow(true);
		FadeIn(0.5);
		SetFadeOutTime(0.1);
		SetAmbientColor(1.0, 0.5, 0.3);
		SetDiffuseColor(1.0, 0.5, 0.3);
		SetFlickerAmplitude(0.3);
		SetFlickerSpeed(0.9);
		SetDancingShadowsMovementSpeed(0.010);
		SetDancingShadowsAmplitude(0.008);
	}
};