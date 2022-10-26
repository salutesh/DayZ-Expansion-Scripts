/**
 * ExpansionVehicleCover.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleCover: ExpansionEntityStoragePlaceholder
{
#ifdef GAMELABS
#ifdef SERVER
	protected ref _Event m_GameLabs_RegisteredInstance;

	override void DeferredInit()
	{
		super.DeferredInit();
		
		if (GetGameLabs())
		{
			string type;
			m_Expansion_NetsyncData.Get(0, type);
			string icon;
			if (GetGame().IsKindOf(type, "ExpansionHelicopterScript"))
				icon = GetExpansionSettings().GetVehicle().CFToolsHeliCoverIconName;
			else if (GetGame().IsKindOf(type, "ExpansionBoatScript"))
				icon = GetExpansionSettings().GetVehicle().CFToolsBoatCoverIconName;
			else
				icon = GetExpansionSettings().GetVehicle().CFToolsCarCoverIconName;
			m_GameLabs_RegisteredInstance = new _Event(type, icon, this);
			GetGameLabs().RegisterEvent(m_GameLabs_RegisteredInstance);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetGameLabs() && m_GameLabs_RegisteredInstance)
			GetGameLabs().RemoveEvent(m_GameLabs_RegisteredInstance);
	}
#endif
#endif

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUncoverVehicle);
	}
};

class Expansion_Generic_Vehicle_Cover: ExpansionVehicleCover {};

#ifndef CARCOVER
class CarCoverBase: ExpansionVehicleCover {};

class CamoNetCivil: CamoNet {};
class CamoNetDesert: CamoNet {};
class CamoNetWinter: CamoNet {};
#endif
