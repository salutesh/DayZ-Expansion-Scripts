/**
 * ExpansionActionChangeCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	protected TentBase m_Tent;
	
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
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
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
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		m_Tent = TentBase.Cast( target.GetParent() );
		if ( m_Tent ) {
			m_Target = ItemBase.Cast( target.GetParent() );
		} else {
			m_Target = ItemBase.Cast( target.GetObject() );
		}
		
		if ( m_Target && !m_Target.ExpansionIsLocked() && m_Target.HasCode() )
		{
			if ( m_Tent && !m_Target.IsOpen() )
			{
				//! If CodelockActionsAnywhere is OFF, then "Change code"
				//! will only be possible from the tent entrance
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

				return true;
			}

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

		if ( !m_Target.ExpansionIsLocked() && m_Target.HasCode() )
		{
			ExpansionCodeLockUI menu = ExpansionCodeLockUI.Cast( GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_CODELOCK_MENU, NULL ) );
			if ( menu )
			{
				menu.SetChangeCodelock( true );
				menu.SetConfirm( true );
				menu.SetTarget( m_Target, selection );
			}
		} 
		else
		{
			auto rpc = ExpansionScriptRPC.Create(ItemBase.s_Expansion_SetCode_RPCID);
			rpc.Write(selection);
			rpc.Expansion_Send(m_Target, true);
		}
	}
}