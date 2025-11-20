modded class TabberUI
{
	override int AddTab( string name )
	{
		//! Almost identical to vanilla AddTab, but we use our own tab control layout file just so title text size matches the rest of the tabs

		int new_index = m_Tabs.Count();
		Widget tab = g_Game.GetWorkspace().CreateWidgets( "gui/layouts/new_ui/tabber_prefab/tab.layout", m_Root );
		Widget control = g_Game.GetWorkspace().CreateWidgets( "DayZExpansion/Core/GUI/layouts/ui/options/tab_control.layout", m_Root.FindAnyWidget( "Tab_Control_Container" ) );
		TextWidget control_text = TextWidget.Cast( control.FindAnyWidget( "Tab_Control_x_Title" ) );
		
		tab.SetName( "Tab_" + new_index );
		control.SetName( "Tab_Control_" + new_index );
		control_text.SetName( "Tab_Control_" + new_index + "_Title" );
		control.FindAnyWidget( "Tab_Control_x_Background" ).SetName( "Tab_Control_" + new_index + "_Background" );
		
		control_text.SetText( name );
		
		control.SetHandler( this );
		m_TabControls.Insert( new_index, control );
		m_Tabs.Insert( new_index, tab );
		
		AlignTabbers();
		
		return new_index;
	}
}
