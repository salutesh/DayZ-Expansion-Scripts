/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class PlayerBase
{
	protected ExpansionPartyPlayerData m_Expansion_PartyPlayerData; //! Server only
	protected int m_Expansion_PartyID = -1; //! Client and Server
	protected int m_ExpansionPartyLeaveTimestamp; //! Server only

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableInt("m_Expansion_PartyID");
	}
	
	override void SetActionsRemoteTarget(out TInputActionMap InputActionMap)
	{
		super.SetActionsRemoteTarget(InputActionMap);

		AddAction(ActionInviteToGroup, InputActionMap);
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Groups];
		if (!ctx) return;

		ctx.Write(m_ExpansionPartyLeaveTimestamp);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Groups];
		if (!ctx) return true;
		
		if (ctx.GetVersion() < 50)
			return true;

		if (!ctx.Read(m_ExpansionPartyLeaveTimestamp))
			return false;

		return true;
	}
	#endif
	
	int Expansion_GetPartyID()
	{
		return m_Expansion_PartyID;
	}
	
	void Expansion_SetPartyPlayerData(ExpansionPartyPlayerData partyPlayerData)
	{
		m_Expansion_PartyPlayerData = partyPlayerData;
		if (partyPlayerData)
			m_Expansion_PartyID = partyPlayerData.GetParty().GetPartyID();
		else
			m_Expansion_PartyID = -1;
		SetSynchDirty();
	}
	
	ExpansionPartyPlayerData Expansion_GetPartyPlayerData()
	{
		return m_Expansion_PartyPlayerData;
	}
	
	ExpansionPartyData Expansion_GetParty()
	{
		if (m_Expansion_PartyPlayerData)
			return m_Expansion_PartyPlayerData.GetParty();

		return NULL;
	}

	override bool Expansion_IsFriendly(PlayerBase other)
	{
		if (super.Expansion_IsFriendly(other))
			return true;

		if (m_Expansion_PartyID != -1 && m_Expansion_PartyID == other.Expansion_GetPartyID())
			return true;

		return false;
	}

	void Expansion_OnLeaveParty()
	{
		auto now = CF_Date.Now(true);
		m_ExpansionPartyLeaveTimestamp = now.GetTimestamp();
	}
	
	bool Expansion_IsPartyInviteCooldownActive()
	{
		if (Expansion_GetPartyInviteCooldown() > 0)
			return true;

		return false;
	}

	/**
	 * @brief Get remaining invite cooldown time in seconds
	 */
	int Expansion_GetPartyInviteCooldown()
	{
		auto now = CF_Date.Now(true);
		int timestamp = now.GetTimestamp();

		int elapsedTime = timestamp - m_ExpansionPartyLeaveTimestamp;
		if (elapsedTime < GetExpansionSettings().GetParty().InviteCooldown)
			return GetExpansionSettings().GetParty().InviteCooldown - elapsedTime;

		return 0;
	}
};