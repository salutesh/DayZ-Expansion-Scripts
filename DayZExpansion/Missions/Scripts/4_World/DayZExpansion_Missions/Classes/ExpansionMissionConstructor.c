class ExpansionMissionConstructor
{
	void GenerateMissionTypes( out map< string, typename > missionMap )
	{
		TTypenameArray missionArrayNames = new TTypenameArray;
		RegisterMissions( missionArrayNames);

		Sort( missionArrayNames, missionArrayNames.Count() );
		
		missionMap = new map< string, typename >;
		
		for(int i = 0; i < missionArrayNames.Count(); i++)
		{
			if ( missionArrayNames[i].IsInherited( ExpansionMissionEventBase ) )
			{
				missionMap.Insert( missionArrayNames[i].ToString(), missionArrayNames[i] );
			}
		}
	}

	void RegisterMissions( out TTypenameArray missions )
	{
		//! Airdrops
		missions.Insert( ExpansionMissionEventAirdrop );
		
		//! DayZ rewritten Contaminated Areas
		missions.Insert( ExpansionMissionEventContaminatedArea );
		
		#ifdef BUILD_EXPERIMENTAL
		//! Zombie Horde
		//missions.Insert( ExpansionMissionEventHorde );
		#ifdef EXPANSIONMODAI
		//! AI General missions (Patrol, Faction War, Protecting Loot)
		missions.Insert( ExpansionMissionEventAI );
		#endif
		#endif
	}
};
