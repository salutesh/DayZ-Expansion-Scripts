/**
 * ExpansionActionOpenParachute.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenParachute: ActionSingleUseBase
{
	ExpansionParachuteBag m_Bag;

	void ExpansionActionOpenParachute()
	{
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTSelf;
	}

	override bool IsInstant()
	{
		return true;
	}

	override string GetText()
	{
		return "#STR_USRACT_OPEN_PARACHUTE";
	}
	
	override bool HasTarget()
	{
		return false;
	} 

/*
	override typename GetInputType()
	{
		return ExpansionInputOpenParachute;
	}
*/

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionDebugger.Push( EXPANSION_DEBUG_PLAYER_PARACHUTES );

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Expansion Fall: " + player.GetCommand_ExpansionFall() );
		if ( !player.GetCommand_ExpansionFall() )
		{
			return false;
		}
		
		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Parachute Deployed: " + player.GetCommand_ExpansionFall().IsParachuteDeployed() );
		if ( player.GetCommand_ExpansionFall().IsParachuteDeployed() )
		{
			return false;
		}

		vector contactPos;
		vector contactDir;
		int contactComponent;
		set<Object> objects = new set<Object>;

		vector start = player.GetPosition();
		vector end = start - "0 2.0 0";

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Raycast" );
		if ( GetGame().IsServer() )
		{
			if ( DayZPhysics.RaycastRV( start, end, contactPos, contactDir, contactComponent, objects, player, player, false, false, ObjIntersectGeom, 0.1 ) )
			{
				return false;
			}
		}

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Bag: " + player.GetAttachmentByType( ExpansionParachuteBag ) );
		if ( !Class.CastTo( m_Bag, player.GetAttachmentByType( ExpansionParachuteBag ) ) )
		{
			return false;
		}

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Can Deploy: " + m_Bag.CanDeployParachute() );
		if ( !m_Bag.CanDeployParachute() )
		{
			return false;
		}

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_PARACHUTES, "Success" );
		return true;
	}
	
	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.DeployParachute( m_Bag );
	}
}