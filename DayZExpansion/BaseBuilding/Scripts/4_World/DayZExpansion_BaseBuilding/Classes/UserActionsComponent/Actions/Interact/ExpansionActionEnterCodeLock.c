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
	protected ItemBase m_Target;  //! CLIENT/SP ONLY
	protected TentBase m_Tent;  //! OBSOLETE - no longer used

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

		if ( tgtItem )
		{
			if ( tgtItem.IsKnownUser( player ) || tgtItem.Expansion_CanUnlockFromInside( player, tgtItem.GetActionComponentName( component ) ) )
				m_IsKnownUser = true;
			else
				m_IsKnownUser = false;

			if ( tent )
			{
				//! If CodelockActionsAnywhere is OFF, then "Set/Enter code", "Unlock" and
				//! "Close & lock" or will only be possible from the tent entrance
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

				if ( tent.ExpansionHasCodeLock( component ) && !tent.IsOpen() )
					return true;

				if ( tent.ExpansionIsLocked() || ( tent.HasCode() && !tent.IsOpen() ) )
					return true;

				return false;
			}

			return Expansion_CodeLockInteractionCondition(player, settings, targetObject, component, tgtItem);
		}
		
		return false;
	}

	static bool Expansion_CodeLockInteractionCondition(PlayerBase player, ExpansionBaseBuildingSettings settings, Object targetObject, int component, ItemBase tgtItem)
	{
		ExpansionCodeLock codelock;
		if (Class.CastTo(codelock, tgtItem))
		{
			//! @note for rebuildable buildings, targetObject is the codelock if pointing at it from the outside, but not the inside
			if (targetObject != codelock && !settings.CodelockActionsAnywhere)
			{
				if (targetObject.IsBuilding())
				{
					//! Prevent double "Lock" action entry on opened building doors (when CodelockActionsAnywhere is OFF,
					//! "Close and lock" will be provided by ExpansionActionClose)
					if (codelock.HasCode() && codelock.IsOpen())
						return false;
				}
			}
		}
		else if (tgtItem.ExpansionHasCodeLock(component))
		{
			codelock = tgtItem.ExpansionGetCodeLock();
			if (!codelock)
				return false;
		}
		else
		{
			return false;
		}

		return !codelock.IsDamageDestroyed();
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
				ExpansionCodeLockUI.Open(m_Target, component, selection, false, !m_Target.HasCode());
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