//! This ONLY exists so 3rd party mods still referencing the type don't break
//! (e.g. COT)
class ExpansionVehicleBase: Transport
{
	[Obsolete("ExpansionVehicleBase will be removed")]
	static set<ExpansionVehicleBase> GetAll()
	{
		return new set<ExpansionVehicleBase>;
	}
	
	[Obsolete("ExpansionVehicleBase will be removed")]
	ExpansionVehicle GetExpansionVehicle()
	{
		return null;
	}
}
