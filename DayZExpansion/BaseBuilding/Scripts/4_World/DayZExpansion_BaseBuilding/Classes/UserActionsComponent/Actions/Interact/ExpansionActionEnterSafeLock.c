/**
 * ExpansionActionEnterSafeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionEnterSafeLock
 * @brief		
 **/
class ExpansionActionEnterSafeLock: ActionInteractBase
{
	//! WARNING: If 'IsLocal' is false, refactor this
	protected ItemBase m_Target;

	bool m_IsKnownUser;
	
	// -----------------------------------------------------------
	// ExpansionActionEnterSafeLock Destructor
	// -----------------------------------------------------------
	void ExpansionActionEnterSafeLock()
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
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}
	
	// -----------------------------------------------------------
	// Override GetText
	// -----------------------------------------------------------
	override string GetText()
	{
		if ( m_Target && m_Target.HasCode() )
		{
			if ( !m_Target.ExpansionIsLocked() )
				return "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";

			if ( m_IsKnownUser )
			{
				string text = "#STR_EXPANSION_BB_CODE_UNLOCK";

				if (GetPermissionsManager().IsAdminToolsToggledOn())
					text = "[ADMIN] " + text;

				return text;
			}

			return "#STR_EXPANSION_BB_CODE_LOCK_ENTER_CODE";
		}

		return "#STR_EXPANSION_BB_CODE_LOCK_SET_CODE";
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

		if ( m_Target )
		{
			m_IsKnownUser = m_Target.IsKnownUser( player );

			string selection = m_Target.GetActionComponentName( target.GetComponentIndex() );

			return m_Target.ExpansionHasCodeLock( selection ) && !m_Target.IsOpen();
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

		if ( m_Target.ExpansionIsLocked() || !m_Target.HasCode() )
		{
			if ( m_Target.HasCode() && m_IsKnownUser )
			{
				auto rpc2 = ExpansionScriptRPC.Create(ItemBase.s_Expansion_Unlock_RPCID);
				rpc2.Write( "" );
				rpc2.Write( selection );
				rpc2.Expansion_Send(m_Target, true);
			} else
			{
				ExpansionNumpadUI menu = ExpansionNumpadUI.Cast( GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_NUMPAD_MENU, NULL ) );
				if ( menu )
				{
					menu.SetChangeCodelock( false );
					menu.SetConfirm( !m_Target.HasCode() );
					menu.SetTarget( m_Target, selection );
				}
			}
		} else
		{
			auto rpc = ExpansionScriptRPC.Create(ItemBase.s_Expansion_Lock_RPCID);
			rpc.Write(selection);
			rpc.Expansion_Send(m_Target, true);
		}
	}
}