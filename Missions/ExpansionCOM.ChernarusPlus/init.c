#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\core\\BaseModuleInclude.c"

#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\expansion\\missions\\MissionConstructor.c"

Mission CreateCustomMission(string path)
{
	return new CommunityOfflineClient();
}

void main()
{
	bool loadTraderObjects = true;
	bool loadTraderNPCs = true;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);

	switch(MissionWorldName)
	{
		case "ChernarusPlusGloom":
		{
			MissionWorldName = "ChernarusPlus";
			break;
		}
		case "enochGloom":
		{
			MissionWorldName = "enoch";
			break;
		}
	}

	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}
	
	//! Init server central economy
	//Hive ce = CreateHive();
	//if ( ce )
	//	ce.InitOffline();
}