/**
 * OptionsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class OptionsMenu
{
	protected ref OptionsMenuExpansion m_ExpansionTab;
	
	override Widget Init()
	{
		layoutRoot = super.Init();
		
		int expansionTabIndex = m_Tabber.AddTab("EXPANSION");
		m_ExpansionTab = new OptionsMenuExpansion(layoutRoot.FindAnyWidget("Tab_" + expansionTabIndex), m_Details, m_Options, this);
		
		return layoutRoot;
	}
	
	override void OnChanged()
	{
		super.OnChanged();

		if ( m_ExpansionTab && m_ExpansionTab.IsChanged() )
		{
			m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
			ColorNormal( m_Apply );
		}
	}
	
	override void Apply()
	{
		super.Apply();

		if( m_ExpansionTab && m_ExpansionTab.IsChanged() )
			m_ExpansionTab.Apply();
	}
	
	protected void SetGameVersion()
	{
		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("version") );

		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		} else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
	}
	
	override void Refresh()
	{
		super.Refresh();

		SetGameVersion();
	}
}
