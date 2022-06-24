/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	private bool m_IsQuestItem = false;

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_IsQuestItem");
	}
	
	bool IsQuestItem()
	{
		return m_IsQuestItem;
	}

	void SetIsQuestItem(bool state)
	{
		m_IsQuestItem = state;
	}
};