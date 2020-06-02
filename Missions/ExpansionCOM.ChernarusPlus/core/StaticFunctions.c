static MissionBase GetMission()
{
	return MissionBase.Cast( GetGame().GetMission() );
}

static CommunityOfflineClient GetClientMission()
{
	return CommunityOfflineClient.Cast( GetGame().GetMission() );
}