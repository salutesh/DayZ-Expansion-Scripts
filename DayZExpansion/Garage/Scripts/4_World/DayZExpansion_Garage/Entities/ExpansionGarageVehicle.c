/**
 * ExpansionGarageVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageVehicle: ExpansionEntityStoragePlaceholder
{
	vector m_Expansion_GaragePosition;

	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsServer())
		{
			m_Expansion_GaragePosition = ExpansionStatic.GetSurfacePosition(GetPosition()) - "0 1 0";
			SetPosition(m_Expansion_GaragePosition);
		}
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		return false;
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		//! Never decay
		SetLifetimeMax(3888000);

		//SetPosition(m_Expansion_GaragePosition);
	}
}
