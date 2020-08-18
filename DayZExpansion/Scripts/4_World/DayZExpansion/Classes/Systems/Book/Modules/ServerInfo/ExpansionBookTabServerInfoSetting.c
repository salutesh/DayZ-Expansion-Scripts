/**
 * ExpansionBookTabServerInfoSetting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabServerInfoSetting extends ScriptedWidgetEventHandler
{
	Widget m_Root;	
	TextWidget m_SettingLable;
	TextWidget m_SettingValue;
	
	ref ExpansionServerInfoSetting m_Setting;
	ref ExpansionUITooltip m_TooltipUI;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSetting Constructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerInfoSetting(Widget parent, ExpansionServerInfoSetting setting)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/settings/expansion_book_settings_entry.layout", parent ) );
		
		m_SettingLable = TextWidget.Cast( m_Root.FindAnyWidget( "setting_entry_setting_label" ) ); 
		m_SettingValue = TextWidget.Cast( m_Root.FindAnyWidget( "setting_entry_val" ) );
		
		m_Setting = setting;
		
		SetSetting();
		
		m_Root.SetHandler( this );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSetting Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookTabServerInfoSetting()
	{
		if ( m_TooltipUI.IsVisible() )
			m_TooltipUI.HideTooltip();
		
		delete m_TooltipUI;
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSetting SetButton
	// ------------------------------------------------------------
	void SetSetting()
	{
		m_SettingLable.SetText( m_Setting.m_Name );
		
		if ( m_Setting.m_IsBoolSetting )
		{
			if ( m_Setting.m_State)
			{
				m_SettingValue.SetText("#STR_EXPANSION_BOOK_SETTINGS_ENABLED");
			}
			else
			{
				m_SettingValue.SetText("#STR_EXPANSION_BOOK_SETTINGS_DISABLED");
			}
		}
		else
		{
			m_SettingValue.SetText( m_Setting.m_Value );
		}
		
		m_TooltipUI = new ExpansionUITooltip(m_Setting.m_Description, m_Setting.m_Name); 
	}	
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_Root && w == m_Root )
		{
			m_TooltipUI.ShowTooltip();
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_Root && w == m_Root )
		{
			m_TooltipUI.HideTooltip();
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// GetTooltip
	// ------------------------------------------------------------	
	ref ExpansionUITooltip GetTooltip()
	{
		return m_TooltipUI;
	}
}