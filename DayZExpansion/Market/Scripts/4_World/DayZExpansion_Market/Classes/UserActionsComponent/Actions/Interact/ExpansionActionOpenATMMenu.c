/**
 * ExpansionActionOpenATMMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenATMMenu: ActionInteractBase
{	
	private ExpansionMarketModule m_MarketModule;

	void ExpansionActionOpenATMMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "#use";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!GetExpansionSettings().GetMarket(false).IsLoaded())
			return false;

		if (!GetExpansionSettings().GetMarket().ATMSystemEnabled)
			return false;
		
		if (!CF_Modules<ExpansionMarketModule>.Get(m_MarketModule))
			return false;

		if (!m_MarketModule.CanOpenMenu())
			return false;
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if ( !target.GetObject() )
				return false;

			m_Text = "#use" + " - " + target.GetObject().GetDisplayName();
		}

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
	}
		
	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);
		
		m_MarketModule.RequestPlayerATMData();
		m_MarketModule.OpenATMMenu();
	}
}