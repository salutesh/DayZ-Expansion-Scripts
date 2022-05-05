/**
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CarScript
{
	float m_Expansion_CrewKilledTimestamp;

	override void DamageCrew(float dmg)
	{
		if (dmg > GameConstants.CARS_CONTACT_DMG_KILLCREW)
			m_Expansion_CrewKilledTimestamp = GetGame().GetTickTime();

		super.DamageCrew(dmg);
	}
}
