class ExpansionVehicleActionPickLock: ExpansionActionPickVehicleLockBase 
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;
		
		if ( player.IsInSafeZone() )
			return false;

		ExpansionVehicleBase vehicleBase = ExpansionVehicleBase.Cast( target.GetParentOrObject() );

		if ( vehicleBase && vehicleBase.IsLocked() && GetExpansionSettings().GetVehicle().CanPickLock )
		{
			if ( GetExpansionSettings().GetVehicle().PickLockTools.Find( item.GetType() ) > -1 )
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		ExpansionVehicleBase vehicleBase = ExpansionVehicleBase.Cast( action_data.m_Target.GetParentOrObject() );

		if ( vehicleBase )
		{
			float pickLockChancePercent = GetExpansionSettings().GetVehicle().PickLockChancePercent;

			if ( GetExpansionSettings().GetLog().VehicleLockPicking )
				GetExpansionSettings().GetLog().PrintLog("[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " is attempting to pick lock on " + vehicleBase.GetType() + " (id=" + vehicleBase.GetVehiclePersistentIDString() + " pos=" + vehicleBase.GetPosition() + ") with " + action_data.m_MainItem.GetType() );

			if ( Math.RandomFloat( 0, 100 ) < pickLockChancePercent )
			{
				vehicleBase.UnlockCarWithoutKey();

				if ( GetExpansionSettings().GetLog().VehicleLockPicking )
					GetExpansionSettings().GetLog().PrintLog( "[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " picked lock on " + vehicleBase.GetType() + " (id=" + vehicleBase.GetVehiclePersistentIDString() + " pos=" + vehicleBase.GetPosition() + ") with " + action_data.m_MainItem.GetType() );
			}
			super.OnFinishProgressServer( action_data );
		}
	}
}
