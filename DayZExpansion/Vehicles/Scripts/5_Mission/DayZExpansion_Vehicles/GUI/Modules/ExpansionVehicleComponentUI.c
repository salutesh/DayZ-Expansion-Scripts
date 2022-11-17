#ifdef CF_MVVM
class ExpansionVehicleComponentUI : CF_Model
{
	ExpansionVehicleBase m_Vehicle;
	ExpansionVehicleComponent m_Module;

	void ExpansionVehicleComponentUI(ExpansionVehicleBase vehicle, ExpansionVehicleComponent module, Widget parent)
	{
		m_Vehicle = vehicle;
		m_Module = module;

		CF_MVVM.Create(this, GetLayoutFile(), parent);
	}

	void OnUpdate(float pDt)
	{

	}
};
#endif
