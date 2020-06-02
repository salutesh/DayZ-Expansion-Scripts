/**
 * ActionStopEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStopEngine
{
	private CarScript m_ExCar;
	
	override string GetText()
	{
		if ( m_ExCar.IsPlane() )
		{
			return "#STR_EXPANSION_UA_STOP_PLANE";
		} else if ( m_ExCar.IsHelicopter() )
		{
			return "#STR_EXPANSION_UA_STOP_HELICOPTER";
		}

		return "#STR_EXPANSION_UA_STOP_CAR";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		IEntity vehicle = player.GetParent();		
		if ( !vehicle )
			return false;

		bool hasDualEngine = false;
		if ( vehicle.IsInherited( ExpansionBoatScript ) )
		{
			hasDualEngine = true;
			
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		} else if ( vehicle.IsInherited( ExpansionHelicopterScript ) || vehicle.IsInherited( ExpansionPlaneScript ) ) 
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		} else if ( vehicle.IsInherited( ExpansionBikeScript ) )
		{
			return false;
		} else 
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
		}
		
		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();

		if ( vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo( m_ExCar, trans ) && m_ExCar.EngineIsOn() )
				{
					if ( hasDualEngine || m_ExCar.GetSpeedometer() <= 8 )
						return true;
				}
			}
		}

		return false;
	}
}