modded class MissionGameplay
{
	override void AddActiveInputExcludes(array<string> excludes)
	{
		Expansion_HandleVehicleExcludes(excludes);

		super.AddActiveInputExcludes(excludes);
	}

	override void RemoveActiveInputExcludes(array<string> excludes, bool bForceSupress = false)
	{
		Expansion_HandleVehicleExcludes(excludes);

		super.RemoveActiveInputExcludes(excludes, bForceSupress);
	}

	void Expansion_HandleVehicleExcludes(array<string> excludes)
	{
		if (excludes.Find("expansionvehicles") == -1)
		{
			array<string> search = {"map", "menu", "radialmenu"};

			foreach (string exclude: search)
			{
				if (excludes.Find(exclude) > -1)
				{
					excludes.Insert("expansionvehicles");
					break;
				}
			}
		}
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		if (GetUApi().GetInputByName("UAExpansionHeliAutoHover").LocalPress() && player && isAliveConscious && !inputIsFocused && !menu && !viewMenu)
		{
			ExpansionVehicle vehicle;
			if (ExpansionVehicle.Get(vehicle, player) && vehicle.IsPlayerInSeat(player, DayZPlayerConstants.VEHICLESEAT_DRIVER))
				vehicle.SwitchAutoHover();
		}
	}
}
