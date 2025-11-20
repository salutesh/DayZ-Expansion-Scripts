modded class ExpansionClientSettingsModule
{
	override void OnSettingsChanged(Class sender, CF_EventArgs args)
	{
		super.OnSettingsChanged(sender, args);

		if (g_Game && g_Game.GetPlayer())
		{
			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			if (player && player.IsAlive() && player.IsInTransport())
			{
				DayZPlayerCamera3rdPersonVehicle camera = DayZPlayerCamera3rdPersonVehicle.Cast(player.GetCurrentCamera());
				if (camera)
				{
					camera.m_ExDistanceMultiplier = GetExpansionClientSettings().VehicleCameraDistance;
					camera.m_ExHeightMultiplier = GetExpansionClientSettings().VehicleCameraHeight;
				}
			}
		}
	}
};
