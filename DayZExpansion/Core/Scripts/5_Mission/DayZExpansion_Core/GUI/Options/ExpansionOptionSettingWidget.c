/**
 * ExpansionOptionSettingWidget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionOptionSettingWidget: ScriptedWidgetEventHandler
{
	protected Widget layoutRoot;
	protected Widget selectWidget;
	protected Widget optionWidget;
	protected TextWidget settingLabel;
	protected ButtonWidget settingButton;
	protected TextWidget settingValue;

	protected ExpansionSettingSerializationBase m_Setting;

	protected ref OptionsMenuExpansion m_Menu;
	protected ref OptionSelectorMultistate m_Selector;
	protected ref OptionSelectorSlider m_Slider;
	protected ref ScriptInvoker m_ApplySetting = new ScriptInvoker;
		
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget Constructor
	
	// -----------------------------------------------------------
	void ExpansionOptionSettingWidget( Widget parent, notnull ExpansionSettingSerializationBase setting, OptionsMenuExpansion menu )
	{
		layoutRoot = Widget.Cast( g_Game.GetWorkspace().CreateWidgets( "DayZExpansion/Core/GUI/layouts/ui/options/expansion_options_setting_new.layout", parent ) );
		
		m_Setting = setting;
		m_Menu = menu;

		selectWidget = Widget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting" ) );
		settingLabel = TextWidget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_label" ) );
		settingLabel.SetText( m_Setting.m_Name );
		optionWidget = Widget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_option" ) );
		settingButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_button" ) );
		settingValue = TextWidget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_value" ) );
		settingValue.Show( false );
		
		layoutRoot.SetHandler( this );

		if ( !ConstructToggle() && !ConstructSlider() && !ConstructEnum() )
		{
			Error( "ExpansionOptionSettingWidget construction failed!" );
		}
	}

	void ~ExpansionOptionSettingWidget()
	{
		m_Menu = null;
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget Apply
	// -----------------------------------------------------------
	void Apply()
	{
		m_ApplySetting.Invoke();
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget ConstructToggle
	// -----------------------------------------------------------
	bool ConstructToggle()
	{
		ExpansionSettingSerializationToggle setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return false;

		int selection = 0;
		if ( setting.GetValue() )
			selection = 1;

		array<string> options = { "NO", "YES" };
		m_Selector = new OptionSelectorMultistate( optionWidget, selection, this, false, options );
		m_Selector.m_OptionChanged.Insert( OnToggleOptionChanged );

		m_ApplySetting.Insert( OnApplySetting_Toggle );

		return true;
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnToggleOptionChanged
	// -----------------------------------------------------------
	private void OnToggleOptionChanged( int selection )
	{
		m_Menu.SetChanged();
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnApplySetting_Toggle
	// -----------------------------------------------------------
	private void OnApplySetting_Toggle()
	{
		ExpansionSettingSerializationToggle setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return;

		if ( m_Selector.GetValue() == 1 )
			setting.SetValue( true );
		else
			setting.SetValue( false );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget ConstructSlider
	// -----------------------------------------------------------
	bool ConstructSlider()
	{
		ExpansionSettingSerializationSlider setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return false;

		float value = setting.GetValue();

		m_Slider = new OptionSelectorSlider( optionWidget, value, this, false, setting.m_Min, setting.m_Max );
		if (setting.m_Step)
			m_Slider.SetStep(setting.m_Step / (setting.m_Max - setting.m_Min));
		m_Slider.m_OptionChanged.Insert( OnSliderOptionChanged );
		settingValue.Show( true );
		settingValue.SetText( m_Slider.GetValue().ToString() );

		m_ApplySetting.Insert( OnApplySetting_Slider );

		return true;
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnSliderOptionChanged
	// -----------------------------------------------------------
	private void OnSliderOptionChanged( float value )
	{
		m_Menu.SetChanged();
		settingValue.SetText( m_Slider.GetValue().ToString() );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnApplySetting_Slider
	// -----------------------------------------------------------
	private void OnApplySetting_Slider()
	{
		ExpansionSettingSerializationSlider setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return;

		setting.SetValue( m_Slider.GetValue() );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget ConstructEnum
	// -----------------------------------------------------------
	bool ConstructEnum()
	{
		ExpansionSettingSerializationEnum setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return false;

		int value = setting.GetValue();

		m_Selector = new OptionSelectorMultistate( optionWidget, value, this, false, setting.m_Values );
		m_Selector.m_OptionChanged.Insert( OnEnumOptionChanged );

		m_ApplySetting.Insert( OnApplySetting_Enum );

		return true;
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnEnumOptionChanged
	// -----------------------------------------------------------
	private void OnEnumOptionChanged( float value )
	{
		m_Menu.SetChanged();
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnApplySetting_Enum
	// -----------------------------------------------------------
	private void OnApplySetting_Enum()
	{
		ExpansionSettingSerializationEnum setting;
		if ( !Class.CastTo( setting, m_Setting ) )
			return;

		setting.SetValue( m_Selector.GetValue() );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnMouseEnter
	// -----------------------------------------------------------
	override bool OnMouseEnter( Widget w, int x, int y )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "OnMouseEnter");
#endif
	
		if ( w == selectWidget )
		{
			m_Menu.ShowDetails( m_Setting.m_Name, m_Setting.m_DetailContent );
			selectWidget.SetColor( ARGB( 255, 0, 0, 0 ) );
			settingLabel.SetColor( ARGB( 255, 255, 0, 0 ) );
			
			if ( m_Selector && m_Selector.GetOptionTextWidget() ) m_Selector.GetOptionTextWidget().SetColor( ARGB( 255, 255, 0, 0 ) );
			
			return true;
		}
		
		return super.OnMouseEnter( w, x, y );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnMouseLeave
	// -----------------------------------------------------------
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "OnMouseLeave");
#endif
	
		if ( w == selectWidget )
		{
			m_Menu.HideDetails();
			selectWidget.SetColor( ARGB( 0, 0, 0, 0 ) );
			settingLabel.SetColor( ARGB( 255, 255, 255, 255 ) );
			
			if ( m_Selector && m_Selector.GetOptionTextWidget() ) m_Selector.GetOptionTextWidget().SetColor( ARGB( 255, 255, 255, 255 ) );
			
			return true;
		}
		
		return super.OnMouseLeave( w, enterW, x, y );
	}
	
	ExpansionSettingSerializationBase GetSetting()
	{
		return m_Setting;
	}
};
