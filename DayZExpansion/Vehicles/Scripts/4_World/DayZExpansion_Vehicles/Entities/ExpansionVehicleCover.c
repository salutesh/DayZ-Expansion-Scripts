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
	protected ExpansionKeyChainBase m_Expansion_KeyChain;

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		//! Fix position of Merlin cover (positioning of model in p3d changed with Expansion 1.8.39)
		if (GetType() == "ExpansionMerlin_Cover" && m_ExpansionSaveVersion < 52)
		{
			vector corrected = GetPosition() - Vector(0, 3.831, 0);
			EXPrint(ToString() + "::AfterStoreLoad - correcting position " + GetPosition() + " -> " + corrected);
			SetPosition(corrected);
		}
	}

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

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (item.IsInherited(ExpansionKeyChainBase))
			m_Expansion_KeyChain = ExpansionKeyChainBase.Cast(item);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (item.IsInherited(ExpansionKeyChainBase))
			m_Expansion_KeyChain = null;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUncoverVehicle);
		AddAction(ExpansionActionBoardBuoyantObject);
	}

	override string GetDisplayName()
	{
		string name = super.GetDisplayName();

		string ownerName = Expansion_GetOwnerName();
		if (ownerName)
			name = string.Format("%1's %2", ownerName, name);

		return name;
	}

	string Expansion_GetOwnerName()
	{
		if (m_Expansion_KeyChain)
			return m_Expansion_KeyChain.Expansion_GetOwnerName();

		return string.Empty;
	}
};

class Expansion_Generic_Vehicle_Cover: ExpansionVehicleCover {};

#ifndef CARCOVER
class CarCoverBase: ExpansionVehicleCover {};

class CamoNetCivil: CamoNet {};
class CamoNetDesert: CamoNet {};
class CamoNetWinter: CamoNet {};
#endif
