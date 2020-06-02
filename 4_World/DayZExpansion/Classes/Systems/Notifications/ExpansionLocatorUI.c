/**
 * ExpansionLocatorUI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionToastLocations
 * @brief		This class handle location toasts
 **/
class ExpansionLocatorUI extends ScriptedWidgetEventHandler
{
	protected Widget m_LocatorRoot;
	protected TextWidget m_LocatorTime;
	protected TextWidget m_LocatorCityName;
	
	protected bool m_IsCityShow;
	protected bool m_IsCityHide;

	protected float m_ShowCityAlpha;
	protected float m_HideCityAlpha;
	
	protected float m_ShowTimeAlpha;
	protected float m_HideTimeAlpha;
	
	protected bool m_PlaySound;
	protected bool m_PlaySoundTime;
	protected bool m_BackSound;
	protected bool m_BackSoundTime;

	protected string m_CityName;
	protected string m_CityTime;
	
	protected ScriptCallQueue Loacator_Queue;
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI Constructor
	// ------------------------------------------------------------
	void ExpansionLocatorUI()
	{
		m_LocatorRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_locator.layout" );
		m_LocatorCityName = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionCityName" ) );
		m_LocatorTime = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionTime" ) );
		m_LocatorRoot.Show( false );

		Loacator_Queue = GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY); 
	}

	// ------------------------------------------------------------
	// ExpansionLocatorUI AnimateShowSimpleLoactionClient
	// ------------------------------------------------------------
	void AnimateShowSimpleLoactionClient()
	{	
		if ( m_LocatorRoot )
		{
			m_LocatorRoot.Show(true);
		}
		else 
		{
			m_LocatorRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_locator.layout" );
			m_LocatorCityName = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionCityName" ) );
			m_LocatorTime = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionTime" ) );

			m_LocatorRoot.Show(true);
		}

		m_LocatorTime.SetText(m_CityName);
		m_LocatorTime.SetText(m_CityTime);
		
		m_ShowCityAlpha += 0.024;
		m_LocatorCityName.SetAlpha(m_ShowCityAlpha);

		if (!m_PlaySound)
		{
			SEffectManager.PlaySoundOnObject("Expansion_Print1_SoundSet", GetGame().GetPlayer());
			m_PlaySound = true;
		}
		
		if (m_ShowCityAlpha > 1)
		{	
			if (!m_PlaySoundTime)
			{
				SEffectManager.PlaySoundOnObject("Expansion_Print2_SoundSet", GetGame().GetPlayer());
				m_PlaySoundTime = true;
			}
			
			m_LocatorCityName.SetAlpha(1);
			m_ShowTimeAlpha += 0.024;
			m_LocatorTime.SetAlpha(m_ShowTimeAlpha);		
			if (m_ShowTimeAlpha > 1)
			{
				m_LocatorTime.SetAlpha(1);
				GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(AnimateShowSimpleLoactionClient);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI HideCityClient
	// ------------------------------------------------------------
	void HideCityClient(int l_Delay = 100)
	{
		Loacator_Queue.CallLater(this.OnHideCityClient, l_Delay, false);
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI AnimateHideSimpleToastClient
	// ------------------------------------------------------------
	void AnimateHideSimpleLocationClient()
	{		
		m_HideCityAlpha = m_HideCityAlpha - 0.024;
		m_LocatorTime.SetAlpha( m_HideCityAlpha );

		if (!m_BackSoundTime) // Put sounds back if you want
		{
				//SEffectManager.PlaySoundOnObject("Expansion_Back2_SoundSet", GetGame().GetPlayer());
				m_BackSoundTime = true;
		}
		
		if (m_HideCityAlpha < 0)
		{	
			if (!m_BackSound)
			{
				//SEffectManager.PlaySoundOnObject("Expansion_Back1_SoundSet", GetGame().GetPlayer());
				m_BackSound = true;
			}
			
			m_LocatorTime.SetAlpha(0);
			m_HideTimeAlpha -= 0.024;
			m_LocatorCityName.SetAlpha(m_HideTimeAlpha);		
			if (m_HideTimeAlpha < 0)
			{
				m_LocatorCityName.SetAlpha(0);
				GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(AnimateHideSimpleLocationClient);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI OnHideCityClient
	// ------------------------------------------------------------
	void OnHideCityClient()
	{
		//! Animation
		m_IsCityHide = true;
		if (m_IsCityHide)
		{
			m_BackSoundTime = false;
			m_BackSound = false;
			
			m_PlaySound = false;
			m_PlaySoundTime = false;
			
			m_HideCityAlpha = 1;
			m_HideTimeAlpha = 1;
			m_LocatorTime.SetAlpha(1);
			m_LocatorCityName.SetAlpha(1);
			
			m_IsCityHide = false;
		}
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(AnimateHideSimpleLocationClient);
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI OnShowCityClient
	// ------------------------------------------------------------
	void OnShowCityClient( string city )
	{
		int m_Case = city.ToUpper();
		
		m_LocatorCityName.SetText( city );

		m_CityName = city;
		
		string m_HourText;
		string m_MinuteText;
		int m_Year, m_Month, m_Day, m_Hour, m_Minute;
		GetGame().GetWorld().GetDate(m_Year, m_Month, m_Day, m_Hour, m_Minute);

		if (m_Hour < 10)
		{
			m_HourText = "0" + m_Hour;
		}
		else
		{
			m_HourText = m_Hour.ToString();
		}

		if (m_Minute < 10)
		{
			m_MinuteText = "0" + m_Minute;
		}
		else
		{
			m_MinuteText = m_Minute.ToString();
		}
		
		m_LocatorTime.SetText(m_HourText + ":" + m_MinuteText + ":" + Math.RandomInt(10, 60));

		m_CityTime = m_HourText + ":" + m_MinuteText + ":" + Math.RandomInt(10, 60);
		
		//! Show widget
		m_LocatorRoot.Show(true);
		
		//! Animation
		m_IsCityShow = true;
		if (m_IsCityShow)
		{
			m_ShowCityAlpha = 0;
			m_ShowTimeAlpha = 0;
			m_LocatorTime.SetAlpha(0);
			m_LocatorCityName.SetAlpha(0);
			
			m_IsCityShow = false;
		}
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(AnimateShowSimpleLoactionClient);
		
		HideCityClient(5000);
	}
}