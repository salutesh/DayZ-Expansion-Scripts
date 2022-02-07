/**
 * ExpansionActionCloseAndLockSafe.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionCloseAndLockSafe: ActionInteractBase
{
	void ExpansionActionCloseAndLockSafe()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override string GetText()
	{
		return "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionSafeBase tgtSafe;
		if ( !Class.CastTo( tgtSafe, target.GetObject() ) )
			if ( !Class.CastTo( tgtSafe, target.GetParent() ) )
				return false;

		string selection = tgtSafe.GetActionComponentName( target.GetComponentIndex() );

		return tgtSafe.!ExpansionIsLocked() && tgtSafe.HasCode();
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ExpansionSafeBase tgtSafe
		if ( !Class.CastTo( tgtSafe, action_data.m_Target.GetObject() ) )
			if ( !Class.CastTo( tgtSafe, action_data.m_Target.GetParent() ) )
				return;

		string selection = tgtSafe.GetActionComponentName( action_data.m_Target.GetComponentIndex() );
		tgtSafe.CloseAndLock( selection );
	}
}