/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_Expansion_Reputation;

	override void Init()
	{
		super.Init();

		m_Expansion_Reputation = -1;

		RegisterNetSyncVariableInt("m_Expansion_Reputation");
	}

	void Expansion_SetReputation(int rep)
	{
		if (rep < 0)
			rep = 0;
		m_Expansion_Reputation = rep;
		SetSynchDirty();
	}

	void Expansion_AddReputation(int rep)
	{
		m_Expansion_Reputation += rep;
		if (m_Expansion_Reputation < 0)
			m_Expansion_Reputation = 0;
		SetSynchDirty();
	}

	void Expansion_DecreaseReputation(int rep)
	{
		Expansion_AddReputation(-rep);
	}

	int Expansion_GetReputation()
	{
		return m_Expansion_Reputation;
	}

	override void EEKilled(Object killer)
	{
		if (GetExpansionSettings().GetHardline().UseReputation)
		{
			ExpansionHardlineModule hardlineModule;
			if (CF_Modules<ExpansionHardlineModule>.Get(hardlineModule))
			{
				hardlineModule.OnEntityKilled(this, killer);
			}
		}

		super.EEKilled(killer);
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Hardline];
		if (!ctx) return;

		ctx.Write(m_Expansion_Reputation);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Hardline];
		if (!ctx) return true;

		if (ctx.GetVersion() < 47)
			return true;

		if (!ctx.Read(m_Expansion_Reputation))
			return false;

		return true;
	}
#endif
};