/**
 * ExpansionActionEnterCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionEnterCodeLock
 * @brief		
 **/
class ExpansionActionEnterCodeLock: ActionInteractBase
{
	//! WARNING: If 'IsLocal' is false, refactor this
	protected ItemBase m_Target;
	protected TentBase m_Tent;

	bool m_IsKnownUser;
	
	// -----------------------------------------------------------
	// ExpansionActionEnterCodeLock Destructor
	// -----------------------------------------------------------
	void ExpansionActionEnterCodeLock()
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
				return "#STR_EXPANSION_BB_CODE_UNLOCK";

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
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		m_Target = ItemBase.Cast( target.GetParentOrObject() );
		m_Tent = TentBase.Cast( m_Target );

		if ( m_Target )
		{
			m_IsKnownUser = m_Target.IsKnownUser( player );

			string selection = m_Target.GetActionComponentName( target.GetComponentIndex() );
			if ( m_Tent )
			{
				//! If CodelockActionsAnywhere is OFF, then "Set/Enter code", "Unlock" and
				//! "Close & lock" or will only be possible from the tent entrance
				if ( !GetExpansionSettings().GetBaseBuilding().CodelockActionsAnywhere )
				{
					Object targetObject = target.GetObject();

					if ( !targetObject )
						return false;

					array< string > selections = new array< string >;
					targetObject.GetActionComponentNameList( target.GetComponentIndex(), selections );

					bool isEntrance;
					for ( int s = 0; s < selections.Count(); s++ )
					{
						if ( !selections[s].Contains( "entrance" ) && !selections[s].Contains( "door" ) )
							continue;

						if ( m_Tent.CanToggleAnimations( selections[s] ) )
						{
							isEntrance = true;
							break;
						}
					}

					if ( !isEntrance )
						return false;
				}

				if ( m_Target.ExpansionHasCodeLock( selection ) && !m_Target.IsOpen() )
					return true;

				if ( m_Target.ExpansionIsLocked() || ( m_Target.HasCode() && !m_Target.IsOpen() ) )
					return true;

				return false;
			}

			return m_Target.ExpansionHasCodeLock( selection );
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
			}
			else
			{
				ExpansionCodeLockUI menu = ExpansionCodeLockUI.Cast( GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_CODELOCK_MENU, NULL ) );
				if ( menu )
				{
					menu.SetChangeCodelock( false );
					menu.SetConfirm( !m_Target.HasCode() );
					menu.SetTarget( m_Target, selection );
				}
			}
		} 
		else
		{
			auto rpc = ExpansionScriptRPC.Create(ItemBase.s_Expansion_Lock_RPCID);
			rpc.Write(selection);
			rpc.Expansion_Send(m_Target, true);
		}
	}
}