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
		Object targetObj = target.GetObject();
		ExpansionTraderObjectBase trader = ExpansionMarketModule.GetTraderFromObject(targetObj, false);

		if (!trader)
			return false;

		ExpansionMarketSettings settings = GetExpansionSettings().GetMarket(false);

		if (!settings.IsLoaded())
			return false;

		if (!settings.MarketSystemEnabled)
			return false;

		if (!ExpansionMarketModule.s_Instance.CanOpenMenu())
			return false;

		if (!trader.GetTraderEntity())
		{
			EXError.InfoOnce(this, string.Format("Trader object %1 has no trader entity", targetObj));

			EntityAI traderEntity;
			if (Class.CastTo(traderEntity, targetObj))
			{
				EXError.Info(this, string.Format("Setting trader entity for object %1", targetObj));

				trader.SetTraderEntity(traderEntity);
				trader.RequestTraderObject();

				if (traderEntity.IsAnyInherited({AnimalBase, BuildingBase, DayZPlayerImplement, ZombieBase}))
				{
					ExpansionNetsyncData netsyncData;
					EnScript.GetClassVar(traderEntity, "m_Expansion_NetsyncData", 0, netsyncData);
					if (!netsyncData)
					{
						netsyncData = new ExpansionNetsyncData(traderEntity, false);
						EnScript.SetClassVar(traderEntity, "m_Expansion_NetsyncData", 0, netsyncData);
						netsyncData.Request();
					}
				}
			}
			else
			{
				return false;
			}
		}
		
		if (!trader.GetTraderMarket() || !ExpansionMarketModule.s_Instance.CheckCanUseTrader(player, trader))
			return false;

		if (!g_Game.IsDedicatedServer())
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