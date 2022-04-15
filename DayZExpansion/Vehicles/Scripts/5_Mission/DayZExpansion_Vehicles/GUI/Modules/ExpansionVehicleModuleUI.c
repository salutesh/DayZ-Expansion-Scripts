#ifdef CF_MVVM
class ExpansionVehicleModuleUI : CF_Model
{
	ExpansionVehicleBase m_Vehicle;
	ExpansionVehicleModule m_Module;
	ExpansionController m_Controller;

	void ExpansionVehicleModuleUI(ExpansionVehicleBase vehicle, ExpansionVehicleModule module, Widget parent)
	{
		m_Vehicle = vehicle;
		m_Module = module;
		m_Controller = m_Vehicle.m_Controller;

		CF_MVVM.Create(this, GetLayoutFile(), parent);
	}

	void OnUpdate(float pDt)
	{

	}
};
#endif
