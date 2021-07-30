/**
 * ExpansionVehicleController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPlaneController
 * @brief		Abstract implementation for vehicle controllers
 */
class ExpansionVehicleController
{
	protected bool m_UseUAInterface = false;

	protected EntityAI m_Vehicle;
	protected PlayerBase m_Player;

	protected HumanCommandVehicle m_Command;
	protected ExpansionHumanCommandVehicle m_ECommand;

	void ExpansionVehicleController( EntityAI vehicle )
	{
		m_Vehicle = vehicle;
	}

	void Update()
	{
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);
		#endif

		Transport transport;
		ExpansionVehicleBase vehicleScript;
		if ( Class.CastTo( transport, m_Vehicle ) )
		{
			#ifdef EXPANSION_DEBUG_UI_VEHICLE
			dbg_Vehicle.Set("Controller", "Vanilla");
			#endif

			m_Player = PlayerBase.Cast( transport.CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );
		} else if ( Class.CastTo( vehicleScript, m_Vehicle ) )
		{
			#ifdef EXPANSION_DEBUG_UI_VEHICLE
			dbg_Vehicle.Set("Controller", "Expansion");
			#endif

			m_Player = PlayerBase.Cast( vehicleScript.CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );
		}

		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Player", m_Player);
		#endif

		if ( m_Player == NULL )
		{
			OnReset();

			return;
		}

		if ( !m_UseUAInterface && GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			return;
		}

		m_Command = m_Player.GetCommand_Vehicle();
		m_ECommand = m_Player.GetCommand_ExpansionVehicle();

		if ( m_Command || m_ECommand )
		{
			if ( m_Command )
			{
				if ( m_Command.IsGettingIn() )
				{
					OnReset();
				} else if ( m_Command.IsGettingOut() )
				{
					OnReset();
				} else if ( m_Command.IsSwitchSeat() )
				{
					OnReset();
				} else
				{
					OnUpdate();
				}
			} else if ( m_ECommand )
			{
				/*if ( m_ECommand.IsGettingIn() )
				{
					OnReset();
				} else if ( m_ECommand.IsGettingOut() )
				{
					OnReset();
				} else if ( m_ECommand.IsSwitchSeat() )
				{
					OnReset();
				} else*/
				{
					OnUpdate();
				}
			}
		} else
		{
			OnReset();
		}
	}

	/**
	 * Called when player is inside the driver seat
	 */
	protected void OnUpdate()
	{
	}

	/**
	 * Called when player is not inside vehicle or they are performing an action
	 */
	protected void OnReset()
	{
	}

	void NetworkSend(  ParamsWriteContext ctx )
	{
		if ( !m_UseUAInterface )
		{
			OnNetworkSend( ctx );
		}
	}

	protected void OnNetworkSend(  ParamsWriteContext ctx )
	{
		
	}

	bool NetworkRecieve(  ParamsWriteContext ctx )
	{
		if ( !m_UseUAInterface )
		{
			return OnNetworkRecieve( ctx );
		}

		return true;
	}

	protected bool OnNetworkRecieve( ParamsReadContext ctx )
	{
		return true;
	}

	protected void GetInputValue( string input, out float value )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( m_Player == NULL || !m_UseUAInterface )
			{
				value = value;
			} else 
			{
				value = m_Player.GetInputInterface().SyncedValue( input );
			}
		} else if ( m_Player )
		{
			value = GetGame().GetInput().LocalValue( input );
		}
	}

	protected void GetInputPress( string input, out bool value )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( m_Player == NULL || !m_UseUAInterface )
			{
				value = value;
			} else 
			{
				value = m_Player.GetInputInterface().SyncedPress( input );
			}
		} else if ( m_Player )
		{
			value = GetGame().GetInput().LocalPress( input );
		}
	}
}