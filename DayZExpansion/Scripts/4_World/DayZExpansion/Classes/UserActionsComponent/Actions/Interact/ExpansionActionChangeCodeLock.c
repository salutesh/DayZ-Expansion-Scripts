/**
 * ExpansionActionChangeCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionChangeCodeLock
 * @brief		
 **/
class ExpansionActionChangeCodeLock: ActionInteractBase
{
	//! WARNING: If 'IsLocal' is false, refactor this
	protected ItemBase m_Target;
	
	// -----------------------------------------------------------
	// ExpansionActionChangeCodeLock Destructor
	// -----------------------------------------------------------
	void ExpansionActionChangeCodeLock()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}
	
	// -----------------------------------------------------------
	// Override CreateConditionComponents
	// -----------------------------------------------------------
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	// -----------------------------------------------------------
	// Override GetText
	// -----------------------------------------------------------
	override string GetText()
	{
		return "#STR_EXPANSION_BB_CODE_LOCK_CHANGE";
	}

	// -----------------------------------------------------------
	// Override IsLocal
	// -----------------------------------------------------------	
	override bool IsLocal()
	{
		return true;
	}

	// -----------------------------------------------------------
	// Override ActionCondition
	// -----------------------------------------------------------
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Target = ItemBase.Cast( target.GetObject() );
		
		if ( m_Target && !m_Target.IsLocked() && m_Target.HasCode() )
		{
			string selection = m_Target.GetActionComponentName( target.GetComponentIndex() );

			return m_Target.ExpansionHasCodeLock( selection ) && !m_Target.IsOpened();
		}
		
		return false;
	}

	// -----------------------------------------------------------
	// Override Start
	// -----------------------------------------------------------
	override void Start( ActionData action_data )
	{
		super.Start( action_data );

		if ( !IsMissionClient() || !m_Target )
			return;
		
		string selection = m_Target.GetActionComponentName( action_data.m_Target.GetComponentIndex() );

		if ( !m_Target.IsLocked() && m_Target.HasCode() )
		{
			ExpansionCodeLockUI menu = ExpansionCodeLockUI.Cast( GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_CODELOCK_MENU, NULL ) );
			if ( menu )
			{
				menu.SetChangeCodelock( true );
				menu.SetTarget( m_Target, selection );
			}
		} 
		else
		{
			ScriptRPC rpc = new ScriptRPC;
			rpc.Write(selection);
			rpc.Send(m_Target, ExpansionLockRPC.SET, true);
		}
	}
}