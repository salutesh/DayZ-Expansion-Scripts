class ExpansionCOTFormBase : JMFormBase
{
	private UIActionScroller m_ActionListScroller;
	private Widget m_ActionListContainer;

	private ref array< UIActionBase > m_Actions;

	private ExpansionCOTModuleBase m_ExpModule;

	void ExpansionCOTFormBase()
	{
		m_Actions = new array< UIActionBase >();
	}

	void ~ExpansionCOTFormBase()
	{
		delete m_Actions;
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		if ( Class.CastTo( m_ExpModule, mdl ) )
		{
			return SetExpansionModule( m_ExpModule );
		}

		return false;
	}

	override void OnInit()
	{
		m_ActionListScroller			= UIActionManager.CreateScroller( layoutRoot.FindAnyWidget( "panel_top" ) );
		m_ActionListScroller.SetFixedHeight( 300 - 30 );

		Widget actions					= m_ActionListScroller.GetContentWidget();

		array< ref ExpansionSettingSerializationBase > settings = m_ExpModule.GetSettings();

		for ( int i = 0; i < settings.Count(); ++i )
		{
			if ( !CreateToggle( actions, settings[i] ) )
				if ( !CreateSlider( actions, settings[i] ) )
					if ( !CreateDropdown( actions, settings[i] ) )
						Print( "Unknown/unsupported action type -> [" + i + "] " + settings[i] );

			UIActionManager.CreatePanel( actions, 0xAA000000, 1 );
		}

		m_ActionListScroller.UpdateScroller();

		Widget admin_actions			= UIActionManager.CreateGridSpacer( layoutRoot.FindAnyWidget( "panel_bottom" ), 1, 2 );
		UIActionManager.CreateButton( admin_actions, "Refresh", this, "Click_Refresh" );
		UIActionManager.CreateButton( admin_actions, "Apply", this, "Click_Apply" );
	}

	private bool CreateToggle( Widget actions, ref ExpansionSettingSerializationBase setting_base )
	{
		ExpansionSettingSerializationToggle setting;
		if ( !Class.CastTo( setting, setting_base ) )
			return false;

		UIActionCheckbox action = UIActionManager.CreateCheckbox( actions, setting.m_Name, this, "OnActionUpdated", setting.GetValue() );
		action.SetData( new ExpansionCOTSerializationData( setting ) );

		setting.m_WidgetHandle = action;

		m_Actions.Insert( action );

		return true;
	}

	private bool CreateSlider( Widget actions, ref ExpansionSettingSerializationBase setting_base )
	{
		ExpansionSettingSerializationSlider setting;
		if ( !Class.CastTo( setting, setting_base ) )
			return false;

		UIActionSlider action = UIActionManager.CreateSlider( actions, setting.m_Name, setting.m_Min, setting.m_Max, this, "OnActionUpdated" );
		action.SetData( new ExpansionCOTSerializationData( setting ) );
		action.SetCurrent( setting.GetValue() );
		action.SetStepValue( setting.m_Step );

		setting.m_WidgetHandle = action;

		m_Actions.Insert( action );

		return true;
	}

	private bool CreateDropdown( Widget actions, ref ExpansionSettingSerializationBase setting_base )
	{
		ExpansionSettingSerializationEnum setting;
		if ( !Class.CastTo( setting, setting_base ) )
			return false;

		UIActionDropdownList action = UIActionManager.CreateDropdownBox( actions, GetLayoutRoot(), setting.m_Name, setting.m_Values, this, "OnActionUpdated" );
		action.SetData( new ExpansionCOTSerializationData( setting ) );
		action.SetText( setting.m_Values[ setting.GetValue() ] );

		setting.m_WidgetHandle = action;

		m_Actions.Insert( action );

		return true;
	}

	void OnActionUpdated( UIEvent id, ref UIActionBase action )
	{
		ExpansionSettingSerializationBase setting_base = ExpansionCOTSerializationData.Cast( action.GetData() ).Get();

		ExpansionSettingSerializationToggle setting_toggle;
		if ( Class.CastTo( setting_toggle, setting_base ) )
		{
			setting_toggle.SetTempValue( action.IsChecked() );
			return;
		}

		ExpansionSettingSerializationSlider setting_slider;
		if ( Class.CastTo( setting_slider, setting_base ) )
		{
			setting_slider.SetTempValue( action.GetCurrent() );
			return;
		}

		ExpansionSettingSerializationEnum setting_enum;
		if ( Class.CastTo( setting_enum, setting_base ) )
		{
			setting_enum.SetTempValue( action.GetText() );
			return;
		}
	}
	
	void Click_Refresh( UIEvent id, ref UIActionBase action )
	{
		OnSettingsUpdated();
	}
	
	void Click_Apply( UIEvent id, ref UIActionBase action )
	{
		m_ExpModule.Update();
	}

	override void OnSettingsUpdated()
	{
		super.OnSettingsUpdated();

		for ( int i = 0; i < m_Actions.Count(); ++i )
		{
			ExpansionSettingSerializationBase setting_base = ExpansionCOTSerializationData.Cast( m_Actions[i].GetData() ).Get();

			ExpansionSettingSerializationToggle setting_toggle;
			if ( Class.CastTo( setting_toggle, setting_base ) )
			{
				m_Actions[i].SetChecked( setting_toggle.GetValue() );
				continue;
			}

			ExpansionSettingSerializationSlider setting_slider;
			if ( Class.CastTo( setting_slider, setting_base ) )
			{
				m_Actions[i].SetCurrent( setting_slider.GetValue() );
				continue;
			}

			ExpansionSettingSerializationEnum setting_enum;
			if ( Class.CastTo( setting_enum, setting_base ) )
			{
				m_Actions[i].SetText( setting_enum.m_Values[ setting_enum.GetValue() ] );
				continue;
			}
		}
	}

	bool SetExpansionModule( ref ExpansionCOTModuleBase mdl )
	{
		return false;
	}

};