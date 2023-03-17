class SpotlightLightProp: SpotLightBase
{
	void SpotlightLightProp()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo(60);
		SetSpotLightAngle(140);
		SetCastShadow(true);
		SetBrightnessTo(20.0);
		SetFadeOutTime(0.3);
		FadeIn(0.2);
		SetAmbientColor(0.8, 0.9, 1.0);
		SetDiffuseColor(0.8, 0.9, 1.0);
	}
};