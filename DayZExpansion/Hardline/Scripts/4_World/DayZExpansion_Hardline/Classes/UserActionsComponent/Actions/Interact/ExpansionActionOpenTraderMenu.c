/**
 * ExpansionActionOpenTraderMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
modded class ExpansionActionOpenTraderMenu
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		m_TraderObject = ExpansionMarketModule.GetTraderFromObject(target.GetObject(), false);
		
		if (m_TraderObject.GetTraderMarket().DisplayName == "HeroTrader" && !player.IsHero())
			return false;
		
		if (m_TraderObject.GetTraderMarket().DisplayName == "BanditTrader" && !player.IsBandit())
			return false;

		return true;
	}
}
#endif
