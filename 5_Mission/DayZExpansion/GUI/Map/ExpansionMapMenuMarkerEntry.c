/**
 * ExpansionMapMenuMarkerEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuMarkerEntry extends ScriptedWidgetEventHandler
{
	protected GridSpacerWidget					m_Root;
	protected ButtonWidget						m_Button;
	protected ImageWidget						m_Icon;
	protected TextWidget						m_Name;

	protected string 							m_MarkerName;
	protected string							m_IconPath;
	protected ExpansionMapMenuMarkerWindow		m_MarkerWindow;
	protected ExpansionMarkerIcon				m_MarkerData;
	protected int								m_MarkerIconIndex;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarkerEntry Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuMarkerEntry(Widget parent, ExpansionMarkerIcon data, ExpansionMapMenuMarkerWindow marker_window)
	{
		m_Root			= GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/map/expansion_marker_entry.layout", parent ) );
		m_Button		= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_entry_button") );
		m_Icon			= ImageWidget.Cast( m_Root.FindAnyWidget("marker_icon") );
		m_Name			= TextWidget.Cast( m_Root.FindAnyWidget("marker_name") );
		m_MarkerData	= data;
		m_MarkerWindow	= marker_window;

		if (m_MarkerData)
			m_MarkerIconIndex = ExpansionMarkerIcons.GetMarkerIndex(m_MarkerData.Path);
		
		m_Button.SetHandler(this);

		SetMarker();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarkerEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuMarkerEntry()
	{
		delete m_Root;
	}

	// ------------------------------------------------------------
	// Expansion GetMarkerData
	// ------------------------------------------------------------
	ExpansionMarkerIcon GetMarkerData()
	{
		return m_MarkerData;
	}
	
	// ------------------------------------------------------------
	// Expansion SetMarker
	// ------------------------------------------------------------
	void SetMarker()
	{
		string icon = GetMarkerData().Path;
		string name = GetMarkerData().Name;

		if (icon != "")
		{
			m_Icon.LoadImageFile(0, icon);
			m_Icon.Show(true);
			m_IconPath = icon;

			m_MarkerIconIndex = ExpansionMarkerIcons.GetMarkerIndex(icon);
		}

		if (name != "")
		{
			m_Name.SetText(name);
			m_MarkerName = name;
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMapMenuMarkerEntry::SetMarker - Created marker entry for marker: " + name + " with Index: " + m_MarkerIconIndex.ToString() );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerName
	// ------------------------------------------------------------
	string GetMarkerName()
	{
		return m_MarkerName;
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerIcon
	// ------------------------------------------------------------
	string GetMarkerIcon()
	{
		return m_IconPath;
	}

	// ------------------------------------------------------------
	// Expansion GetMarkerIconIndex
	// ------------------------------------------------------------
	int GetMarkerIconIndex()
	{
		return m_MarkerIconIndex;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonUp
	// ------------------------------------------------------------
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if(w == m_Button)
		{
			m_MarkerWindow.SetMarkerSelection( this );
			return true;
		}
		return false;
	}
}