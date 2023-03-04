/**
 * ExpansionActionOpenTraderMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenTraderMenu: ActionInteractBase
{
	private ref ExpansionTraderObjectBase m_TraderObject;
	private ExpansionMarketModule m_MarketModule;
	
	void ExpansionActionOpenTraderMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "#STR_USRACT_TRADE";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}
			
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TraderObject = ExpansionMarketModule.GetTraderFromObject(target.GetObject(), false);

		if (!m_TraderObject)
			return false;

		if (!CF_Modules<ExpansionMarketModule>.Get(m_MarketModule))
			return false;

		if (!m_MarketModule.CanOpenMenu())
			return false;

		if (!GetExpansionSettings().GetMarket().MarketSystemEnabled)
			return false;
		
		if (!GetGame().IsDedicatedServer())
		{
			if ( !m_TraderObject || !m_TraderObject.GetTraderMarket() )
				return false;

			m_Text = "#STR_USRACT_TRADE" + " - " + m_TraderObject.GetDisplayName();
		}

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		Print("ExpansionActionOpenTraderMenu::OnStartServer - Start");
		
		PlayerBase player = action_data.m_Player;
		
		if (!player)
		{
			Error("ExpansionActionOpenTraderMenu::OnExecuteServer - Player base is NULL!");
			return;
		}
		
		if (!player.GetIdentity())
		{
			Error("ExpansionActionOpenTraderMenu::OnExecuteServer - Player identity is NULL!");
			return;
		}
	}
		
	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);
		
		Print("ExpansionActionOpenTraderMenu::OnStartClient - Start");
		
		/**
		 * Client/server handshake
		 * 
		 * Client: RequestTraderData
		 * Server: RPC_RequestTraderData
		 * Server: LoadTraderData
		 * Client: RPC_LoadTraderData
		 * Client: RequestTraderItems
		 * Server: RPC_RequestTraderItems
		 * Server: LoadTraderItems
		 * Client: RPC_LoadTraderItems
		 **/
		m_MarketModule.RequestTraderData(m_TraderObject);
		m_MarketModule.OpenTraderMenu();
	}
}