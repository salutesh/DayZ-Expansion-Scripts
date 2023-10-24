/**
 * ExpansionSafeZoneCylinder.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZoneCylinder
 * @brief
 **/
class ExpansionSafeZoneCylinder: ExpansionSafeZoneElement
{
	vector Center;
	float Radius;
	float Height;

	void ExpansionSafeZoneCylinder(vector center, float radius, float height)
	{
		Center = center;
		Radius = radius;
		Height = height;
	}

	override ExpansionZone Create()
	{
		return new ExpansionZoneCylinder(ExpansionZoneType.SAFE, Center, Radius, Height);
	}
};
