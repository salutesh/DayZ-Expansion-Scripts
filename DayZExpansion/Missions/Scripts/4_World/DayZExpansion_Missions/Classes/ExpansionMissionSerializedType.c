class ExpansionMissionSerializedEvent
{
	string m_MissionName;
	bool m_IsRunning;
	bool m_IsEnabled;
}

class ExpansionMissionSerializedType
{
	string m_MissionType;

	ref array< ref ExpansionMissionSerializedEvent > m_Missions;

	void ExpansionMissionSerializedType( string missionType, array< ExpansionMissionEventBase > missions )
	{
		m_MissionType = missionType;
		m_Missions = new array< ref ExpansionMissionSerializedEvent >;

		for ( int i = 0; i < missions.Count(); i++ )
		{
			ExpansionMissionSerializedEvent evt = new ExpansionMissionSerializedEvent;
			evt.m_MissionName = missions[i].MissionName;
			evt.m_IsRunning = missions[i].IsRunning();
			evt.m_IsEnabled = missions[i].Enabled;
			m_Missions.Insert( evt);
		}
	}
};
