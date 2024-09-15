#ifndef DAYZ_1_25
//! Bugfix for 1.26 failing to load 1.25 agentpool https://feedback.bistudio.com/T183495
modded class PlayerAgentPool
{
	//! Instead of using AgentPool::STORAGE_VERSION, hardcode version to 100 (AgentPool::STORAGE_VERSION value under DayZ 1.25)
	//! so AI saved under versions older than DayZ 1.26 can still be loaded
	override int GetStorageVersion()
	{
		return 100;
	}
}
#endif
