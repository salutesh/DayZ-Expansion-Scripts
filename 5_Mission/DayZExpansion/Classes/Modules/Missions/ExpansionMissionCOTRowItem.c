class ExpansionMissionCOTRowItem extends ScriptedWidgetEventHandler 
{
	private Widget layoutRoot;

    private string m_MissionType;
    private ExpansionMissionSerializedEvent m_Mission;

	private TextWidget m_Name;
	private ButtonWidget m_Button;

	private ExpansionMissionCOTForm m_Form;

	private bool m_IsHeading;

	void OnWidgetScriptInit( Widget w )
	{
		layoutRoot = w;
		layoutRoot.SetHandler( this );
        
		Init();
	}

	void InitForm( ExpansionMissionCOTForm form ) 
	{
        m_Form = form;
	}

    string GetMissionName()
    {
        return m_Mission.m_MissionName;
    }

    ExpansionMissionSerializedEvent GetMission()
    {
        return m_Mission;
    }

    string GetMissionType()
    {
        return m_MissionType;
    }

	void RegisterHeading( string missionType )
	{
        if ( missionType == "" )
        {
            Hide();
            return;
        }

		m_IsHeading = true;

        m_MissionType = missionType;

        Show();

		m_Name.SetColor( 0xFFF5C987 );
		m_Name.SetBold( true );
        m_Name.SetText( m_MissionType );
		m_Name.SetTextOffset( 0, 0 );
	}

    void Register( string missionType, ExpansionMissionSerializedEvent evt )
    {
        if ( missionType == "" )
        {
            Hide();
            return;
        }

		m_IsHeading = false;

        Show();

        m_Mission = evt;
        m_MissionType = missionType;

        if ( !evt.m_IsEnabled )
        {
			m_Name.SetColor( 0xFFFC0303 );
        } else if ( evt.m_IsRunning )
        {
			m_Name.SetColor( 0xFF03FC2C );
        } else
        {
			m_Name.SetColor( 0xFFFFFFFF );
        }

        m_Name.SetText( "- " + m_Mission.m_MissionName );
		m_Name.SetTextOffset( 32, 0 );
    }

    void Init()
    {
        Class.CastTo( m_Name, layoutRoot.FindAnyWidget( "text_name" ) );
        Class.CastTo( m_Button, layoutRoot.FindAnyWidget( "button" ) );
    }

	void Show()
	{
		layoutRoot.Show( true );
		m_Button.Show( true );
		m_Button.Enable( !m_IsHeading );
		m_Name.Show( true );

		OnShow();
	}

	void Hide()
	{
		OnHide();

		m_Name.Show( false );
		m_Button.Show( false );
		layoutRoot.Show( false );
	}

	void OnShow()
	{
	}

	void OnHide() 
	{
	}

	void Update() 
	{
		
	}

	Widget GetLayoutRoot() 
	{
		return layoutRoot;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( super.OnClick( w, x, y, button ) )
		{  
            return true;
		}

		if ( w == m_Button && !m_IsHeading )
		{
            m_Form.SetSelectedMission( this );

			return true;
		}

		return false;
	}
}