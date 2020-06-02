/**
 * ExpansionCrewData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCrewData
{
	private ExpansionVehicleScript m_Vehicle;
	private string m_Name;

	private int m_ComponentIndex;
	private string m_ActionSelection;
	private bool m_IsDriver;

	private vector m_GetInPos;
	private vector m_GetInDir;

	private DayZPlayerImplement m_Player;

	private vector m_SeatTransform[4];

	void ExpansionCrewData( ExpansionVehicleScript vehicle, string name )
	{
		m_Vehicle = vehicle;
		m_Name = name;

		string getinpos_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " getInPos";
		m_GetInPos = m_Vehicle.GetSelectionPositionMS( GetGame().ConfigGetTextOut( getinpos_path ) );

		string getindir_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " getInDir";
		m_GetInDir = m_Vehicle.GetSelectionPositionMS( GetGame().ConfigGetTextOut( getindir_path ) );

		string action_selection_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " actionSel";
		m_ActionSelection = GetGame().ConfigGetTextOut( action_selection_path );
		m_ActionSelection.ToLower();

		m_ComponentIndex = -1;

		TStringArray selections = new TStringArray;
		m_Vehicle.GetSelectionList( selections );
		for ( int i = 0; i < selections.Count(); ++i )
		{
			string sel = selections.Get( i );
			sel.ToLower();
			if ( sel == m_ActionSelection )
			{
				m_ComponentIndex = i;
				break;
			}
		}

		string is_driver_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " isDriver";
		m_IsDriver = GetGame().ConfigGetInt( is_driver_path ) == 1;

		string proxy_pos_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " proxyPos";
		string proxyPos = GetGame().ConfigGetTextOut( proxy_pos_path );

		m_SeatTransform[0] = Vector( 1, 0, 0 );
		m_SeatTransform[1] = Vector( 0, 1, 0 );
		m_SeatTransform[2] = Vector( 0, 0, 1 );

		LOD lod = m_Vehicle.GetLODByName( "geometryView" );
		Selection selection = lod.GetSelectionByName( proxyPos );
		if ( selection )
		{
			m_SeatTransform[3] = selection.GetVertexPosition( lod, 0 );
		} else
		{
			m_SeatTransform[3] = "0 0 0";
		}
	}

	string GetName()
	{
		return m_Name;
	}

	ExpansionVehicleScript GetVehicle()
	{
		return m_Vehicle;
	}

	int GetComponentIndex()
	{
		return m_ComponentIndex;
	}

	string GetComponentSelection()
	{
		return m_ActionSelection;
	}

	bool IsDriver()
	{
		return m_IsDriver;
	}

	void GetIn( out vector pos, out vector dir )
	{
		pos = m_GetInPos;
		dir = m_GetInDir;
	}

	void GetTransform( out vector trans[4] )
	{
		trans[0] = m_SeatTransform[0];
		trans[1] = m_SeatTransform[1];
		trans[2] = m_SeatTransform[2];
		trans[3] = m_SeatTransform[3];
	}

	void SetPlayer( DayZPlayerImplement player )
	{
		m_Player = player;
	}

	DayZPlayerImplement GetPlayer()
	{
		return m_Player;
	}
}