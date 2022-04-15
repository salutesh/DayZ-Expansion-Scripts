/**
 * ModsMenuSimple.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionModsMenuSimple extends ScriptedWidgetEventHandler
{	
	protected Widget													m_Root;
	protected Widget													m_MoreButton;
	protected ref map<ModInfo, ref ExpansionModsMenuSimpleEntry>		m_Data;
	protected ModsMenuDetailed											m_DetailMenu;
	ref MainMenu 														m_MainMenu;
	ref ExpansionNewsfeed 												m_ExpansionNewsfeed;

	void ExpansionModsMenuSimple(array<ref ModInfo> data, Widget parent, ModsMenuDetailed detail_menu)
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/mods_menu/mods_menu_simple.layout", parent);
		m_MoreButton = m_Root.FindAnyWidget("ModMore");
		m_Data = new map<ModInfo, ref ExpansionModsMenuSimpleEntry>;
		m_DetailMenu = detail_menu;
		
		m_Root.SetHandler(this);
		LoadEntries(data);
	}
	
	void ~ExpansionModsMenuSimple()
	{
		delete m_Root;
	}

	void LoadEntries(array<ref ModInfo> data)
	{
		int count;
		if( data.Count() > 13 )
		{
			count = 13;
			m_Root.FindAnyWidget("ModMore").Show(true);
		}
		else
		{
			count = data.Count();
			m_Root.FindAnyWidget("ModMore").Show(false);
		}
		
		bool has_mods = false;
		for (int i = count - 1; i >= 0; i--)
		{
			ExpansionModsMenuSimpleEntry entry = new ExpansionModsMenuSimpleEntry(data.Get(i), i, m_Root, this);
			m_Data.Insert(data.Get(i), entry);
			has_mods = ( has_mods || !data.Get(i).GetIsDLC() );
		}
		
		m_Root.FindAnyWidget("mods_separator_panel").Show( has_mods );
	}
	
	void Select( ModInfo mod )
	{
		m_DetailMenu.Open();
		m_DetailMenu.Highlight( mod );
		
		m_MainMenu.ShowNewsfeed(false);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if( w == m_MoreButton )
		{
			if( m_DetailMenu.IsOpen() )
				m_DetailMenu.Close();
			else
				m_DetailMenu.Open();
			return true;
		}
		return false;
	}
	
	void SetMainMenu(MainMenu menu)
	{
		m_MainMenu = menu;
	}
}

class ExpansionModsMenuSimpleEntry extends ScriptedWidgetEventHandler
{
	protected ButtonWidget				m_ModButton;
	protected ImageWidget				m_Icon;
	protected Widget					m_Hover;

	protected bool						m_HasLogoOver;
	protected ModInfo					m_Data;
	protected ExpansionModsMenuSimple	m_ParentMenu;

	void ExpansionModsMenuSimpleEntry(ModInfo data, int index, Widget parent, ExpansionModsMenuSimple parent_menu)
	{
		m_ModButton		= ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/mods_menu/mods_menu_simple_entry.layout", parent));
		m_Icon			= ImageWidget.Cast(m_ModButton.FindAnyWidget("Icon"));
		m_Hover			= m_ModButton.FindAnyWidget("Overlay");
		m_Data			= data;
		m_ParentMenu	= parent_menu;
		
		if( data.GetIsDLC() )
		{
			m_ModButton.SetSort( index );
			m_ModButton.FindAnyWidget("ModOwnership").Show( true );
			m_ModButton.FindAnyWidget("Owned").Show( data.GetIsOwned() );
			m_ModButton.FindAnyWidget("Unowned").Show( !data.GetIsOwned() );
		}
		else
		{
			m_ModButton.SetSort( index + 5 );
		}
		
		
		m_ModButton.SetHandler(this);

		LoadData();
	}
	
	void ~ExpansionModsMenuSimpleEntry()
	{
		delete m_ModButton;
	}

	void LoadData()
	{
		string logo = m_Data.GetLogo();
		string logo_over = m_Data.GetLogoOver();
		
		if (logo != "")
		{
			m_Icon.LoadImageFile(0, logo);
		}
		else
		{
			//Load default image
		}
		
		if (logo_over != "")
		{
			m_HasLogoOver = true;
			m_Icon.LoadImageFile(1, logo_over);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if( w == m_ModButton )
		{
			m_ParentMenu.Select( m_Data );
			return true;
		}
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if( w == m_ModButton )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 1 );
			m_Hover.Show( true );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if( enterW != m_ModButton )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 0 );
			m_Hover.Show( false );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( w == m_ModButton )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 1 );
			m_Hover.Show( true );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( w == m_ModButton )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 0 );
			m_Hover.Show( false );
			return true;
		}
		return false;
	}
}