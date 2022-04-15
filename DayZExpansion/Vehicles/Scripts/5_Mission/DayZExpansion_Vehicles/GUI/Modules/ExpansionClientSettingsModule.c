modded class ExpansionClientSettingsModule
{
	override void OnSettingsChanged(Class sender, CF_EventArgs args)
	{
		super.OnSettingsChanged(sender, args);

		if (GetGame() && GetGame().GetPlayer())
		{
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
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
