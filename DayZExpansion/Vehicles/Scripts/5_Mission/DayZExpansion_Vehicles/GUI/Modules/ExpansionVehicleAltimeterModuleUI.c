#ifdef CF_MVVM
class ExpansionVehicleAltimeterModuleUI : ExpansionVehicleModuleUI
{
	float m_Altimeter_Rotation;
	int m_Altitude;

	void ExpansionVehicleAltimeterModuleUI(ExpansionVehicleBase vehicle, ExpansionVehicleModule module, Widget parent)
	{
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Vehicles/GUI/layouts/hud/altimeter.layout";
	}

	override void OnUpdate(float pDt)
	{
		super.OnUpdate(pDt);

		m_Altimeter_Rotation = ((m_Vehicle.GetPosition()[1] * 360.0) / 1600.0) - 130.0;
		// "Index out of range 1>=1" if the addition isn't there 
		m_Altitude = 0.0 + m_Vehicle.GetPosition()[1];

		NotifyPropertyChanged("m_Altimeter_Rotation");
		NotifyPropertyChanged("m_Altitude");
	}
};
#endif
