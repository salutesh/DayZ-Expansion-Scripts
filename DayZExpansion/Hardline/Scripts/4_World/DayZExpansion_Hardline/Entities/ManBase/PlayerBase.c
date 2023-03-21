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
	ref ExpansionHardlinePlayerData m_Expansion_HardlineData;

	override void Init()
	{
		super.Init();

		m_Expansion_Reputation = -1;

		if (GetGame().IsServer())
			m_Expansion_HardlineData = new ExpansionHardlinePlayerData();

		RegisterNetSyncVariableInt("m_Expansion_Reputation");
	}

	//! Only to be called on server!
	void Expansion_SaveHardlineData(bool force = false)
	{
		bool save = force || m_Expansion_HardlineData.Reputation != m_Expansion_Reputation;
	#ifdef EXPANSIONMODAI
		save |= m_Expansion_HardlineData.FactionID != eAI_GetFactionTypeID();
	#endif
		if (GetIdentity() && save)
		{
			m_Expansion_HardlineData.Reputation = m_Expansion_Reputation;
			m_Expansion_HardlineData.Save(GetIdentity().GetId());
		}
	}

	//! Only to be called on server!
	void Expansion_SetReputation(int rep)
	{
		if (rep < 0)
			rep = 0;
		m_Expansion_Reputation = rep;
		SetSynchDirty();
		Expansion_SaveHardlineData();
	}

	//! Only to be called on server!
	void Expansion_AddReputation(int rep)
	{
		m_Expansion_Reputation += rep;
		if (m_Expansion_Reputation < 0)
			m_Expansion_Reputation = 0;
		SetSynchDirty();
		Expansion_SaveHardlineData();
	}

	//! Only to be called on server!
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
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Hardline];
		if (!ctx) return true;

		if (ctx.GetVersion() < 47 || ctx.GetVersion() >= 48)
			return true;

		if (!ctx.Read(m_Expansion_Reputation))
			return false;

		return true;
	}
#endif
};