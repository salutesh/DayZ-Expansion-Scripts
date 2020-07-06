/**
 * ExpansionOptionSettingWidget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionOptionSettingWidget extends ScriptedWidgetEventHandler
{
	protected Widget layoutRoot;
	protected Widget selectWidget;
	protected Widget optionWidget;
	protected TextWidget settingLable;
	protected ButtonWidget settingButton;

	protected ref ExpansionClientSetting m_Setting;
	protected ref OptionsMenuExpansion m_Menu;
	protected ref OptionSelectorMultistate m_Selector;
	protected ref OptionSelectorSlider m_Slider;
	protected ref ScriptInvoker m_ApplySetting = new ScriptInvoker;
	
	void ExpansionOptionSettingWidget( Widget parent, ref ExpansionClientSetting setting, ref OptionsMenuExpansion menu )
	{
		layoutRoot = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/options/expansion_options_setting_new.layout", parent ) );
		
		m_Setting = setting;
		m_Menu = menu;

		selectWidget = Widget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting" ) );
		settingLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_label" ) );
		settingLable.SetText( m_Setting.m_Name );
		optionWidget = Widget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_option" ) );
		settingButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "expansion_options_setting_button" ) );
		
		layoutRoot.SetHandler( this );
		
		if ( !ConstructToggle( ExpansionClientSettingToggle.Cast( m_Setting ) ) && !ConstructSlider( ExpansionClientSettingSlider.Cast( m_Setting ) ) )
		{
			Error("ExpansionOptionSettingWidget::ExpansionOptionSettingWidget It was not ExpansionClientSettingToggle and ExpansionClientSettingSlider");
		}
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
	bool ConstructToggle( ExpansionClientSettingToggle toggle )
	{
		if ( !toggle )
			return false;

		int selection = 0;
		if ( toggle.GetValue() )
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
		int selection = OptionSelectorMultistate.Cast( m_Selector ).GetValue();
		
		if ( selection == 0 )
		{
			ExpansionClientSettingToggle.Cast( m_Setting ).SetValue( false );
		} else
		{
			ExpansionClientSettingToggle.Cast( m_Setting ).SetValue( true );
		}
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget ConstructSlider
	// -----------------------------------------------------------
	bool ConstructSlider( ExpansionClientSettingSlider toggle )
	{
		if ( !toggle )
			return false;

		float value = toggle.GetValue();

		m_Slider = new OptionSelectorSlider( optionWidget, value, this, false, toggle.m_Min, toggle.m_Max );
		m_Slider.m_OptionChanged.Insert( OnSliderOptionChanged );

		m_ApplySetting.Insert( OnApplySetting_Slider );

		return true;
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnSliderOptionChanged
	// -----------------------------------------------------------
	private void OnSliderOptionChanged( float value )
	{
		m_Menu.SetChanged();
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnApplySetting_Slider
	// -----------------------------------------------------------
	private void OnApplySetting_Slider()
	{
		float value = OptionSelectorSlider.Cast( m_Slider ).GetValue();
		
		ExpansionClientSettingSlider.Cast( m_Setting ).SetValue( value );
	}
	
	// -----------------------------------------------------------
	// ExpansionOptionSettingWidget OnMouseEnter
	// -----------------------------------------------------------
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionOptionSettingWidget::OnMouseEnter - w: " + w.GetName() );
		#endif
		
		if ( w == selectWidget )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionOptionSettingWidget::OnMouseEnter - selectWidget");
			#endif

			m_Menu.ShowDetails( m_Setting.m_DetailLabel, m_Setting.m_DetailContent );
			selectWidget.SetColor( ARGB( 255, 0, 0, 0 ) );
			settingLable.SetColor( ARGB( 255, 255, 0, 0 ) );
			
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionOptionSettingWidget::OnMouseLeave - w: " + w.GetName() );
		#endif
		
		if ( w == selectWidget )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionOptionSettingWidget::OnMouseLeave - selectWidget");
			#endif

			m_Menu.HideDetails();
			selectWidget.SetColor( ARGB( 0, 0, 0, 0 ) );
			settingLable.SetColor( ARGB( 255, 255, 255, 255 ) );
			
			if ( m_Selector && m_Selector.GetOptionTextWidget() ) m_Selector.GetOptionTextWidget().SetColor( ARGB( 255, 255, 255, 255 ) );
			
			return true;
		}
		
		return super.OnMouseLeave( w, enterW, x, y );
	}
}