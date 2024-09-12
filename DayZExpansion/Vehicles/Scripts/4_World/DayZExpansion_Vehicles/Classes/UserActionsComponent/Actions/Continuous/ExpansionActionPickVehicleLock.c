class ExpansionActionPickVehicleLockBase: ExpansionActionToolBase 
{
	void ExpansionActionPickVehicleLockBase()
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
		m_Time = GetExpansionSettings().GetVehicle().PickLockTimeSeconds;
		m_Cycles = 1;
		m_ToolDamagePercent = GetExpansionSettings().GetVehicle().PickLockToolDamagePercent;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_PICK_VEHICLE_LOCK";
	}
}

class ExpansionActionPickVehicleLock: ExpansionActionPickVehicleLockBase 
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if ( player.Expansion_IsInSafeZone() )
			return false;

		auto vehicle = ExpansionVehicle.Get( target.GetParentOrObject() );

		if ( vehicle && vehicle.IsLocked() && GetExpansionSettings().GetVehicle().CanPickLock )
		{
			if ( GetExpansionSettings().GetVehicle().PickLockTools.Find( item.GetType() ) > -1 )
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		auto vehicle = ExpansionVehicle.Get( action_data.m_Target.GetParentOrObject() );

		if ( vehicle )
		{
			float lockComplexity = vehicle.GetLockComplexity();
			float pickLockChancePercent = GetExpansionSettings().GetVehicle().PickLockChancePercent;
			if (lockComplexity)
				pickLockChancePercent /= lockComplexity;

			if ( GetExpansionSettings().GetLog().VehicleLockPicking )
				GetExpansionSettings().GetLog().PrintLog("[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " is attempting to pick lock on " + vehicle.GetType() + " (id=" + vehicle.GetPersistentIDString() + " pos=" + vehicle.GetPosition() + ") with " + action_data.m_MainItem.GetType() );

			if ( Math.RandomFloat( 0, 100 ) < pickLockChancePercent )
			{
				vehicle.ForceUnlock();

				if ( GetExpansionSettings().GetLog().VehicleLockPicking )
					GetExpansionSettings().GetLog().PrintLog( "[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " picked lock on " + vehicle.GetType() + " (id=" + vehicle.GetPersistentIDString() + " pos=" + vehicle.GetPosition() + ") with " + action_data.m_MainItem.GetType() );
			}
			super.OnFinishProgressServer( action_data );
		}
	}

	override bool Expansion_CheckSuccess(ActionData action_data)
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Target.GetParentOrObject());
		if (vehicle && !vehicle.IsLocked())
			return true;

		return false;
	}
}
