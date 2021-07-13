/**
 * ExpansionPartyPlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyPlayerData
{
	string UID;
	string Name;
	bool Promoted;

	PlayerBase Player;

	ref ExpansionPlayerMarkerData m_TempMarkerData;

	ref ExpansionPlayerMarkerData Marker;
	ref ExpansionPartyQuickMarkerData QuickMarker;

	void ExpansionPartyPlayerData()
	{
		m_TempMarkerData = new ExpansionPlayerMarkerData();
		GetExpansionClientSettings().SI_UpdateSetting.Insert( OnSettingChanged );

		CreateMarker();
	}

	void ~ExpansionPartyPlayerData()
	{
		if ( m_TempMarkerData )
			delete m_TempMarkerData;

		if ( Marker )
			delete Marker;

		if ( QuickMarker )
			delete QuickMarker;
			
		GetExpansionClientSettings().SI_UpdateSetting.Remove( OnSettingChanged );
	}

	void InitMarker()
	{
		m_TempMarkerData.SetUID( UID );
		m_TempMarkerData.SetIcon( ExpansionIcons.Get( "Persona" ) );
		m_TempMarkerData.Set3D( true );
		if( !GetExpansionClientSettings().ShowMemberNameMarker )
		{
			m_TempMarkerData.SetName( "" );
		} else {
			m_TempMarkerData.SetName( Name );
		}

		int markerColorA;
		int markerColorR;
		int markerColorG;
		int markerColorB;
		IntToARGB( m_TempMarkerData.GetColor(), markerColorA, markerColorR, markerColorG, markerColorB );
		
		m_TempMarkerData.SetColor( ARGB( 255, markerColorR, markerColorG, markerColorB ) );
	}

	void CreateMarker()
	{
		int max = 255;
		int a = 255;
		int r = Math.RandomIntInclusive( 0, max );
		int g = Math.RandomIntInclusive( 0, max );
		int b = Math.RandomIntInclusive( 0, max );
		m_TempMarkerData.SetColor( ARGB( a, r, g, b ) );

		InitMarker();
	}

	void OnLeave()
	{
		if ( Marker )
		{
			m_TempMarkerData = ExpansionPlayerMarkerData.Cast( ExpansionMarkerData.Copy( Marker ) );

			delete Marker;
		}

		if ( QuickMarker )
		{
			delete QuickMarker;
		}
	}

	void OnJoin( PlayerBase player )
	{
		Name = player.GetIdentityName();

		InitMarker();

		if ( !Marker ) // marker may already exist (respawn)
		{
			Marker = ExpansionPlayerMarkerData.Cast(ExpansionMarkerData.Copy( m_TempMarkerData ) );
		}
		
		Marker.UpdateObject( player );
	}

	void SetQuickMarker( vector position )
	{
		if ( position != vector.Zero && !QuickMarker )
		{
			QuickMarker = new ExpansionPartyQuickMarkerData( "QuickMarker" + UID );
		} else if ( position == vector.Zero && QuickMarker || QuickMarker && vector.Distance(position, QuickMarker.GetPosition()) < 2.5 )
		{
			delete QuickMarker;
		}

		if ( QuickMarker )
		{
			QuickMarker.SetIcon( ExpansionIcons.Get( "Map Marker" ) );
			QuickMarker.SetColor( m_TempMarkerData.GetColor() );
			QuickMarker.SetPosition( position );
			QuickMarker.SetName( Name );
		}
	}
	
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write( UID );
		ctx.Write( Name );
		ctx.Write( Promoted );

		if ( Marker )
		{
			m_TempMarkerData = ExpansionPlayerMarkerData.Cast( ExpansionMarkerData.Copy( Marker ) );

			Marker.OnStoreSave( ctx );
		} else
		{
			m_TempMarkerData.OnStoreSave( ctx );
		}
	}
	
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( ctx.Read( UID ), "[" + this + "] Failed reading UID" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( Name ), "[" + this + "] Failed reading Name" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( Promoted ), "[" + this + "] Failed reading Promoted" ) )
			return false;

		if ( version < 9 )
		{
			CreateMarker();
		} else
		{
			m_TempMarkerData.OnStoreLoad( ctx, version );
		}

		InitMarker();
		
		return true;
	}

	void OnSettingChanged()
	{
		if ( m_TempMarkerData )
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
};