/**
 * Ammunition_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Ammunition_Base
{
	override bool CanBeSplit()
	{
		if (IsQuestItem())
		{
			Man itemOwner = GetHierarchyRootPlayer();
			if (itemOwner && itemOwner.GetIdentity())
			{
				StringLocaliser text = new StringLocaliser("The item %1 is a quest item and can't be split!", GetDisplayName());
				ExpansionNotification("Can't split item", text, "Error", COLOR_EXPANSION_NOTIFICATION_ORANGE, 7).Create(itemOwner.GetIdentity());
			}

			return false;
		}

		return super.CanBeSplit();
	}
};
