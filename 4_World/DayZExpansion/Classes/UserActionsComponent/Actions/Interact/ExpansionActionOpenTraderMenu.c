/**
 * ExpansionActionOpenTraderMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenTraderMenu: ActionInteractBase
{
	private ExpansionTraderBase m_Trader;
	private ExpansionMarketModule m_MarketModule;

	void ExpansionActionOpenTraderMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}

	override string GetText()
	{
		return "#STR_USRACT_TRADE";
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !Class.CastTo( m_Trader, target.GetObject() ) )
			return false;

		if ( !Class.CastTo( m_MarketModule, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
			return false;

		if ( !m_MarketModule.CanOpenTraderMenu() )
			return false;

		return true;
	}
	
	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		m_MarketModule.OpenTraderMenu();
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		if ( IsMissionClient() )
		{
			m_MarketModule.OpenTraderMenu();
		}

		m_MarketModule.LoadTraderData( m_Trader, action_data.m_Player.GetIdentity() );
	}
}