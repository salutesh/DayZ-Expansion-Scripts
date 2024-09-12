/**
 * ExpansionActionPickVehicleLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionChangeVehicleLock: ExpansionActionToolBase 
{
	void ExpansionActionChangeVehicleLock()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override bool SetupCondition()
	{
		return GetExpansionSettings().GetVehicle(false).IsLoaded();
	}

	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Time = GetExpansionSettings().GetVehicle().ChangeLockTimeSeconds;
		m_Cycles = 1;
		m_ToolDamagePercent = GetExpansionSettings().GetVehicle().ChangeLockToolDamagePercent;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_CHANGE_VEHICLE_LOCK";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;
		
		if ( player.Expansion_IsInSafeZone() )
			return false;

		auto vehicle = ExpansionVehicle.Get(target.GetParentOrObject());
		if (!vehicle)
			return false;

		if (vehicle.HasKey() && !vehicle.IsLocked())
		{
			if ( !GetExpansionSettings().GetVehicle().CanChangeLock )
				return false;

			if ( GetExpansionSettings().GetVehicle().ChangeLockTools.Find( item.GetType() ) > -1 )
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Target.GetParentOrObject());

		if (!vehicle)
			return;

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;

		ExpansionCarKey.GetKeysForVehicle(vehicle, keys);
		vehicle.ResetKeyPairing();

		foreach (auto key: keys)
			key.Unpair(true);
	}
}
