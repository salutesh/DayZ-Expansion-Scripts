modded class CarHud
{
	TextWidget m_Expansion_RPMLabel;

	override void Init(Widget vehicleHudPanels)
	{		
		super.Init(vehicleHudPanels);

		m_Expansion_RPMLabel = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("RPMLabel"));
	}

	override void ShowVehicleInfo(PlayerBase player)
	{
		super.ShowVehicleInfo(player);

		if (m_CurrentVehicle)
		{
			//! Hide RPM pointer/redline if boat and not hybrid

			bool showRPM = true;

			if (m_CurrentVehicle.Expansion_IsBoat() && !m_CurrentVehicle.Expansion_IsCar())
				showRPM = false;

			m_VehicleRPMPointer.Show(showRPM);
			m_VehicleRPMRedline.Show(showRPM);
			m_Expansion_RPMLabel.Show(showRPM);
		}
	}

	override void RefreshVehicleHud(float timeslice)
	{
		super.RefreshVehicleHud(timeslice);

		if (!m_CurrentVehicle)
			return;

		int currentEngine = m_CurrentVehicle.Expansion_EngineGetCurrent();

		if (currentEngine > 0)
		{
			if (m_CurrentVehicle.Expansion_IsBoat())
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
		}
	}
}
