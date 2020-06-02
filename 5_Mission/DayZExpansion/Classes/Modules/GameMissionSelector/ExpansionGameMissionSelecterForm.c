class ExpansionGameMissionSelecterForm extends JMFormBase
{
	private Widget m_ActionsWrapper;
	private Widget m_ActionsFilterWrapper;

	private UIActionEditableText m_Filter;
	private UIActionEditableText m_SelectedMission;

	private TextListboxWidget m_MissionsList;

    private string m_PathToMissions;

	void ExpansionGameMissionSelecterForm()
	{
    }

	void ~ExpansionGameMissionSelecterForm()
	{
	}

	override void OnInit()
	{
		m_ActionsFilterWrapper = layoutRoot.FindAnyWidget( "actions_filter_wrapper" );

		m_Filter = UIActionManager.CreateEditableText( m_ActionsFilterWrapper, "Filter: ", this, "Type_UpdateList" );

		m_MissionsList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("game_missions_list") );

		m_ActionsWrapper = layoutRoot.FindAnyWidget( "actions_wrapper" );

        m_SelectedMission = UIActionManager.CreateEditableText( m_ActionsWrapper, "Mission: ", this, "Type_SelectedMission" );

        UIActionManager.CreateButton( m_ActionsWrapper, "Load Mission", this, "LoadMission" );

        UpdateList();
	}

	void Type_UpdateList( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CHANGE )
			return;

		UpdateList();
	}

	void Type_SelectedMission( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CHANGE )
			return;

	}

	void UpdateList()
	{
		m_MissionsList.ClearItems();

		string filter = "" + m_Filter.GetText();
		filter.ToLower();

		TStringArray missions = GetMissionList();
		if ( !missions || missions.Count() == 0 )
			return;

		for ( int i = 0; i < missions.Count(); i++ )
		{
			string name = "" + missions[i];
			name.ToLower();

			if ( (filter != "" && (!name.Contains( filter ))) ) 
			{
				continue;
			}

			m_MissionsList.AddItem( missions[i], NULL, 0 );
		}
	}

	private TStringArray GetMissionList()
	{
		string file_name;
		int file_attr;
		int flags;
		TStringArray list = new TStringArray;
		
		m_PathToMissions = "missions";//"$saves:\\missions";
		
		string path_find_pattern = m_PathToMissions + "\\*";
		FindFileHandle file_handler = FindFile( path_find_pattern, file_name, file_attr, flags );
		
		bool found = true;
		while ( found )
		{				
			if ( file_attr & FileAttr.DIRECTORY )
			{
				list.Insert( file_name );
			}
			
			found = FindNextFile( file_handler, file_name, file_attr );
		}
		
		return list;
	}

	string GetCurrentSelectedMissionName()
	{
		if ( m_MissionsList.GetSelectedRow() != -1 )
		{
			string mission_name;
			m_MissionsList.GetItemText( m_MissionsList.GetSelectedRow(), 0, mission_name );
			return mission_name;
		}

		return "";
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
        m_SelectedMission.SetText( GetCurrentSelectedMissionName() );

		return true;
	}
    
	void LoadMission( UIEvent eid, ref UIActionBase action ) 
	{
		//CURLCore curlCore = GetCURLCore();
		//if ( curlCore == NULL )
		//	curlCore = CreateCURLCore();
//
		//curlCore.EnableDebug( true );
		//CURLContext curlCtx = curlCore.GetCURLContext("https://la-quarantaine.fr/uploads/affiches/");
		//if ( curlCtx )
		//{
		//	curlCtx.SetHeader( "text/plain" );
//
		// Print( "ECURL_EMPTY" + ECURLResultState.ECURL_EMPTY ); 
		// Print( "ECURL_PENDING" + ECURLResultState.ECURL_PENDING ); 
		// Print( "ECURL_FEEDING" + ECURLResultState.ECURL_FEEDING ); 
		// Print( "ECURL_SUCCESS" + ECURLResultState.ECURL_SUCCESS ); 
		// Print( "ECURL_PROCESSED" + ECURLResultState.ECURL_PROCESSED ); 
		// Print( "ECURL_ERROR" + ECURLResultState.ECURL_ERROR ); 
		// Print( "ECURL_ERROR_CLIENTERROR" + ECURLResultState.ECURL_ERROR_CLIENTERROR ); 
		// Print( "ECURL_ERROR_SERVERERROR" + ECURLResultState.ECURL_ERROR_SERVERERROR ); 
		// Print( "ECURL_ERROR_APPERROR" + ECURLResultState.ECURL_ERROR_APPERROR ); 
		// Print( "ECURL_ERROR_TIMEOUT" + ECURLResultState.ECURL_ERROR_TIMEOUT ); 
		// Print( "ECURL_ERROR_NOTIMPLEMENTED" + ECURLResultState.ECURL_ERROR_NOTIMPLEMENTED ); 
		// Print( "ECURL_ERROR_UNKNOWN" + ECURLResultState.ECURL_ERROR_UNKNOWN ); 
//
		//	ExpansionTestCallback callback = new ExpansionTestCallback;
		//	Print( curlCtx.FILE( callback, "test.txt", "$profile:test.txt" ) );
		//}

		string mission_name = m_SelectedMission.GetText();
        if ( mission_name != "" )
		{
            string mission_path = m_PathToMissions + "\\" + mission_name;
                    
            GetGame().PlayMission( mission_path );
        }
	}
}

class ExpansionTestCallback: CURLCallback
{
	/**
	\brief Called in case request failed (ECURLResultState) - Note! May be called multiple times in case of (RetryCount > 1)
	*/
	override void OnError( int errorCode )
	{
		// override this with your implementation
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint(" !!! OnError() ");
		#endif
	}

	/**
	\brief Called in case request timed out or handled improperly (no error, no success, no data)
	*/
	override void OnTimeout()
	{
		// override this with your implementation
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint(" !!! OnTimeout() ");
		#endif
	}

	/**
	\brief Called when data arrived and/ or response processed successfully
	*/
	override void OnSuccess( string data, int dataSize )
	{
		// override this with your implementation
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint(" !!! OnSuccess() size=" + dataSize );
		#endif
		
		if( dataSize > 0 )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint(data);
			#endif
		}
	}

	override void OnFileCreated( string fileName, int dataSize )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint(" !!! OnFileCreated() file=" + fileName + " size=" + dataSize );
		#endif
	}
}