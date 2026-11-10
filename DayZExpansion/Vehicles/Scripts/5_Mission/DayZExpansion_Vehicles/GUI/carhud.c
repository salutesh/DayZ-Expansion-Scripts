modded class CarHud
{
	override void RefreshVehicleHud(float timeslice)
	{
		super.RefreshVehicleHud(timeslice);

		if (!m_CurrentVehicle)
			return;

		int currentEngine = m_CurrentVehicle.Expansion_EngineGetCurrent();

		if (currentEngine > 0)
		{
			if (m_CurrentVehicle.Expansion_IsBoat() && !m_CurrentVehicle.Expansion_IsCar())
			{
				//! If 2nd engine is selected for boat or hybrid vehicle, show only one forward gear

				int engagedGear	= -1;
				int prevGear	= -1;
				int nextGear	= -1;
				
				if (m_CurrentVehicle.GearboxGetType() == CarGearboxType.MANUAL)
				{
					engagedGear = m_CurrentVehicle.GetGear();
					prevGear = engagedGear - 1;
					nextGear = engagedGear + 1;

					if (engagedGear == CarGear.NEUTRAL)
					{
						prevGear = CarGear.REVERSE;
					}
					else if (engagedGear == CarGear.REVERSE)
					{
						prevGear = -1;
						nextGear = CarGear.NEUTRAL;
					}

					string engagedGearLabel;

					if (engagedGear >= CarGear.FIRST)
						engagedGearLabel = "F";
					else
						engagedGearLabel = m_VehicleGearTable.Get(engagedGear);

					m_VehicleCurrentGearValue.SetText(engagedGearLabel);
					m_VehicleNextGearValue.Show(nextGear <= CarGear.FIRST);

					string nextGearLabel;

					if (nextGear >= CarGear.FIRST)
						nextGearLabel = "F";
					else
						nextGearLabel = m_VehicleGearTable.Get(nextGear);

					m_VehicleNextGearValue.SetText(nextGearLabel);

					string prevGearLabel;

					if (prevGear >= CarGear.FIRST)
						prevGearLabel = "N";
					else
						prevGearLabel = m_VehicleGearTable.Get(prevGear);

					m_VehiclePrevGearValue.SetText(prevGearLabel);
				}
			}

			if (g_Game.IsMultiplayer())
			{
				float rpm01 = m_CurrentVehicle.Expansion_EngineGetRPM(currentEngine) / m_CurrentVehicle.Expansion_EngineGetRPMMax(currentEngine);

				m_VehicleRPMPointer.SetRotation(0, 0, rpm01 * 270 - 130, true);
			}
		}
	}
}
