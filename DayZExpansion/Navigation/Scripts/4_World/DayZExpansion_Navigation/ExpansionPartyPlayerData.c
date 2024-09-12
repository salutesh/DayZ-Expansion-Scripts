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

#ifdef EXPANSIONMODGROUPS
modded class ExpansionPartyPlayerData
{
	ref ExpansionPartyQuickMarkerData QuickMarker;
	
	void ExpansionPartyPlayerData(ExpansionPartyData party)
	{
		if (GetExpansionClientSettings())
			GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);
	}
	
	void ~ExpansionPartyPlayerData()
	{
		if ( QuickMarker )
			delete QuickMarker;
			
		if (GetGame() && GetExpansionClientSettings())
			GetExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
	}
	
	void InitMarker()
	{
		if (m_TempMarkerData)
		{
			m_TempMarkerData.SetUID(UID);
		}
	}

	void CreateMarker()
	{
		Error("DEPRECATED");
	}
	
	override void OnLeave()
	{
		super.OnLeave();
		
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
	
	override void OnJoin(PlayerBase player)
	{
		super.OnJoin(player);
	
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
		}
		else if (position == vector.Zero && QuickMarker || QuickMarker && vector.Distance(position, QuickMarker.GetPosition()) < 2.5)
		{
			delete QuickMarker;
		}

		if (QuickMarker)
		{
			QuickMarker.SetColor(m_TempMarkerData.GetColor());
			QuickMarker.SetPosition(position);
		}
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
     	if (!super.OnStoreLoad(ctx, version))
			return false;

        InitMarker();
                
        return true;
    }
}
#endif