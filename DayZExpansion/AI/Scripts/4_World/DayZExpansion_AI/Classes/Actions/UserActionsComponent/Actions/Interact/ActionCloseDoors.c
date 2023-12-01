//! If vanilla noisesystem is ever updated so that noise events are generated in script, we won't need these overrides anymore

modded class ActionOpenDoors
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player)
			eAINoiseSystem.AddNoise(action_data.m_Player, action_data.m_Player.GetPosition(), "CfgVehicles SurvivorBase NoiseActionDefault");
	}
}

modded class ActionCloseDoors
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player)
			eAINoiseSystem.AddNoise(action_data.m_Player, action_data.m_Player.GetPosition(), "CfgVehicles SurvivorBase NoiseActionDefault");
	}
}

modded class ActionOpenFence
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player)
			eAINoiseSystem.AddNoise(action_data.m_Player, action_data.m_Player.GetPosition(), "CfgVehicles SurvivorBase NoiseActionDefault");
	}
}

modded class ActionCloseFence
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player)
			eAINoiseSystem.AddNoise(action_data.m_Player, action_data.m_Player.GetPosition(), "CfgVehicles SurvivorBase NoiseActionDefault");
	}
}

