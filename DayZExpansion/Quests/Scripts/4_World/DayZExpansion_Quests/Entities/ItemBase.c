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
	protected bool m_Expansion_IsQuestItem;

	void ItemBase()
	{
		RegisterNetSyncVariableBool("m_Expansion_IsQuestItem");
	}

	bool IsQuestItem()
	{
		return m_Expansion_IsQuestItem;
	}

	void SetIsQuestItem(bool state)
	{
		m_Expansion_IsQuestItem = state;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		//! Queue world quest items for deletion on next server start
		if (m_Expansion_IsQuestItem && !m_Expansion_IsStoreSaved && !GetHierarchyParent())
			Expansion_QueueEntityActions(ExpansionItemBaseModule.DELETE);

		super.CF_OnStoreSave(storage);
	}
	#endif
};