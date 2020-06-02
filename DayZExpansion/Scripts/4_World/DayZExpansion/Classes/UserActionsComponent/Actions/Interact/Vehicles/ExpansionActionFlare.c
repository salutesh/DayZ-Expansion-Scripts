/**
 * ExpansionActionFlare.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionFlare: ActionInteractBase
{
	private CarScript m_Vehicle;

	private ref Timer m_FlareTimer;
	private ref Timer m_FlareCooldown;

	private int m_Flares;

	void ExpansionActionFlare()
	{
		m_FlareTimer = new Timer();
		m_FlareCooldown = new Timer();

		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon	 	= CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget 	= new CCTNone;
	}	

	override string GetText()
	{
		return "#flare";
	}

	override typename GetInputType()
	{
		return ExpansionInputFlare;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			if ( Class.CastTo( m_Vehicle, vehCommand.GetTransport() ) )
			{
				if ( m_Vehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					if ( !m_FlareTimer.IsRunning() && !m_FlareCooldown.IsRunning() )
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	protected void SpawnFlare()
	{
		m_Flares += 1;

		if ( IsMissionClient() )
		{
			if ( m_Vehicle.MemoryPointExists( "flare_launcher1_dir" ) )
			{
				Particle p1 = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_FLARE, m_Vehicle.ModelToWorld( m_Vehicle.GetMemoryPointPos( "flare_launcher1_dir" ) ) );
				p1.SetOrientation( "0 90 0" );
			}

			if ( m_Vehicle.MemoryPointExists( "flare_launcher2_dir" ) )
			{
				Particle p2 = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_FLARE, m_Vehicle.ModelToWorld( m_Vehicle.GetMemoryPointPos( "flare_launcher2_dir" ) ) );
				p2.SetOrientation( "0 -90 0" );
			}
		}

		if ( m_Flares >= 5 )
		{
			m_FlareTimer.Stop();

			m_FlareCooldown.Run( 30, this, "", NULL, false ); 
		}
	}

	override void OnExecuteServer( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
				{
					m_Flares = 0;

					m_FlareTimer.Run( 0.25, this, "SpawnFlare", NULL, true ); 
				}
			}
		}
	}

	override void OnExecuteClient( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
				{
					m_Flares = 0;

					m_FlareTimer.Run( 0.25, this, "SpawnFlare", NULL, true ); 
				}
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}