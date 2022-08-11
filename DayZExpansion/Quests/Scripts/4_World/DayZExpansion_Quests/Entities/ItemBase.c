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
	protected bool m_Expansion_IsQuestItemSaved;
	static ExpansionQuestModule s_QuestModule;

	void ItemBase()
	{
		RegisterNetSyncVariableBool("m_Expansion_IsQuestItem");
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (!GetGame().IsDedicatedServer() || !m_Expansion_IsStoreLoaded)
			return;

		//! Ideally would do this in AfterStoreLoad, but vanilla doesn't always call super >:(
		if (s_QuestModule || CF_Modules<ExpansionQuestModule>.Get(s_QuestModule))
		{
			s_QuestModule.WorldCleanup(this);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGame().IsDedicatedServer() || !m_Expansion_IsQuestItem)
			return;

		//! Deal with the case where a quest item is deleted while the server is running
		if (s_QuestModule || CF_Modules<ExpansionQuestModule>.Get(s_QuestModule))
		{
			s_QuestModule.WorldCleanup(this, false);
		}
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
		super.CF_OnStoreSave(storage);

		if (m_Expansion_IsQuestItem && !m_Expansion_IsQuestItemSaved)
		{
			m_Expansion_IsQuestItemSaved = true;
			if (s_QuestModule || CF_Modules<ExpansionQuestModule>.Get(s_QuestModule))
			{
				s_QuestModule.RegisterEntityForCleanup(this);
			}
		}
	}
	#endif
};