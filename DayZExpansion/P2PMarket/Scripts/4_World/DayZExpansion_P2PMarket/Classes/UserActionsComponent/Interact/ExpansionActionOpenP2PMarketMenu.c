/**
 * ExpansionActionOpenP2PMarketMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenP2PMarketMenu: ActionInteractBase
{
	void ExpansionActionOpenP2PMarketMenu()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "#STR_EXPANSION_MARKET_P2P_ACTION_OPENP2PMARKETMENU";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject;
		if (!Class.CastTo(targetObject, target.GetParentOrObject()))
			return false;

		auto object = ExpansionP2PMarketTraderStatic.Cast(targetObject);
		auto npc = ExpansionP2PMarketTraderNPC.Cast(targetObject);
	#ifdef ENFUSION_AI_PROJECT
		auto npcAI = ExpansionP2PMarketTraderNPCAI.Cast(targetObject);
	#endif

	#ifdef ENFUSION_AI_PROJECT
		if (!object && !npc && !npcAI)
	#else
		if (!object && !npc)
	#endif
			return false;

		if (!ExpansionP2PMarketModule.GetModuleInstance())
			return false;

		auto settings = GetExpansionSettings().GetP2PMarket(false);
		if (!settings.IsLoaded())
			return false;

		if (!settings.Enabled)
			return false;

#ifdef SERVER
		int traderID = -1;
		if (object)
			traderID = object.GetP2PTraderID();
		else if (npc)
			traderID = npc.GetP2PTraderID();
	#ifdef ENFUSION_AI_PROJECT
		else if (npcAI)
			traderID = npcAI.GetP2PTraderID();
	#endif

		ExpansionP2PMarketTraderConfig traderConfig = ExpansionP2PMarketModule.GetModuleInstance().GetP2PTraderConfigByID(traderID);
		if (!ExpansionP2PMarketModule.GetModuleInstance().CheckCanUseTrader(player, traderConfig))
			return false;
#endif

		return true;
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionP2PMarketMenu");
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Object targetObject;
		if (!Class.CastTo(targetObject, action_data.m_Target.GetParentOrObject()))
			return;

		auto object = ExpansionP2PMarketTraderStatic.Cast(targetObject);
		auto npc = ExpansionP2PMarketTraderNPC.Cast(targetObject);
	#ifdef ENFUSION_AI_PROJECT
		auto npcAI = ExpansionP2PMarketTraderNPCAI.Cast(targetObject);
	#endif

	#ifdef ENFUSION_AI_PROJECT
		if (!object && !npc && !npcAI)
	#else
		if (!object && !npc)
	#endif
			return;

		int traderID = -1;
		string traderName;
		string iconName;
		if (object)
		{
			traderID = object.GetP2PTraderID();
			object.m_Expansion_NetsyncData.Get(0, traderName);
			object.m_Expansion_NetsyncData.Get(1, iconName);
		}
		else if (npc)
		{
			traderID = npc.GetP2PTraderID();
			npc.m_Expansion_NetsyncData.Get(0, traderName);
			npc.m_Expansion_NetsyncData.Get(1, iconName);
		}
	#ifdef ENFUSION_AI_PROJECT
		else if (npcAI)
		{
			traderID = npcAI.GetP2PTraderID();
			npcAI.m_Expansion_NetsyncData.Get(0, traderName);
			npcAI.m_Expansion_NetsyncData.Get(1, iconName);
			
		#ifdef EXPANSIONMODAI
			ExpansionP2PMarketTraderConfig traderConfig = ExpansionP2PMarketModule.GetModuleInstance().GetP2PTraderConfigByID(traderID);
			if (traderConfig && traderConfig.GetEmoteID() > 0)
			{
				EmoteManager npcEmoteManager = npcAI.GetEmoteManager();
				if (!npcEmoteManager.IsEmotePlaying())
				{
					npcEmoteManager.PlayEmote(traderConfig.GetEmoteID());
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
				}
			}
		#endif
		}
	#endif

		if (!ExpansionP2PMarketModule.GetModuleInstance())
			return;

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
			return;

		ExpansionP2PMarketModule.GetModuleInstance().SendBMTraderData(traderID, player.GetIdentity(), traderName, iconName);
		ExpansionP2PMarketModule.GetModuleInstance().AddTradingPlayer(traderID, player.GetIdentity().GetId());
	}
};
