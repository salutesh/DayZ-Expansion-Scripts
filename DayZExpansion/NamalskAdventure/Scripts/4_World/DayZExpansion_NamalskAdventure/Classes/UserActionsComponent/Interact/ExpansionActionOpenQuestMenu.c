/**
 * ExpansionActionOpenQuestMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS
modded class ExpansionActionOpenQuestMenu
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		if (!GetGame().IsDedicatedServer())
		{			
			bool changeText = false;
			string actionText;
			auto npcObject = ExpansionQuestStaticObject.Cast(target.GetParentOrObject());
			if (npcObject)
			{
				ExpansionQuestObjectCardboardBox cardboardBox = ExpansionQuestObjectCardboardBox.Cast(npcObject);
				ExpansionQuestObjectLocker locker = ExpansionQuestObjectLocker.Cast(npcObject);
				ExpansionQuestObjectGiftBox_Base giftBox = ExpansionQuestObjectGiftBox_Base.Cast(npcObject);
				if (cardboardBox || giftBox)
				{
					changeText = true;
					actionText = "Open";
				}
				else if (locker)
				{
					changeText = true;
					actionText = "Search";
				}
			}

			if (changeText)
				m_Text = actionText + " " + npcObject.GetDisplayName();
		}

		return true;
	}
}
#endif