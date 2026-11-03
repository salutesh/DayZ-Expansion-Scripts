/**
 * ExpansionActionOpen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpen: ActionInteractBase
{
	ItemBase m_Target;  //! CLIENT/SP ONLY

	void ExpansionActionOpen()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		string text;

		if ( m_Target && m_Target.ExpansionIsLocked() )
		{
			string type = m_Target.GetType();
			ExpansionWallBase wall;
			if ( m_Target.IsInherited( ExpansionSafeBase ) )
			{
				text = "#STR_EXPANSION_OPEN_LOCKED_SAFE";
			}
			else if ( (Class.CastTo( wall, m_Target ) && wall.HasGate()) || type == "Fence" )
			{
				text = "#STR_EXPANSION_OPEN_LOCKED_GATE";
			}
			else if ( m_Target.IsKindOf( "Container_Base" ) && !m_Target.IsBuilding() )
			{
				if ( type.Contains("Safe") )
					text = "#STR_EXPANSION_OPEN_LOCKED_SAFE";
				else
					text = "#STR_EXPANSION_OPEN_LOCKED_CONTAINER";
			}
			else
			{
				text = "#STR_EXPANSION_OPEN_LOCKED_DOOR";
			}

			if (m_Target && GetPermissionsManager().IsAdminToolsToggledOn())
				text = "[ADMIN] " + text;
		}
		else
		{
			text = "#open";
		}

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

		return tgtItem.ExpansionCanOpen( player, component );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		int component = action_data.m_Target.GetComponentIndex();

		ItemBase tgtItem;
		if (!ItemBase.Expansion_GetLockableItem(tgtItem, action_data.m_Target.Expansion_GetObjectOrParent(), component))
			return;

		if ( tgtItem.ExpansionIsLocked() && !tgtItem.IsKnownUser( action_data.m_Player ) && !tgtItem.Expansion_CanUnlockFromInside(action_data.m_Player, action_data.m_Target.GetObject().GetActionComponentName(component)) )
			return;

		if ( tgtItem.ExpansionIsLocked() )
			tgtItem.UnlockAndOpen( component );
		else
			tgtItem.Expansion_Open( component );

	#ifdef EXPANSIONMODAI
		if (action_data.m_Player)
			eAINoiseSystem.AddNoise(action_data.m_Player, action_data.m_Player.GetPosition(), "CfgVehicles SurvivorBase NoiseActionDefault");
	#endif
	}
}
