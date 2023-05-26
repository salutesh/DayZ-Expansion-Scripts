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
	private int m_Expansion_PersonalStorageLevel;
	ref ExpansionHardlinePlayerData m_Expansion_HardlineData;

	override void Init()
	{
		super.Init();

		m_Expansion_Reputation = -1;

		RegisterNetSyncVariableInt("m_Expansion_Reputation");
		RegisterNetSyncVariableInt("m_Expansion_PersonalStorageLevel", 0, 255);
	}

	//! Only to be called on server!
	void Expansion_SaveHardlineData(bool force = false)
	{
		bool save = force || m_Expansion_HardlineData.Reputation != m_Expansion_Reputation || m_Expansion_HardlineData.PersonalStorageLevel != m_Expansion_PersonalStorageLevel;
	#ifdef EXPANSIONMODAI
		save |= m_Expansion_HardlineData.FactionID != eAI_GetFactionTypeID();
	#endif
		if (GetIdentity() && save)
		{
			m_Expansion_HardlineData.Reputation = m_Expansion_Reputation;
			m_Expansion_HardlineData.PersonalStorageLevel = m_Expansion_PersonalStorageLevel;
			m_Expansion_HardlineData.Save(GetIdentity().GetId());
		}
	}

	void Expansion_SetHardlineData(ExpansionHardlinePlayerData data)
	{
		m_Expansion_HardlineData = data;
	#ifdef EXPANSIONMODAI
		//! @note this takes precedence over random faction from AISettings.json
		if (m_Expansion_HardlineData.FactionID != -1)
		{
			typename factionType = eAIFaction.GetTypeByID(m_Expansion_HardlineData.FactionID);
			if (factionType)
				SetGroup(eAIGroup.CreateGroup(eAIFaction.Cast(factionType.Spawn())));
		}
	#endif
		m_Expansion_PersonalStorageLevel = m_Expansion_HardlineData.PersonalStorageLevel;
		//! If data was successfully loaded, player rep will be set to value from file, else zero
		Expansion_SetReputation(m_Expansion_HardlineData.Reputation);
	}

	//! Only to be called on server!
	void Expansion_SetReputation(int rep, bool forceSave = false)
	{
		if (rep < 0)
		{
			rep = 0;
		}
		else
		{
			int maxRep = GetExpansionSettings().GetHardline().MaxReputation;
			if (maxRep > 0 && rep > maxRep)
				rep = maxRep;
		}
		
		m_Expansion_Reputation = rep;
		
		SetSynchDirty();
		Expansion_SaveHardlineData(forceSave);
	}

	//! Only to be called on server!
	void Expansion_AddReputation(int rep)
	{
		Expansion_SetReputation(m_Expansion_Reputation + rep);
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

	//! Only to be called on server!
	void Expansion_SetPersonalStorageLevel(int lvl)
	{
		m_Expansion_PersonalStorageLevel = lvl;
		SetSynchDirty();
		Expansion_SaveHardlineData();
	}

	int Expansion_GetPersonalStorageLevel()
	{
		return m_Expansion_PersonalStorageLevel;
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (GetExpansionSettings().GetHardline().UseReputation)
			ExpansionHardlineModule.GetModuleInstance().OnEntityKilled(this, killer);
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