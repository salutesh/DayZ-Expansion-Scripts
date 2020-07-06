/**
 * ExpansionMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarker
{
	protected bool m_IsPartyMarker;
	protected bool m_Is3DMarker;
	protected string m_Text;
	protected int m_IconIndex;
	protected int m_Color;
	protected vector m_Position;

	void ExpansionMapMarker(string text, int iconIndex, vector position, int color, bool partyMarker, bool marker3d = false )
	{
		m_Text = text;
		m_IconIndex = iconIndex;
		m_Position = position;
		m_Color = color;
		m_IsPartyMarker = partyMarker;
		m_Is3DMarker = marker3d;
	}

	string GetMarkerText()
	{
		return m_Text;
	}

	vector GetMarkerPosition()
	{
		return m_Position;
	}

	int GetIconIndex()
	{
		return m_IconIndex;
	}
	
	int GetMarkerColor()
	{
		return m_Color;
	}

	bool IsPartyMarker()
	{
		return m_IsPartyMarker;
	}

	bool Is3DMarker()
	{
		return m_Is3DMarker;
	}
	
	void SetPosition(vector pos)
	{
		m_Position = pos;
	}
	
	void OnStoreSave(ParamsWriteContext ctx)
	{
		ctx.Write( m_IsPartyMarker );
		ctx.Write( m_Is3DMarker );
		ctx.Write( m_Text );
		ctx.Write( m_IconIndex );
		ctx.Write( m_Color );
		ctx.Write( m_Position );
	}
	
	bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!ctx.Read(m_IsPartyMarker))
			return false;
		
		if (!ctx.Read(m_Is3DMarker))
			return false;
		
		if (!ctx.Read(m_Text))
			return false;
		
		if (!ctx.Read(m_IconIndex))
			return false;
		
		if (!ctx.Read(m_Color))
			return false;
		
		if (!ctx.Read(m_Position))
			return false;
		
		return true;
	}
}