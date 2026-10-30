/**
 * ExpansionActionClose.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionClose: ActionInteractBase
{
	ItemBase m_Target;  //! CLIENT ONLY

	void ExpansionActionClose()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		string text;

		if ( Expansion_CloseAndLockCondition( m_Target ) )
			text = "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";
		else
			text = "#close";

		if (GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		int component = target.GetComponentIndex();

		ItemBase tgtItem;
		if (!ItemBase.Expansion_GetLockableItem(tgtItem, target.Expansion_GetObjectOrParent(), component))
			return false;

		if (!g_Game.IsDedicatedServer())
			m_Target = tgtItem;

		return tgtItem.ExpansionCanClose( player, component );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		int component = action_data.m_Target.GetComponentIndex();

		ItemBase tgtItem;
		if (!ItemBase.Expansion_GetLockableItem(tgtItem, action_data.m_Target.Expansion_GetObjectOrParent(), component))
			return;

		if ( Expansion_CloseAndLockCondition( tgtItem ) )
			tgtItem.CloseAndLock( component );
		else
			tgtItem.Expansion_Close( component );
	}

	//! If CodelockActionsAnywhere is ON, then "Close and lock" will be provided by ExpansionActionEnterCodeLock
	//! If it is OFF, then it will be provided by this action
	static bool Expansion_CloseAndLockCondition( ItemBase item )
	{
		if ( !item.Expansion_IsCodeLock() )
			item = item.Expansion_GetCodeLock();

		if (item && !item.IsDamageDestroyed() && item.HasCode())
		{
			//! Enable "Close & lock" for non-Expansion codelocks
			if (!item.IsInherited(ExpansionCodeLock))
				return true;

			return !GetExpansionSettings().GetBaseBuilding(false).CodelockActionsAnywhere;
		}

		return false;
	}
}