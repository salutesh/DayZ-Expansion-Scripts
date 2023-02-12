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
	}

	override bool IsServer()
	{
		return false;
	}

	static bool SHIFT()
	{		
   		return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
	}

	static bool CTRL()
	{		
   		return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
	}

	void AutoRun()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "AutoRun");
#endif

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( m_AutoWalkMode > 0 && !SHIFT() )
		{
			m_AutoWalkMode = 0;
			GetUApi().GetInputByName("UAMoveForward").ForceEnable(false);
			GetUApi().GetInputByName("UATurbo").ForceEnable(false);
			GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(false);
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
	   				GetUApi().GetInputByName("UAMoveForward").ForceEnable(true);
					m_AutoWalkMode = 1;
	   			}
	   			else if ( m_AutoWalkMode == 3 && !m_StartedWithSprint )
	   	   		{
	   				GetUApi().GetInputByName("UAMoveForward").ForceEnable(true);
					GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(true);
					m_AutoWalkMode = 3;
	   			}
	   			else
	   			{
					GetUApi().GetInputByName("UAMoveForward").ForceEnable(true);
					GetUApi().GetInputByName("UATurbo").ForceEnable(true);
					m_AutoWalkMode = 2;
	   			}
			}

			if (m_OldAutoWalkMode != m_AutoWalkMode)
			{
				m_OldAutoWalkMode = m_AutoWalkMode;
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
};
