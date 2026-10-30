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
	protected ItemBase m_Target;  //! CLIENT/SP ONLY
	protected TentBase m_Tent;  //! OBSOLETE - no longer used
	
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
		auto settings = GetExpansionSettings().GetBaseBuilding(false);

		if (!settings.IsLoaded())
			return false;

		Object targetObject = target.GetObject();

		int component = target.GetComponentIndex();

		ItemBase tgtItem;
		TentBase tent = TentBase.Cast( target.GetParent() );
		if ( tent )
			tgtItem = tent;
		else
			tgtItem = ItemBase.Expansion_GetLockableItem( targetObject, component );

		if (!g_Game.IsDedicatedServer())
			m_Target = tgtItem;
		
		if ( tgtItem && !tgtItem.ExpansionIsLocked() && tgtItem.HasCode() )
		{
			if ( tent && !tgtItem.IsOpen() )
			{
				//! If CodelockActionsAnywhere is OFF, then "Change code"
				//! will only be possible from the tent entrance
				if ( !settings.CodelockActionsAnywhere )
				{
					if ( !targetObject )
						return false;

					array< string > selections = new array< string >;
					targetObject.GetActionComponentNameList( target.GetComponentIndex(), selections );

					bool isEntrance;
					for ( int s = 0; s < selections.Count(); s++ )
					{
						if ( !selections[s].Contains( "entrance" ) && !selections[s].Contains( "door" ) )
							continue;

						if ( tent.CanToggleAnimations( selections[s] ) )
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

			return ExpansionActionEnterCodeLock.Expansion_CodeLockInteractionCondition(player, settings, targetObject, component, tgtItem);
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

		int component = action_data.m_Target.GetComponentIndex();
		string selection = m_Target.GetActionComponentName( component );

		if ( !m_Target.ExpansionIsLocked() && m_Target.HasCode() )
		{
			ExpansionCodeLockUI.Open(m_Target, component, selection, true, true);
		} 
		else
		{
			auto rpc = ExpansionScriptRPC.Create(ItemBase.s_Expansion_SetCode_RPCID);
			rpc.Write(selection);
			rpc.Expansion_Send(m_Target, true);
		}
	}
}