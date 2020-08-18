class ExpansionDropdownPrefab extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected WrapSpacerWidget		m_Wraper;
	protected ScrollWidget			m_Scroller;
	protected Widget				m_ContentContainer;
	protected ref array<Widget>		m_Content		= new array<Widget>>;
	
	protected Widget				m_Button;
	protected TextWidget			m_Text;
	protected ImageWidget			m_ImageExpand;
	protected ImageWidget			m_ImageCollapse;
	
	protected bool					m_IsExpanded;
	ref ScriptInvoker				m_OnSelectItem	= new ScriptInvoker();

	void ExpansionDropdownPrefab( Widget root, string text = "" )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "DayzExpansion/GUI/layouts/ui/expansion_dropdown_prefab.layout", root );
		
		m_Wraper			= WrapSpacerWidget.Cast( m_Root.FindAnyWidget( "dropdown_wraper" ) );
		m_Scroller			= ScrollWidget.Cast( m_Root.FindAnyWidget( "dropdown_scroller" ) );
		m_ContentContainer	= m_Root.FindAnyWidget( "dropdown_content" );
		m_Text				= TextWidget.Cast( m_Root.FindAnyWidget( "dropdown_text" ) );
		SetText( text );
		
		m_Button			= m_Root.FindAnyWidget( "dropdown_selector_button" );
		m_ImageExpand		= ImageWidget.Cast( m_Root.FindAnyWidget( "expand_image" ) );
		m_ImageCollapse		= ImageWidget.Cast( m_Root.FindAnyWidget( "collapse_image" ) );
		
		m_Root.SetHandler( this );
		
		RefreshContent();
	}
	
	void RefreshContent()
	{
		Widget child = m_ContentContainer.GetChildren();
		while( child )
		{
			if( m_Content.Find( child ) > -1 )
			{
				m_Content.Insert( child );
			}
		}
		
		m_ContentContainer.Update();
		m_Root.Update();
		
		float x, y;
		m_ContentContainer.GetScreenSize( x, y );
		if( y > m_Scroller.GetContentHeight() )
		{
			m_Scroller.SetAlpha( 1 );
		}
		else
		{
			m_Scroller.SetAlpha( 0 );
		}
	}
	
	int AddElement( string text, Widget content = null )
	{
		ButtonWidget element = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets( "DayzExpansion/GUI/layouts/ui/expansion_dropdown_element.layout", m_ContentContainer));
		TextWidget text_widget = TextWidget.Cast(element.FindAnyWidget("text"));
		text_widget.SetText( text );
		
		if( content )
		{
			element.AddChild( content, false );
		}
		m_ContentContainer.Update();
		m_Root.Update();
		
		m_Content.Insert( element );
		return m_Content.Count() - 1;
	}
	
	void RemoveElement( int index )
	{
		if( 0 < index && index < m_Content.Count() )
		{
			delete m_Content.Get( index );
			m_ContentContainer.Update();
			m_Root.Update();
		}
	}
	
	void Close()
	{
		if( m_IsExpanded )
		{
			m_Wraper.Show( false );
			m_Scroller.Show( false );
			m_ImageExpand.Show( false );
			m_ImageCollapse.Show( true );
		}
	}
	
	void SetText( string text )
	{
		m_Text.SetText( text );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		int index = m_Content.Find( w );
		if( index > -1 )
		{
			m_OnSelectItem.Invoke( index );
			m_IsExpanded = false;
			m_Wraper.Show( m_IsExpanded );
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( w == m_Root && button == MouseState.LEFT )
		{
			m_IsExpanded = !m_IsExpanded;
			m_Wraper.Show( m_IsExpanded );
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			
			m_Root.Update();
			return true;
		}
		return false;
	}
	
	bool IsExpanded()
	{
		return m_IsExpanded;
	}
}