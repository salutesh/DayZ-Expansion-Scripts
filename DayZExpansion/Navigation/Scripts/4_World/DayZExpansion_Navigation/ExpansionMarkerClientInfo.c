/**
 * ExpansionMarkerClientInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarkerClientInfo
{
	private string m_UID;
	private int m_Visibility;

	void ExpansionMarkerClientInfo( string uid = "" )
	{
		m_UID = uid;

		SetVisibility( EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write( m_UID );
		ctx.Write( m_Visibility );
	}

	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( ctx.Read( m_UID ), "~m_UID failed" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Visibility ), "~m_Visibility failed" ) )
			return false;
		
		return true;
	}

	void SetUID( string uid )
	{
		m_UID = uid;
	}

	string GetUID()
	{
		return m_UID;
	}

	int SetVisibility( int vis )
	{
		m_Visibility |= vis;
		return m_Visibility;
	}

	int FlipVisibility( int vis )
	{
		if ( ( m_Visibility & vis ) != 0 )
			m_Visibility &= ~vis;
		else
			m_Visibility |= vis;

		return m_Visibility;
	}

	int RemoveVisibility( int vis )
	{
		m_Visibility &= ~vis;
		return m_Visibility;
	}

	int ClearVisibility()
	{
		m_Visibility = 0;
		return m_Visibility;
	}

	int GetVisibility()
	{
		return m_Visibility;
	}

	bool IsVisible()
	{
		return m_Visibility != 0;
	}

	bool IsWorldVisible()
	{
		return (m_Visibility & EXPANSION_MARKER_VIS_WORLD) != 0;
	}

	bool IsMapVisible()
	{
		return (m_Visibility & EXPANSION_MARKER_VIS_MAP) != 0;
	}
};