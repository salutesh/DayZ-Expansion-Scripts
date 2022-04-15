/**
 * ExpansionPartyPlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionPartyPlayerPermissions
{
	NONE = 0,
	CAN_EDIT = 1, 										// 1
	CAN_INVITE = 2,									// 2
	CAN_KICK = 4,										// 4
	CAN_DELETE = 8,									// 8
	CAN_WITHDRAW_MONEY = 16			// 16
}

class ExpansionPartyPlayerData
{
	string UID;
	string Name;
	PlayerBase Player;
	int Permissions;
	ref ExpansionPartyData m_Party;

	//! These have to be here so that marker data can be retained when Navigation mod is not loaded
	ref ExpansionPlayerMarkerData m_TempMarkerData;
	ref ExpansionPlayerMarkerData Marker;

	void ExpansionPartyPlayerData(ExpansionPartyData party)
	{
		Permissions = ExpansionPartyPlayerPermissions.NONE;
		m_Party = party;
		m_TempMarkerData = new ExpansionPlayerMarkerData();
	}
	
	void ~ExpansionPartyPlayerData()
	{
		if ( m_TempMarkerData )
			delete m_TempMarkerData;

		if ( Marker )
			delete Marker;
	}

	void OnLeave()	{}

	void OnJoin(PlayerBase player)
	{
		Name = player.GetIdentityName();
	}
	
	void OnStoreSave(ParamsWriteContext ctx)
	{
		ctx.Write(UID);
		ctx.Write(Name);
		ctx.Write(Permissions);

		if ( Marker )
		{
			m_TempMarkerData = ExpansionPlayerMarkerData.Cast(ExpansionMarkerData.Copy(Marker));

			Marker.OnStoreSave(ctx);
		} 
		else
		{
			m_TempMarkerData.OnStoreSave(ctx);
		}
	}
	
	bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (Expansion_Assert_False(ctx.Read(UID), "[" + this + "] Failed reading UID"))
            return false;
 
        if (Expansion_Assert_False(ctx.Read(Name), "[" + this + "] Failed reading Name"))
            return false;
        
        if (version < 26)
        {
            bool promoted;
            if (Expansion_Assert_False(ctx.Read(promoted), "[" + this + "] Failed reading obsolete promoted"))
                return false;

            if (promoted)
                SetPermissions(ExpansionPartyPlayerPermissions.CAN_EDIT | ExpansionPartyPlayerPermissions.CAN_INVITE | ExpansionPartyPlayerPermissions.CAN_KICK | ExpansionPartyPlayerPermissions.CAN_DELETE | ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY);
        }
        else
        {
			 if (Expansion_Assert_False(ctx.Read(Permissions), "[" + this + "] Failed reading Permissions"))
            	return false;
        }
 
        if (version >= 9)
        {
            m_TempMarkerData.OnStoreLoad(ctx, version);
        }
                 
        return true;
    }

	void OnSettingChanged() {}
	
	string GetID()
	{
		return UID;
	}
	
	string GetName()
	{
		return Name;
	}
	
	ExpansionPartyData GetParty()
	{
		return m_Party;
	}

	//! Add the passed in permissions (single value or bitmask) to current permissions
	int AddPermission(int perm)
	{
		Permissions |= perm;
		return Permissions;
	}
	
	//! Remove the passed in permissions (single value or bitmask) from current permissions
	int RemovePermission(int perm)
	{
		Permissions &= ~perm;
		return Permissions;
	}
	
	//! Set permissions EXACTLY to the passed in single value or bitmask, replacing current permissions
	int SetPermissions(int perm)
	{
		Permissions = perm;
		return Permissions;
	}
	
	int GetPermissions()
	{
		return Permissions;
	}
	
	bool CanEdit()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_EDIT) != 0;
	}
	
	bool CanInvite()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_INVITE) != 0;
	}
	
	bool CanKick()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_KICK) != 0;
	}
	
	bool CanDelete()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_DELETE) != 0;
	}
	
	bool CanWithdrawMoney()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY) != 0;
	}
};
