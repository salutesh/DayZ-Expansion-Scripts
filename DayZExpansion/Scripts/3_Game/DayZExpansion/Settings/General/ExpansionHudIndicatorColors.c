/**
 * ExpansionHudIndicatorColors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHudIndicatorColors: ExpansionColorSettingsBase
{
	string StaminaBarColor;
	string StaminaBarColorHalf;
	string StaminaBarColorLow;

	string NotifierDividerColor;	//! Divider between stamina and Notifiers (health, hunger, tempe, etc)

	string TemperatureBurningColor;
	string TemperatureHotColor;
	string TemperatureIdealColor;
	string TemperatureColdColor;
	string TemperatureFreezingColor;

	string NotifiersIdealColor;		//! White
	string NotifiersHalfColor;		//! Yellow
	string NotifiersLowColor;		//! Red

	string ReputationBaseColor;
	string ReputationMedColor;
	string ReputationHighColor;

	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults
		if (!Get("StaminaBarColor"))
			Set("StaminaBarColor", ARGB(255, 255, 255, 255));

		if (!Get("StaminaBarColorHalf"))
			Set("StaminaBarColorHalf", ARGB(255, 255, 255, 255));

		if (!Get("StaminaBarColorLow"))
			Set("StaminaBarColorLow", ARGB(255, 255, 255, 255));

		if (!Get("NotifierDividerColor"))
			Set("NotifierDividerColor", ARGB(255, 220, 220, 220));

		// Temperatures
		if (!Get("TemperatureBurningColor"))
			Set("TemperatureBurningColor", ARGB(255, 220, 0, 0));

		if (!Get("TemperatureHotColor"))
			Set("TemperatureHotColor", ARGB(255, 220, 220, 0));

		if (!Get("TemperatureIdealColor"))
			Set("TemperatureIdealColor", ARGB(255, 220, 220, 220));

		if (!Get("TemperatureColdColor"))
			Set("TemperatureColdColor", ARGB(255, 0, 206, 209));

		if (!Get("TemperatureFreezingColor"))
			Set("TemperatureFreezingColor", ARGB(255, 30, 144, 220));

		// Notifiers
		if (!Get("NotifiersIdealColor"))
			Set("NotifiersIdealColor", ARGB(255, 220, 220, 220));

		if (!Get("NotifiersHalfColor"))
			Set("NotifiersHalfColor", ARGB(255, 220, 220, 0));

		if (!Get("NotifiersLowColor"))
			Set("NotifiersLowColor", ARGB(255, 220, 0, 0));

		if (!Get("ReputationBaseColor"))
			Set("ReputationBaseColor", ARGB(255, 220, 220, 220));

		if (!Get("ReputationMedColor"))
			Set("ReputationMedColor", ARGB(255, 220, 220, 0));

		if (!Get("ReputationHighColor"))
			Set("ReputationHighColor", ARGB(255, 220, 0, 0));
	}
}
