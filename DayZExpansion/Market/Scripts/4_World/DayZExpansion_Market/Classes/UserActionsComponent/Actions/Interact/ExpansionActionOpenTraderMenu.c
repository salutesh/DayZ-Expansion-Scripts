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
	void ExpansionActionOpenTraderMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "#STR_USRACT_TRADE";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
			
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionTraderObjectBase trader = ExpansionMarketModule.GetTraderFromObject(target.GetObject(), false);

		if (!trader)
			return false;

		if (!GetExpansionSettings().GetMarket().MarketSystemEnabled)
			return false;

		if (!ExpansionMarketModule.s_Instance.CanOpenMenu())
			return false;
		
		if (!trader.GetTraderMarket() || !ExpansionMarketModule.s_Instance.CheckCanUseTrader(player, trader))
			return false;

		if (!GetGame().IsDedicatedServer())
			m_Text = "#STR_USRACT_TRADE" + " - " + trader.GetDisplayName();

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		Print("ExpansionActionOpenTraderMenu::OnStartServer - Start");
		
		PlayerBase player = action_data.m_Player;
		
		if (!player)
		{
			Error("ExpansionActionOpenTraderMenu::OnStartServer - Player base is NULL!");
			return;
		}
		
		if (!player.GetIdentity())
		{
			Error("ExpansionActionOpenTraderMenu::OnStartServer - Player identity is NULL!");
			return;
		}

		ExpansionTraderObjectBase trader = ExpansionMarketModule.GetTraderFromObject(action_data.m_Target.GetObject());

		if (!trader)
			return;

		/**
		 * Client/server handshake
		 * 
		 * Server: StartTrading
		 * Client: RPC_LoadTraderData
		 * Client: RequestTraderItems
		 * Server: RPC_RequestTraderItems
		 * Server: LoadTraderItems
		 * Client: RPC_LoadTraderItems
		 **/
		ExpansionMarketModule.s_Instance.StartTrading(trader, player.GetIdentity());
	}
}