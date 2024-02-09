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

	#ifdef EXPANSIONMODGROUPS
		if (Expansion_GetParty() == other.Expansion_GetParty())
			return true;
	#endif

		return false;
	}
};