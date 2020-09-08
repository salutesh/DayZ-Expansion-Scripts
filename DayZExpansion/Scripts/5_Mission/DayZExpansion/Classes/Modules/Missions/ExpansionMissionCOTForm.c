/**
 * ExpansionMissionCOTForm.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMissionCOTForm extends JMFormBase
{
	private autoptr array< ExpansionMissionCOTRowItem > m_MissionList;

	private UIActionText m_MissionListCount;
	private UIActionEditableText m_MissionListFilter;
	
	private UIActionScroller m_MissionListScroller;
	private Widget m_MissionListRows;

	private Widget m_LeftPanel;
	private Widget m_RightPanel;
	private Widget m_RightPanelDisable;

	private UIActionScroller m_ActionListScroller;
	private Widget m_ActionsWrapper;
	
	private UIActionButton m_StartMission;
	private UIActionButton m_EndMission;

	private ExpansionMissionCOTRowItem m_SelectedMission;
	private int m_NumberRunningMissions;

	private ExpansionMissionCOTModule m_Module;
	private ExpansionMissionModule m_BaseModule;

	void ExpansionMissionCOTForm()
	{
		ExpansionMissionModule.SI_Started.Insert( Event_Started );
		ExpansionMissionModule.SI_Ended.Insert( Event_Ended );
	}

	void ~ExpansionMissionCOTForm()
	{
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		return Class.CastTo( m_BaseModule, GetModuleManager().GetModule( ExpansionMissionModule ) ) && Class.CastTo( m_Module, mdl );
	}

	override void OnShow()
	{
		super.OnShow();

		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( RefreshData, 1000, true );

		SetSelectedMission( NULL );

		RefreshData();
	}

	override void OnHide() 
	{
		super.OnHide();

		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( RefreshData );
	}

	override void OnInit()
	{
		InitWidgetsLeft();
		InitWidgetsRight();
	}

	private void InitWidgetsLeft()
	{
		m_LeftPanel = layoutRoot.FindAnyWidget( "panel_left" );

		Widget leftPanelGrid = UIActionManager.CreateGridSpacer( m_LeftPanel, 3, 1 );

		m_MissionList = new array< ExpansionMissionCOTRowItem >;

		m_MissionListCount = UIActionManager.CreateText( leftPanelGrid, "Total Missions:", "0/0" );
		m_MissionListFilter = UIActionManager.CreateEditableText( leftPanelGrid, "Filter: ", this, "Event_RefreshData" );
		m_MissionListScroller = UIActionManager.CreateScroller( leftPanelGrid );
		m_MissionListRows = UIActionManager.CreateActionRows( m_MissionListScroller.GetContentWidget() );
		
		for ( int i = 0; i < 10; i++ )
		{
			GridSpacerWidget gsw;
			if ( !Class.CastTo( gsw, m_MissionListRows.FindAnyWidget( "Content_Row_0" + i ) ) )
				continue;

			for ( int j = 0; j < 100; j++ )
			{
				Widget rowWidget = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/mission_row_item.layout", gsw );
				
				if ( !rowWidget )
					continue;

				ExpansionMissionCOTRowItem rowScript;
				rowWidget.GetScript( rowScript );

				if ( !rowScript )
					continue;

				rowScript.InitForm( this );

				m_MissionList.Insert( rowScript );
			}
		}

		m_MissionListScroller.UpdateScroller();
	}

	private void InitWidgetsRight()
	{
		m_RightPanel = layoutRoot.FindAnyWidget( "panel_right" );
		m_RightPanelDisable = layoutRoot.FindAnyWidget( "panel_right_disable" );

		// m_ActionListScroller = UIActionManager.CreateScroller( m_RightPanel );
		// m_ActionsWrapper = UIActionManager.CreateGridSpacer( m_ActionListScroller.GetContentWidget(), 2, 1 );
		m_ActionsWrapper = UIActionManager.CreateGridSpacer( m_RightPanel, 2, 1 );

		m_StartMission = UIActionManager.CreateButton( m_ActionsWrapper, "Start", this, "Event_StartMission" );
		m_EndMission = UIActionManager.CreateButton( m_ActionsWrapper, "Stop", this, "Event_EndMission" );
	}

	void Event_StartMission( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		if ( !m_SelectedMission )
			return;

		m_BaseModule.StartMission( m_SelectedMission.GetMissionType(), m_SelectedMission.GetMissionName() );
	}

	void Event_EndMission( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		if ( !m_SelectedMission )
			return;

		m_BaseModule.EndMission( m_SelectedMission.GetMissionType(), m_SelectedMission.GetMissionName() );
	}

	void SetSelectedMission( ref ExpansionMissionCOTRowItem mission )
	{
		m_SelectedMission = NULL;
		m_RightPanelDisable.Show( true );

		if ( mission == NULL )
			return;

		if ( m_SelectedMission != NULL && m_SelectedMission.GetMissionName() == mission.GetMissionName() && m_SelectedMission.GetMissionType() == mission.GetMissionType() )
			return;

		m_SelectedMission = mission;
		m_RightPanelDisable.Show( false );

		UpdateSelectedMission();
	}

	void Event_Started( ExpansionMissionEventBase evt )
	{
		RefreshData();
	}

	void Event_Ended( ExpansionMissionEventBase evt )
	{
		RefreshData();
	}

	void Event_RefreshData( UIEvent eid, ref UIActionBase action )
	{
		RefreshData();
	}

	void UpdateSelectedMission()
	{
		if ( m_SelectedMission )
		{
			bool canRunNewMission = false;
			if ( m_NumberRunningMissions < GetExpansionSettings().GetMission().MaxMissions )
				canRunNewMission = true;

			bool isRunning = m_SelectedMission.GetMission().m_IsRunning;
			bool isEnabled = m_SelectedMission.GetMission().m_IsEnabled;

			m_StartMission.SetEnabled( !isRunning && canRunNewMission && isEnabled );
			m_EndMission.SetEnabled( isRunning && isEnabled );
		}
	}

	// to handle by recieving by rpc later. this only works in sp for now
	void RefreshData()
	{
		ExpansionMissionModule mod;
		if ( !Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMissionModule ) ) )
			return;

		ref array< ref ExpansionMissionSerializedType > serialized = mod.Serialize();

		string filter = m_MissionListFilter.GetText();
		bool isFiltering = filter.Length() > 0;
		filter.ToLower();

		int idx = 0;
		int total_count = 0;
		int count = 0;
		int missionTypeHeading = -1;

		m_NumberRunningMissions = 0;
	
		for ( int i = 0; i < serialized.Count(); i++ ) 
		{
			missionTypeHeading = -1;

			if ( !serialized[i] )
				continue;

			for ( int j = 0; j < serialized[i].m_Missions.Count(); j++ ) 
			{
				string name = serialized[i].m_Missions[j].m_MissionName;
				name.ToLower();

				if ( serialized[i].m_Missions[j].m_IsRunning )
					m_NumberRunningMissions++;

				total_count++;

				if ( isFiltering && !name.Contains( filter ) )
					continue;

				if ( missionTypeHeading == -1 )
				{
					missionTypeHeading = idx;
					idx++;
				}

				m_MissionList[idx].Register( serialized[i].m_MissionType, serialized[i].m_Missions[j] );
				idx++;
				count++;
			}

			if ( missionTypeHeading != -1 )
			{
				m_MissionList[missionTypeHeading].RegisterHeading( serialized[i].m_MissionType );
			}
		}

		m_MissionListCount.SetText( "" + count + "/" + total_count );

		while ( idx < m_MissionList.Count() )
		{
			m_MissionList[idx].Register( "", NULL );
			idx++;
		}

		UpdateSelectedMission();
	}
}