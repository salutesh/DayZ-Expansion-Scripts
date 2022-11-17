#ifdef CF_MVVM
class ExpansionVehicleEngineUI : ExpansionVehicleComponentUI
{
	ref map<int, string> m_Gears;

	ExpansionVehicleEngineBase m_Engine;

	float m_RPM_Rotation;

	float m_RPM_Mask;
	float m_RPM_MaskNeg;

	bool m_HasGears;

	string m_GearPrevious;
	string m_GearCurrent;
	string m_GearNext;
	bool m_GearPreviousShow;
	bool m_GearNextShow;

	void ExpansionVehicleEngineUI(ExpansionVehicleBase vehicle, ExpansionVehicleComponent module, Widget parent)
	{
		Class.CastTo(m_Engine, m_Module);

		m_Gears = new map<int, string>;
		m_Gears.Set(-1, "");
		m_Gears.Set((int)CarGear.REVERSE, "R");
		m_Gears.Set((int)CarGear.NEUTRAL, "N");
		m_Gears.Set((int)CarGear.FIRST, "1");
		m_Gears.Set((int)CarGear.SECOND, "2");
		m_Gears.Set((int)CarGear.THIRD, "3");
		m_Gears.Set((int)CarGear.FOURTH, "4");
		m_Gears.Set((int)CarGear.FIFTH, "5");
		m_Gears.Set((int)CarGear.SIXTH, "6");
		m_Gears.Set((int)CarGear.SEVENTH, "7");
		m_Gears.Set((int)CarGear.EIGTH, "8");

		m_RPM_Mask = m_Engine.m_RPMRedline / m_Engine.m_RPMMax;
		m_RPM_MaskNeg = 1.0 - m_RPM_Mask;

		NotifyPropertyChanged();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Vehicles/GUI/layouts/hud/engine.layout";
	}

	override void OnUpdate(float pDt)
	{
		super.OnUpdate(pDt);

		if (!m_Engine)
			return;

		float rpmNormalized = m_Engine.m_RPM / m_Engine.m_RPMMax;

		m_RPM_Rotation = (rpmNormalized * 270.0) - 130.0;

		m_HasGears = m_Engine.m_GearIndex >= 0;
		if (m_HasGears)
		{
			/*
			int gear = m_Controller.m_Gear[m_Engine.m_GearIndex];

			m_GearPreviousShow = false;
			m_GearNextShow = false;

			if (gear - 1 >= 0)
				m_GearPreviousShow = true;
			if (gear + 1 < m_Controller.m_GearCount[m_Engine.m_GearIndex])
				m_GearNextShow = true;

			if (m_GearPreviousShow)
				m_GearPrevious = m_Gears[gear - 1];

			m_GearCurrent = m_Gears[gear];

			if (m_GearNextShow)
				m_GearNext = m_Gears[gear + 1];
			*/
		}

		NotifyPropertyChanged("m_RPM_Rotation");
		NotifyPropertyChanged("m_HasGears");
		NotifyPropertyChanged("m_GearPreviousShow");
		NotifyPropertyChanged("m_GearNextShow");
		NotifyPropertyChanged("m_GearPrevious");
		NotifyPropertyChanged("m_GearCurrent");
		NotifyPropertyChanged("m_GearNext");
	}
};
#endif
