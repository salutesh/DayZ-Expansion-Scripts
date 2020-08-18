class ExpansionMapWidgetBase extends ScriptedWidgetEventHandler
{
	reference string MapWidgetName;

	private MapWidget m_MapWidget;

	private Widget m_LayoutRoot;

	private vector m_WorldPosition;

	private bool m_Dragging;
	private float m_DragOffsetX;
	private float m_DragOffsetY;

	private bool m_Initialized;

/*
	void OnWidgetScriptInit( Widget w )
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler( this );

		Class.CastTo( m_MapWidget, m_LayoutRoot.FindAnyWidget( MapWidgetName ) );

		OnInit( m_LayoutRoot );
	}
*/

	void ExpansionMapWidgetBase( Widget parent, MapWidget mapWidget, bool autoInit = true )
	{
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets( GetLayoutPath(), parent );
		m_MapWidget = mapWidget;
		if ( autoInit )
			OnInit( m_LayoutRoot );
		
		m_LayoutRoot.SetHandler( this );
	}

	void ~ExpansionMapWidgetBase()
	{
		if( m_LayoutRoot )
			m_LayoutRoot.Unlink();
		 //delete m_LayoutRoot;
	}

	bool HasBeenInitialized()
	{
		return m_Initialized;
	}

	void Init()
	{
		m_Initialized = true;
		OnInit( m_LayoutRoot );
	}

	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}

	MapWidget GetMapWidget()
	{
		return m_MapWidget;
	}

	string GetLayoutPath()
	{
		return "DayZExpansion/GUI/layouts/map/expansion_map_marker.layout";
	}

	string GetDropSoundEffect()
	{
		return "Expansion_Draws_SoundSet";
	}

	bool IsEditButton( Widget w )
	{
		return false;
	}

	bool IsDragWidget( Widget w )
	{
		return false;
	}

	bool IsDragging()
	{
		return m_Dragging;
	}

	protected void OnInit( Widget layoutRoot ) 
	{
	}

	void Update( float pDt )
	{
		if ( !m_Dragging )
		{
			vector position = m_MapWidget.MapToScreen(m_WorldPosition);
			m_LayoutRoot.SetPos( position[0], position[1], true );
		}
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w != NULL && IsEditButton( w ) )
		{
			OnEdit();
			
			return true;
		}

		return false;
	}
	
	override bool OnDrag( Widget w, int x, int y )
	{
		if ( w != NULL && IsDragWidget( w ) )
		{
			m_LayoutRoot.GetPos( m_DragOffsetX, m_DragOffsetY );

			m_DragOffsetX = x - m_DragOffsetX;
			m_DragOffsetY = y - m_DragOffsetY;
			
			m_Dragging = true;
			
			return true;
		}

		return false;
	}
	
	override bool OnDragging( Widget w, int x, int y, Widget reciever )
	{
		if ( w != NULL && IsDragWidget( w ) )
		{			
			SetPosition( x - m_DragOffsetX, y - m_DragOffsetY );

			return true;
		}

		return false;
	}
	
	override bool OnDrop( Widget w, int x, int y, Widget reciever )
	{
		if ( w != NULL && IsDragWidget( w ) )
		{			
			SetPosition( x - m_DragOffsetX, y - m_DragOffsetY, true );
			
			m_Dragging = false;
			
			return true;
		}

		return false;
	}

	void SetPosition( int x, int y, bool performDropEvent = false )
	{
		m_LayoutRoot.SetPos( x, y, true );
		float previousY = m_WorldPosition[1];
		vector newPos = m_MapWidget.ScreenToMap( Vector( x, y, 0 ) );

		SetPosition( newPos, performDropEvent );
	}

	void SetPosition( vector position, bool performDropEvent = false )
	{
		m_WorldPosition = position;

		vector mPos = m_MapWidget.MapToScreen( m_WorldPosition );
		m_LayoutRoot.SetPos( mPos[0], mPos[1], true );

		if ( performDropEvent )
		{
			OnDrop();
		}
	}

	vector GetPosition()
	{
		return m_WorldPosition;
	}

	void OnEdit()
	{
		
	}

	void OnDrop()
	{
		if ( IsMissionClient() && GetDropSoundEffect() != "" )
		{
			SEffectManager.PlaySound( GetDropSoundEffect(), GetGame().GetPlayer().GetPosition() );
		}
	}
	
	void Show()
	{
		m_LayoutRoot.Show( true );
	}
	
	void Hide()
	{
		m_LayoutRoot.Show( false );
	}
};