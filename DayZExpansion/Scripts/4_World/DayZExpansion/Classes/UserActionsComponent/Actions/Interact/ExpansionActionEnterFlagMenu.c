/**
 * ExpansionActionEnterFlagMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionEnterFlagMenu
 * @brief		
 **/
class ExpansionActionEnterFlagMenu: ActionInteractBase
{
	protected ExpansionTerritoryModule m_TerritoryModule;
	
	// -----------------------------------------------------------
	// ExpansionActionEnterFlagMenu Destructor
	// -----------------------------------------------------------
	void ExpansionActionEnterFlagMenu()
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::ExpansionActionEnterFlagMenu - Start");
		#endif
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
		
		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::ExpansionActionEnterFlagMenu - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Override CreateConditionComponents
	// -----------------------------------------------------------
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}
	
	// -----------------------------------------------------------
	// Override GetText
	// -----------------------------------------------------------
	override string GetText()
	{
		return "#STR_EXPANSION_BB_FLAG_CHANGE";
	}
	
	// -----------------------------------------------------------
	// Override IsInstant
	// -----------------------------------------------------------
	override bool IsInstant()
	{
		return true;
	}

	// -----------------------------------------------------------
	// Override ActionCondition
	// -----------------------------------------------------------
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - Start");
		#endif
		
		if ( !target.GetObject() )
			return false;

	#ifdef DAYZ_1_09
		if ( !m_TerritoryModule )
			return false;
		
		if ( !GetExpansionSettings().GetBaseBuilding().EnableFlagMenu )
			return false;
		
		//! Is this a new flag ?
		TerritoryFlag flag;
		if ( Class.CastTo( flag, target.GetObject() ) )
		{
			//! Is the flag fully construced ?
			float state = flag.GetAnimationPhase("flag_mast");
			if ( flag.FindAttachmentBySlotName("Material_FPole_Flag") && state >= 0.99 && GetExpansionSettings().GetBaseBuilding().EnableFlagMenu )
			{	
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS CONSTRUCTED FLAG!");
				#endif
				
				//! Is flag already a territory flag?
				if ( flag.IsTerritoryFlag() )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS TERRITORY FLAG!");
					#endif
					
					//! Is he in a territory ?
					if ( player.IsInTerritory() )
						return player.IsInsideOwnTerritory(); //! show the prompt only if it's his territory
		
					//! Even if a user can place a flag, he can't create a territory if the limit is reached
					return true; //! Show the prompt
				} 
				else
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS NORMAL FLAG!");
					EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [2] End and return true!");
					#endif
					//! If flag is a normal flag anyone can access the menu?!
					return true;
				}
			}
		}
	#else
		//! Is this a flag ?
		ExpansionFlagBase flag;
		if ( Class.CastTo( flag, target.GetObject() ) )
		{
			//! Is he in a territory ?
			if ( player.IsInTerritory() )
				return player.IsInsideOwnTerritory(); //! show the prompt only if it's his territory

			//! Even if a user can place a flag, he can't create a territory if the limit is reached
			return true; //! Show the prompt
		}
	#endif
	
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [3] TerritoryFlag false!");
		#endif
		
		return false;
	}
	
	// -----------------------------------------------------------
	// Override OnStartClient
	// -----------------------------------------------------------
	override void OnStartClient(ActionData action_data)
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::OnStartClient - Start");
		#endif
		
		ExpansionTerritoryModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionTerritoryModule ) ) )
		{
			module.CloseMenus();
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::OnStartClient - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Override OnStartServer
	// -----------------------------------------------------------
	override void OnStartServer(ActionData action_data)
	{
		if (!(action_data.m_Player) || !(action_data.m_Player.GetIdentity()) || !(action_data.m_Target) || !(action_data.m_Target.GetObject())) return;
	#ifdef DAYZ_1_09
		TerritoryFlag flag;
		if ( Class.CastTo( flag, action_data.m_Target.GetObject() ) )
		{
			ScriptRPC rpc = new ScriptRPC;
			rpc.Send( flag, ExpansionTerritoryModuleRPC.OpenFlagMenu, true, action_data.m_Player.GetIdentity() );
			return;
		}
	#else
		ExpansionFlagBase flag;
		if ( Class.CastTo( flag, action_data.m_Target.GetObject() ) )
		{
			if ( flag.GetOwnerID() == action_data.m_Player.GetIdentityUID() )
			{
				ScriptRPC rpc = new ScriptRPC;
				rpc.Send( flag, ExpansionTerritoryModuleRPC.OpenFlagMenu, true, action_data.m_Player.GetIdentity() );
				return;
			}
		} 
	#endif
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ),  new StringLocaliser( "" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST, 5, action_data.m_Player.GetIdentity() );
	}
}