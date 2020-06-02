/**
 * ExpansionNotificationUI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNotificationUIGrid extends ScriptedWidgetEventHandler
{
	protected ref Widget m_NotificationGridRoot;
	protected ref GridSpacerWidget m_NotificationGrid;
		
	// ------------------------------------------------------------
	// ExpansionNotificationUIGrid Constructor
	// ------------------------------------------------------------
	void ExpansionNotificationUIGrid()
	{
		m_NotificationGridRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_notification_grid.layout" );
		m_NotificationGrid = GridSpacerWidget.Cast( m_NotificationGridRoot.FindAnyWidget( "NotificationGrid" ) );
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUIGrid Destructor
	// ------------------------------------------------------------
	void ~ExpansionNotificationUIGrid()
	{
		if ( m_NotificationGridRoot )
		{
			m_NotificationGridRoot.Unlink();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUIGrid GetGrid
	// ------------------------------------------------------------
	ref GridSpacerWidget GetGrid()
	{
		return m_NotificationGrid;
	}
}

/**@class		ExpansionNotificationUI
 * @brief		This class handle notification ui data
 **/
class ExpansionNotificationUI extends ScriptedWidgetEventHandler
{   
	protected ref Widget m_NotificationRoot;
	protected ref Widget m_NotificationPanel;
	protected ref Widget m_NotificationColorSpacer;
	protected ref Widget m_NotificationElement;
	protected ref Widget m_NotificationTopSpacer;
	protected ref Widget m_NotificationBottomSpacer;
	protected ref Widget m_NotificationFotterSpacer;
	protected ref ImageWidget m_NotificationColor;
	protected ref ImageWidget m_NotificationIcon;
	protected ref RichTextWidget m_NotificationTitle;
	protected ref RichTextWidget m_NotificationText;

	protected NotificationRuntimeData m_Data;
	protected ExpansionNotificationModule m_Module;
	
	protected bool m_Showing;
	protected bool m_Hiding;
	protected bool m_Moving;
	
	protected float m_ShowUpdateTime;
	protected float m_HideUpdateTime;
	protected float m_MoveUpdateTime;
	
	protected float m_TotalShowUpdateTime;
	protected float m_TotalHideUpdateTime;
	protected float m_TotalMoveUpdateTime;
	
	protected bool m_SystemRemovalIdentified;
	
	protected ref AbstractWave m_Sound;
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI Constructor
	// ------------------------------------------------------------
	void ExpansionNotificationUI( ref NotificationRuntimeData data, ref ExpansionNotificationModule module, Widget grid )
	{
		m_Data = data;
		m_Module = module;
		
		m_NotificationRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_notification.layout", grid );	  
		m_NotificationPanel	= m_NotificationRoot.FindAnyWidget("Notification");
		m_NotificationColorSpacer = m_NotificationRoot.FindAnyWidget("NotificationColorSpacer");
		m_NotificationElement = m_NotificationRoot.FindAnyWidget("NotificationElement");
	
		m_NotificationTopSpacer = m_NotificationRoot.FindAnyWidget("TopSpacer");
		m_NotificationBottomSpacer = m_NotificationRoot.FindAnyWidget("BottomSpacer");
		m_NotificationFotterSpacer = m_NotificationRoot.FindAnyWidget("FotterSpacer");
		
		m_NotificationColor = ImageWidget.Cast( m_NotificationRoot.FindAnyWidget("NotificationColor") );
		m_NotificationIcon = ImageWidget.Cast( m_NotificationRoot.FindAnyWidget("NotificationIcon") );
		m_NotificationTitle = RichTextWidget.Cast( m_NotificationRoot.FindAnyWidget("NotificationTitle") );
		m_NotificationText = RichTextWidget.Cast( m_NotificationRoot.FindAnyWidget("NotificationText") );
		
		m_NotificationRoot.Show( false );
		m_NotificationIcon.Show( false );
		
		m_TotalShowUpdateTime = 0.2;
		m_TotalHideUpdateTime = 0.2;
		m_TotalMoveUpdateTime = 0.4;
		
		m_Hiding = false;
		m_Showing = false;
		m_Moving = false;

		m_NotificationRoot.SetAlpha( 0 );
		m_NotificationTitle.SetAlpha( 0 );
		m_NotificationText.SetAlpha( 0 );
		m_NotificationColor.SetAlpha( 0 );
		m_NotificationIcon.SetAlpha( 0 );
		m_NotificationElement.SetAlpha( 0 );
		
		UpdateElements();
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI Destructor
	// ------------------------------------------------------------	
	void ~ExpansionNotificationUI()
	{
		if ( m_NotificationRoot )
		{
			m_NotificationRoot.Unlink();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI ShowNotification
	// ------------------------------------------------------------	
	void UpdateElements()
	{
		m_NotificationText.Update();
		m_NotificationRoot.Update();
		m_NotificationPanel.Update();
		m_NotificationColorSpacer.Update();
		m_NotificationElement.Update();
		m_NotificationTopSpacer.Update();
		m_NotificationBottomSpacer.Update();
		m_NotificationFotterSpacer.Update();
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI ShowNotification
	// ------------------------------------------------------------	
	void ShowNotification()
	{
		//! Set widgets text, color and Icon
		m_NotificationTitle.SetText( m_Data.GetTitleText() );
		m_NotificationText.SetText( m_Data.GetDetailText() );
		m_NotificationColor.SetColor( m_Data.GetColor() );
		
		if ( m_Data.GetIcon() )
		{
			m_NotificationIcon.LoadImageFile( 0, m_Data.GetIcon() );
			m_NotificationIcon.SetColor( m_Data.GetColor() );
			m_NotificationIcon.Show( true );
		}
		
		UpdateElements();

		if ( GetExpansionClientSettings() && GetExpansionClientSettings().ShowNotifications && GetExpansionClientSettings().NotificationSound && GetGame() && GetGame().GetPlayer() )
		{
			SEffectManager.PlaySoundOnObject("Expansion_Hint_Sound_SoundSet", GetGame().GetPlayer(), 0, 0.15, false );
		}
		
		//! Show root widget
		m_NotificationRoot.Show( true );
		
		m_Showing = true;
		m_ShowUpdateTime = 0;
	}
	
	// ------------------------------------------------------------
	// Expansion AnimateShowToast
	// ------------------------------------------------------------
	void AnimateShowNotification( float timeSlice )
	{		
		m_ShowUpdateTime += timeSlice;

		float actual = 1.5;
		if ( m_TotalShowUpdateTime != 0 )
			actual = m_ShowUpdateTime / m_TotalShowUpdateTime;
				
		m_NotificationTitle.SetAlpha( actual );
		m_NotificationText.SetAlpha( actual );
		m_NotificationRoot.SetAlpha( actual );
		m_NotificationColor.SetAlpha( actual );
		m_NotificationElement.SetAlpha( actual / 2 );
		
		if ( m_ShowUpdateTime >= m_TotalShowUpdateTime )
		{
			m_NotificationTitle.SetAlpha( 1 );
			m_NotificationText.SetAlpha( 1 );
			m_NotificationRoot.SetAlpha( 1 );
			m_NotificationColor.SetAlpha( 1 );
			m_NotificationElement.SetAlpha( 0.3 );
			
			m_Showing = false;
			
			m_Data.m_HasShown = true;
			m_Data.m_NotificationTime = GetGame().GetTickTime() + m_Data.GetTime();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI HideNotification
	// ------------------------------------------------------------
	void HideNotification()
	{	
		m_Hiding = true;
		m_HideUpdateTime = 0;
	}
	
	// ------------------------------------------------------------
	// Expansion AnimateHideNotification
	// ------------------------------------------------------------
	void AnimateHideNotification( float timeSlice )
	{
		m_HideUpdateTime += timeSlice;
		
		float actual = 1.5;
		if ( m_TotalHideUpdateTime != 0 )
			actual = m_HideUpdateTime / m_TotalHideUpdateTime;
				
		m_NotificationTitle.SetAlpha( 1 - actual );
		m_NotificationText.SetAlpha( 1 - actual );
		m_NotificationRoot.SetAlpha( 1 - actual );
		m_NotificationColor.SetAlpha( 1 - actual );
		m_NotificationElement.SetAlpha( ( 1 - actual ) / 2 );

		if ( m_HideUpdateTime >= m_TotalHideUpdateTime )
		{
			m_NotificationTitle.SetAlpha( 0 );
			m_NotificationText.SetAlpha( 0 );
			m_NotificationRoot.SetAlpha( 0 );
			m_NotificationColor.SetAlpha( 0 );
			m_NotificationElement.SetAlpha( 0 );

			m_NotificationRoot.Show( false );
			
			m_Module.RemoveNotification( this );
			
			m_Hiding = false;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionNotificationUI HideNotification
	// ------------------------------------------------------------
	void Update( float timeslice )
	{
		if ( m_NotificationRoot )
		{
			UpdateElements();
		}
		
		if ( m_Hiding )
		{
			AnimateHideNotification( timeslice );
		}
	
		if ( m_Showing )
		{
			AnimateShowNotification( timeslice );
		}
	}
}