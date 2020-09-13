/**
 * ExpansionM18Teargas.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_M18SmokeGrenade_Teargas : M18SmokeGrenade_White
{
	protected const float MAX_SHOCK_INFLICTED = -25.0;
	protected const float MIN_SHOCK_INFLICTED = -20.0;

	protected int m_CoughTimer = 1000;
	protected bool m_ZoneActive = false;

	override void OnWorkStart()
	{
		super.OnWorkStart();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart Start");
		#endif

		m_ZoneActive = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, 2000, false);

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart End");
		#endif
	}

	protected void MakePlayerCough()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::MakePlayerCough Start");
		#endif

		m_CoughTimer = Math.RandomIntInclusive(500, 5000);
		ref array<Object> nearest_objects = new array<Object>();
		ref array<CargoBase> proxy_cargos = new array<CargoBase>();

		GetGame().GetObjectsAtPosition3D(GetPosition(), 5, nearest_objects, proxy_cargos);

		for (int i = 0; i < nearest_objects.Count(); i++)
		{
			Object nearest_object = nearest_objects.Get(i);
			if (nearest_object.IsInherited(PlayerBase))
			{
				PlayerBase player = PlayerBase.Cast(nearest_object);
				if (player && !IsProtected(player))
				{
					player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
					player.GiveShock(Math.RandomFloatInclusive(MAX_SHOCK_INFLICTED, MIN_SHOCK_INFLICTED));
				}
			}
		}

		if (m_ZoneActive)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, m_CoughTimer, false);

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::MakePlayerCough End");
		#endif

	}

	protected bool IsProtected(notnull Man player)
	{
		EntityAI mask = player.GetInventory().FindAttachment(InventorySlots.MASK);
		if (!mask) return false;

		string protectionPath = "CfgVehicles " + mask.GetType() + " Protection ";
		return GetGame().ConfigGetInt(protectionPath + "biological");
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		m_ZoneActive = false;
	}
}