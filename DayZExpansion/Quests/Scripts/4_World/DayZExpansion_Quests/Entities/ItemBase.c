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
	protected int m_Expansion_QuestID = -1;

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_Expansion_QuestID");
	}

	bool IsQuestItem()
	{
		return (m_Expansion_QuestID > -1);
	}
	
	int GetQuestID()
	{
		return m_Expansion_QuestID;
	}

	void SetQuestID(int id)
	{
		m_Expansion_QuestID = id;
	}
	
	override void DeferredInit()
	{
		super.DeferredInit();

		if (m_Expansion_IsStoreLoaded && m_Expansion_QuestID > -1)
		{
			if (!GetHierarchyRootPlayer())	
				GetGame().ObjectDelete(this);
		}
	}
	
	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Quests];
		if (!ctx) return;

		ctx.Write(m_Expansion_QuestID);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Quests];
		if (!ctx) return true;

		if (ctx.GetVersion() < 44)
			return true;

		if (!ctx.Read(m_Expansion_QuestID))
			return false;
		
		return true;
	}
	#endif
};
