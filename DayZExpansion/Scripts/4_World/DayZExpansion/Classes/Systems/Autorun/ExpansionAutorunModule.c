/**
 * ExpansionAutorunModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionAutorunModule)]
class ExpansionAutorunModule: CF_ModuleWorld
{
	protected int m_AutoWalkMode = 0;
	protected int m_OldAutoWalkMode = m_AutoWalkMode;
	protected bool m_StartedWithSprint = false;

	override void OnInit()
	{
		super.OnInit();

		EnableRPC();
	}

	// ------------------------------------------------------------
	// Expansion SHIFT
	// ------------------------------------------------------------
	static bool SHIFT()
	{		
   		return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
	}
	
	// ------------------------------------------------------------
	// Expansion CTRL
	// ------------------------------------------------------------
	static bool CTRL()
	{		
   		return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
	}
	
	// ------------------------------------------------------------
	// Expansion AutorunSync
	// ------------------------------------------------------------
	void AutorunSync(ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "AutorunSync");
#endif

		if ( !IsMissionHost() )
			return;
		
		int autoWalkMode;
		if ( !ctx.Read( autoWalkMode ) )
		{
			Error("ExpansionAutorunModule::AutorunSync can't read autoWalkMode");
			return;
		}
		
		PlayerBase player;
		if ( !Class.CastTo( player, target ) )
		{
			return;
		}

		if ( autoWalkMode > 0 )
	   	{
		   	if ( ( player.GetInputController().LimitsIsSprintDisabled() ) || ( autoWalkMode == 1 ) )
			{
		   		player.GetInputController().OverrideMovementSpeed( true, 2 );
		   	} else if ( autoWalkMode == 3 )
			{
		   		player.GetInputController().OverrideMovementSpeed( true, 1 );
		   	} else
		   	{
			   	player.GetInputController().OverrideMovementSpeed( true, 3 );
		   	}

		   	player.GetInputController().OverrideMovementAngle( true, 1 );
	   	}
	}
	
	// ------------------------------------------------------------
	// Expansion AutorunDisable
	// ------------------------------------------------------------
	void AutorunDisable(ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "AutorunDisable");
#endif

		if ( !IsMissionHost() )
			return;
		
		PlayerBase player;
		if ( !Class.CastTo( player, target ) )
		{
			return;
		}

		player.GetInputController().OverrideMovementSpeed( false, 1 );	
		player.GetInputController().OverrideMovementAngle( false, 1 );
	}
	
	// ------------------------------------------------------------
	// Expansion AutoRun
	// ------------------------------------------------------------
	void AutoRun()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "AutoRun");
#endif

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( m_AutoWalkMode > 0 && !SHIFT() )
		{
			m_AutoWalkMode = 0;
			player.GetInputController().OverrideMovementSpeed( false, 0 );
			player.GetInputController().OverrideMovementAngle( false, 0 );
			m_StartedWithSprint = false;
		} else if ( m_AutoWalkMode == 0 )
		{
			if ( SHIFT() )
			{
				m_AutoWalkMode = 2;
				m_StartedWithSprint = true;
			} else if ( CTRL() )
			{
				m_AutoWalkMode = 3;
				m_StartedWithSprint = false;
			} else {
				m_AutoWalkMode = 1;
				m_StartedWithSprint = false;
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateAutoWalk
	// ------------------------------------------------------------
	void UpdateAutoWalk()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "UpdateAutoWalk");
#endif

		if (GetGame() && GetGame().GetPlayer()) 
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if ( m_AutoWalkMode > 0 )
	  		{
	   			if ( ( player.GetInputController().LimitsIsSprintDisabled() && m_StartedWithSprint ) || ( m_AutoWalkMode == 1 && !m_StartedWithSprint ) )
	   	   		{
	   				player.GetInputController().OverrideMovementSpeed( true, 2 );
					m_AutoWalkMode = 1;
	   			}
	   			else if ( m_AutoWalkMode == 3 && !m_StartedWithSprint )
	   	   		{
	   				player.GetInputController().OverrideMovementSpeed( true, 1 );
					m_AutoWalkMode = 3;
	   			}
	   			else
	   			{
	   				player.GetInputController().OverrideMovementSpeed( true, 3 );
					m_AutoWalkMode = 2;
	   			}

	   			player.GetInputController().OverrideMovementAngle( true, 1 );
				
				if (m_OldAutoWalkMode != m_AutoWalkMode)
				{
					ScriptRPC rpc = new ScriptRPC;
					rpc.Write( m_AutoWalkMode );
					rpc.Send( player, ExpansionAutoRunRPC.AUTORUNSYNC, true );	
					
					m_OldAutoWalkMode = m_AutoWalkMode;
				}
	   		}
			else
			{
				if (m_OldAutoWalkMode != m_AutoWalkMode)
				{
					ScriptRPC rpc2 = new ScriptRPC;
					rpc2.Send( player, ExpansionAutoRunRPC.AUTORUNDISABLE, true );
					
					m_OldAutoWalkMode = m_AutoWalkMode;
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion IsDisabled
	// ------------------------------------------------------------
	bool IsDisabled()
	{
		return ( m_AutoWalkMode == 0 );
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionAutoRunRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionAutoRunRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	override void OnRPC(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "OnRPC");
#endif

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionAutoRunRPC.AUTORUNSYNC:
				AutorunSync( rpc.Context, rpc.Sender, rpc.Target );
			break;
			
			case ExpansionAutoRunRPC.AUTORUNDISABLE:
				AutorunDisable( rpc.Context, rpc.Sender, rpc.Target );
			break;
		}
	}
};
