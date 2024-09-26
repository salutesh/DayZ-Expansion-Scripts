/**
 * ExpansionP2PMarketModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODP2PMARKET
modded class ExpansionP2PMarketModule
{
	override bool CheckCanUseTrader(PlayerBase player, ExpansionP2PMarketTraderConfig trader)
	{
		if (!super.CheckCanUseTrader(player, trader))
			return false;

		if (GetGame().IsServer() && trader.m_UseReputation)
		{
			int minRep = trader.m_MinRequiredReputation;
			int maxRep = trader.m_MaxRequiredReputation;
			if (!Math.IsInRangeInt(player.Expansion_GetReputation(), minRep, maxRep))
			{
				if (maxRep != int.MAX)
					ExpansionNotification("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE", new StringLocaliser("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE_MINMAX_TRADER", minRep.ToString()), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7).Create(player.GetIdentity());
				else
					ExpansionNotification("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE", new StringLocaliser("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE_TRADER", minRep.ToString(), maxRep.ToString()), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7).Create(player.GetIdentity());

				return false;
			}
		}

		return true;
	}
};
#endif
