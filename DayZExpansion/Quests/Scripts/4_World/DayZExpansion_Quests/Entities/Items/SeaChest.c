/**
 * SeaChest.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SeaChest
{
	private bool m_IsQuestChest = false;
	private ref ExpansionQuest m_Quest;

	void SetIsQuestChest(bool state)
	{
		m_IsQuestChest = state;
	}

	void SetLinkedQuest(ExpansionQuest quest)
	{
		m_Quest = quest;
	}
};