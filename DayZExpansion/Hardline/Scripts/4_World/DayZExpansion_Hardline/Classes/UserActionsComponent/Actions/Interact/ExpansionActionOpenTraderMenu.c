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
		
		//! TODO: Condition should be moved to modded market module within Hardline and checked in Exec_RequestPurchase / Exec_RequestSell as well
		if (GetGame().IsServer() && GetExpansionSettings().GetHardline().UseReputation)
		{
			int minRep = m_TraderObject.GetTraderMarket().MinRequiredReputation;
			int maxRep = m_TraderObject.GetTraderMarket().MaxRequiredReputation;
			if (!Math.IsInRangeInt(player.Expansion_GetReputation(), minRep, maxRep))
			{
				if (player.GetIdentity())
				{
					ExpansionNotification("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE", new StringLocaliser("STR_EXPANSION_HARDLINE_REPUTATION_OUTOFRANGE_TRADER", minRep.ToString(), maxRep.ToString()), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
				}

				return false;
			}
		}

		return true;
	}
}
#endif
