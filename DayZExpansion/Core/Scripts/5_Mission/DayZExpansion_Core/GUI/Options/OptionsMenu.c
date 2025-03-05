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
	
	void ~OptionsMenu()
	{
		if (m_ExpansionTab)
			delete m_ExpansionTab;
	}

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
	
	override void Refresh()
	{
		super.Refresh();

		GetDayZGame().Expansion_SetGameVersionText(m_Version);
	}
}
