/**
 * ExpansionActionEnterFlagMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionEnterFlagMenuCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(-1.0);
	}
};

/**@class		ExpansionActionEnterFlagMenu
 * @brief		
 **/
class ExpansionActionEnterFlagMenu: ActionContinuousBase
{
	protected ExpansionTerritoryModule m_TerritoryModule;
	protected bool m_ActionCreate;
	
	// -----------------------------------------------------------
	// ExpansionActionEnterFlagMenu Destructor
	// -----------------------------------------------------------
	void ExpansionActionEnterFlagMenu()
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionActionEnterFlagMenu::ExpansionActionEnterFlagMenu - Start");
		#endif
		
		m_CallbackClass = ExpansionActionEnterFlagMenuCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon = CursorIcons.CloseDoors;
		
		CF_Modules<ExpansionTerritoryModule>.Get(m_TerritoryModule);
		
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
		m_ConditionTarget = new CCTCursor;
	}
	
	// -----------------------------------------------------------
	// Override GetText
	// -----------------------------------------------------------
	override string GetText()
	{
		string text;

		if (m_ActionCreate)
			text = "#STR_EXPANSION_BB_FLAG_CREATE_TERRITORY";
		else
			text = "#STR_EXPANSION_BB_FLAG_CHANGE";

		if (GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}
	
	// -----------------------------------------------------------
	// Override IsInstant
	// -----------------------------------------------------------
	override bool IsInstant()
	{
		return true;
	}

	override bool HasProgress()
	{
		return false;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
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

		if ( !m_TerritoryModule )
			return false;
		
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		if ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Disabled )
			return false;
		
		//! Is this a new flag ?
		TerritoryFlag flag;
		if ( Class.CastTo( flag, target.GetObject() ) )
		{
			//! Is the flag fully construced ?
			float state = flag.GetAnimationPhase("flag_mast");
			if ( flag.FindAttachmentBySlotName("Material_FPole_Flag") && state >= 0.99 )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS CONSTRUCTED FLAG!");
				#endif
				
				//! Is flag already a territory flag?
				if ( flag.HasExpansionTerritoryInformation() )
				{
					if ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Enabled )
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS TERRITORY FLAG!");
						#endif

						m_ActionCreate = false;
						
						//! Is he in a territory ?
						if ( player.IsInTerritory() )
							return player.IsInsideOwnTerritory(); //! show the prompt only if it's his territory
			
						//! Even if a user can place a flag, he can't create a territory if the limit is reached
						return true; //! Show the prompt
					}
				} 
				else
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - IS NORMAL FLAG!");
					#endif

					//! Is the player outside of any potential territories and their perimeters ?
					if ( !player.IsInTerritoryOrPerimeter() )
					{
						m_ActionCreate = true; //! Add the create territory action

						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [2] is not inside a territory or perimeter, end and return true!");
						#endif

						return true;
					}

					//! Is the player the owner of the nearest Territory and inside it ?
					if ( player.IsInsideOwnTerritory() )
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [2] is inside own territory, end and return true!");
						#endif

						return true;
					}

					//! Is the player the owner of the nearest Territory but in his perimeter ?
					if ( player.IsInsideOwnPerimeter() )
					{
						m_ActionCreate = true; //! Add the create territory action

						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [2] is inside own territory perimeter, end and return true!");
						#endif

						return true;
					}

					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionActionEnterFlagMenu::ActionCondition - [2] is inside other player territory, end and return false!");
					#endif

					return false;
				}
			}
		}
	
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
		if ( CF_Modules<ExpansionTerritoryModule>.Get(module) )
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
		
		TerritoryFlag flag;
		if ( Class.CastTo( flag, action_data.m_Target.GetObject() ) )
		{
			auto rpc = ExpansionScriptRPC.Create(ExpansionTerritoryModule.s_OpenFlagMenu_RPCID);
			rpc.Expansion_Send(flag, true, action_data.m_Player.GetIdentity());
			return;
		}
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "", EXPANSION_NOTIFICATION_ICON_T_Batteries, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(action_data.m_Player.GetIdentity());
	}
}