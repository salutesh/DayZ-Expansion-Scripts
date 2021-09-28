/**
 * ExpansionPartyPlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	bool Promoted;

	PlayerBase Player;

	ref ExpansionPlayerMarkerData m_TempMarkerData;

	ref ExpansionPlayerMarkerData Marker;
	ref ExpansionPartyQuickMarkerData QuickMarker;

	ExpansionPartyPlayerPermissions Permissions;
	
	ref ExpansionPartyData m_Party;

	void ExpansionPartyPlayerData(ExpansionPartyData party)
	{
		m_TempMarkerData = new ExpansionPlayerMarkerData();
		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);
		CreateMarker();
		Permissions = ExpansionPartyPlayerPermissions.NONE;
		m_Party = party;
	}

	void ~ExpansionPartyPlayerData()
	{
		if ( m_TempMarkerData )
			delete m_TempMarkerData;

		if ( Marker )
			delete Marker;

		if ( QuickMarker )
			delete QuickMarker;
			
		GetExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
	}

	void InitMarker()
	{
		if (m_TempMarkerData)
		{
			m_TempMarkerData.SetUID(UID);
			m_TempMarkerData.SetIcon(ExpansionIcons.Get("Persona"));
			m_TempMarkerData.Set3D(true);
			if (GetExpansionClientSettings() && !GetExpansionClientSettings().ShowMemberNameMarker)
			{
				m_TempMarkerData.SetName("");
			} 
			else if (GetExpansionClientSettings() && GetExpansionClientSettings().ShowMemberNameMarker)
			{
				m_TempMarkerData.SetName(Name);
			}
			
			int markerColorA;
			int markerColorR;
			int markerColorG;
			int markerColorB;
			ExpansionStatic.IntToARGB(m_TempMarkerData.GetColor(), markerColorA, markerColorR, markerColorG, markerColorB);
			
			m_TempMarkerData.SetColor(ARGB(255, markerColorR, markerColorG, markerColorB));
		}
		else if (Marker)
		{
			if (GetExpansionClientSettings() && !GetExpansionClientSettings().ShowMemberNameMarker)
			{
				Marker.SetName("");
			} 
			else if (GetExpansionClientSettings() && GetExpansionClientSettings().ShowMemberNameMarker)
			{
				Marker.SetName(Name);
			}
		}
	}

	void CreateMarker()
	{
		int max = 255;
		int a = 255;
		int r = Math.RandomIntInclusive(0, max);
		int g = Math.RandomIntInclusive(0, max);
		int b = Math.RandomIntInclusive(0, max);
		m_TempMarkerData.SetColor(ARGB(a, r, g, b));

		InitMarker();
	}

	void OnLeave()
	{
		if (Marker)
		{
			m_TempMarkerData = ExpansionPlayerMarkerData.Cast(ExpansionMarkerData.Copy(Marker));

			delete Marker;
		}

		if (QuickMarker)
		{
			delete QuickMarker;
		}
	}

	void OnJoin(PlayerBase player)
	{
		Name = player.GetIdentityName();
		
		InitMarker();

		if (!Marker) // marker may already exist (respawn)
		{
			Marker = ExpansionPlayerMarkerData.Cast(ExpansionMarkerData.Copy(m_TempMarkerData));
		}
		
		Marker.UpdateObject(player);
	}

	void SetQuickMarker(vector position)
	{
		if (position != vector.Zero && !QuickMarker)
		{
			QuickMarker = new ExpansionPartyQuickMarkerData("QuickMarker" + UID);
		} else if (position == vector.Zero && QuickMarker || QuickMarker && vector.Distance(position, QuickMarker.GetPosition()) < 2.5)
		{
			delete QuickMarker;
		}

		if (QuickMarker)
		{
			QuickMarker.SetIcon(ExpansionIcons.Get("Map Marker"));
			QuickMarker.SetColor(m_TempMarkerData.GetColor());
			QuickMarker.SetPosition(position);
			QuickMarker.SetName(Name);
		}
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
            if (Expansion_Assert_False(ctx.Read(promoted), "[" + this + "] Failed reading Promoted"))
                return false;

            if (promoted)
                SetPermissions(ExpansionPartyPlayerPermissions.CAN_EDIT | ExpansionPartyPlayerPermissions.CAN_INVITE | ExpansionPartyPlayerPermissions.CAN_KICK | ExpansionPartyPlayerPermissions.CAN_DELETE | ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY);
        }
        else
        {
			 if (Expansion_Assert_False(ctx.Read(Permissions), "[" + this + "] Failed reading Permissions"))
            	return false;
        }
 
        if (version < 9)
        {
            CreateMarker();
        } else
        {
            m_TempMarkerData.OnStoreLoad(ctx, version);
        }
 
        InitMarker();
                
        return true;
    }

	void OnSettingChanged()
	{
		if (m_TempMarkerData)
			delete m_TempMarkerData;

		InitMarker();
	}
	
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

	ExpansionPartyPlayerPermissions ApplyPermissions(ExpansionPartyPlayerPermissions perm)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::ApplyPermissions - Apply Permissions: " + perm.ToString());
		#endif
		Permissions = perm;
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::ApplyPermissions - Permissions: " + Permissions.ToString());
		#endif
		return Permissions;
	}
	
	ExpansionPartyPlayerPermissions SetPermissions(ExpansionPartyPlayerPermissions perm)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::SetPermissions - Set Permissions: " + perm.ToString());
		#endif
		Permissions |= perm;
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::SetPermissions - Permissions: " + Permissions.ToString());
		#endif
		return Permissions;
	}
	
	ExpansionPartyPlayerPermissions RemovePermission(ExpansionPartyPlayerPermissions perm)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::RemovePermission - Remove Permissions: " + perm.ToString());
		#endif
		Permissions &= ~perm;
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyPlayerData::RemovePermission - Permissions: " + Permissions.ToString());
		#endif
		return Permissions;
	}
	
	ExpansionPartyPlayerPermissions GetPermissions()
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
	
#ifdef EXPANSIONMODMARKET	
	bool CanWithdrawMoney()
	{
		return (GetPermissions() & ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY) != 0;
	}
#endif
};