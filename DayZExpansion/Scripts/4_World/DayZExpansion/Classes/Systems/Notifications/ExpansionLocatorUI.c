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

	protected float m_FadeIn;
	protected float m_FadeOut;
	
	protected ScriptCallQueue Loacator_Queue;
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI Constructor
	// ------------------------------------------------------------
	void ExpansionLocatorUI()
	{
		SetupWidgets();

		Loacator_Queue = GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ); 
	}

	void SetupWidgets()
	{
		m_LocatorRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_locator.layout" );
		m_LocatorCityName = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionCityName" ) );
		m_LocatorTime = TextWidget.Cast( m_LocatorRoot.FindAnyWidget( "ExpansionTime" ) );
		m_LocatorRoot.Show( false );
	}

	// ------------------------------------------------------------
	// ExpansionLocatorUI AnimateShowSimpleLoactionClient
	// ------------------------------------------------------------
	void AnimateShowSimpleLoactionClient()
	{
		m_ShowCityAlpha += m_FadeIn;
		m_LocatorCityName.SetAlpha( m_ShowCityAlpha );

		if ( !m_PlaySound )
		{
			SEffectManager.PlaySoundOnObject( "Expansion_Print1_SoundSet", GetGame().GetPlayer() );
			m_PlaySound = true;
		}
		
		if ( m_ShowCityAlpha > 1 )
		{	
			if ( !m_PlaySoundTime )
			{
				SEffectManager.PlaySoundOnObject( "Expansion_Print2_SoundSet", GetGame().GetPlayer() );
				m_PlaySoundTime = true;
			}
			
			m_LocatorCityName.SetAlpha( 1 );
			m_ShowTimeAlpha += m_FadeIn;
			m_LocatorTime.SetAlpha( m_ShowTimeAlpha );
			if ( m_ShowTimeAlpha > 1 )
			{
				m_LocatorTime.SetAlpha( 1 );
				GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Remove( AnimateShowSimpleLoactionClient );
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI HideCityClient
	// ------------------------------------------------------------
	void HideCityClient( int delay = 5000 )
	{
		Loacator_Queue.CallLater( OnHideCityClient, delay, false );
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI AnimateHideSimpleToastClient
	// ------------------------------------------------------------
	void AnimateHideSimpleLocationClient()
	{		
		m_HideCityAlpha = m_HideCityAlpha - m_FadeOut;
		m_LocatorTime.SetAlpha( m_HideCityAlpha );

		if ( !m_BackSoundTime ) // Put sounds back if you want
		{
				//SEffectManager.PlaySoundOnObject( "Expansion_Back2_SoundSet", GetGame().GetPlayer() );
				m_BackSoundTime = true;
		}
		
		if ( m_HideCityAlpha < 0 )
		{	
			if ( !m_BackSound )
			{
				//SEffectManager.PlaySoundOnObject( "Expansion_Back1_SoundSet", GetGame().GetPlayer() );
				m_BackSound = true;
			}
			
			m_LocatorTime.SetAlpha( 0 );
			m_HideTimeAlpha -= m_FadeOut;
			m_LocatorCityName.SetAlpha( m_HideTimeAlpha );		
			if ( m_HideTimeAlpha < 0 )
			{
				m_LocatorCityName.SetAlpha( 0 );
				GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Remove( AnimateHideSimpleLocationClient );
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI OnHideCityClient
	// ------------------------------------------------------------
	void OnHideCityClient()
	{
		//! Animation
		m_BackSoundTime = false;
		m_BackSound = false;
		
		m_PlaySound = false;
		m_PlaySoundTime = false;
		
		m_HideCityAlpha = 1;
		m_HideTimeAlpha = 1;
		m_LocatorTime.SetAlpha( 1 );
		m_LocatorCityName.SetAlpha( 1 );
		
		GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Insert( AnimateHideSimpleLocationClient );
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorUI OnShowCityClient
	// ------------------------------------------------------------
	void OnShowCityClient( string title = "", string description = "", int duration = 5000, float fadein = 0.024, float fadeout = 0.024 )
	{
		if ( !m_LocatorRoot )
		{
			//! This is needed because on reconnect, even if our ExpansionLocatorUI instance still exists, the widgets will not anymore
			SetupWidgets();
		}

		int m_Case = title.ToUpper();
		m_LocatorCityName.SetText( title );

		if ( description == "" )
		{
			string m_HourText;
			string m_MinuteText;
			int m_Year, m_Month, m_Day, m_Hour, m_Minute;
			GetGame().GetWorld().GetDate( m_Year, m_Month, m_Day, m_Hour, m_Minute );

			if ( m_Hour < 10 )
			{
				m_HourText = "0" + m_Hour;
			}
			else
			{
				m_HourText = m_Hour.ToString();
			}

			if ( m_Minute < 10 )
			{
				m_MinuteText = "0" + m_Minute;
			}
			else
			{
				m_MinuteText = m_Minute.ToString();
			}

			description = m_HourText + ":" + m_MinuteText + ":" + Math.RandomInt(10, 60);
		}
		
		m_LocatorTime.SetText( description );

		m_FadeIn = fadein;
		m_FadeOut = fadeout;
		
		//! Animation
		m_ShowCityAlpha = 0;
		m_ShowTimeAlpha = 0;
		m_LocatorTime.SetAlpha( 0 );
		m_LocatorCityName.SetAlpha( 0 );
		
		//! Show widget
		m_LocatorRoot.Show( true );
		
		GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Insert( AnimateShowSimpleLoactionClient );
		
		HideCityClient( duration );
	}
}
