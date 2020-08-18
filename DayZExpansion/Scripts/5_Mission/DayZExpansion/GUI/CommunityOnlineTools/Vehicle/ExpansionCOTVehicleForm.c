class ExpansionCOTVehicleForm: ExpansionCOTFormBase
{
	private ExpansionCOTVehicleModule m_Module;

	override bool SetExpansionModule( ref ExpansionCOTModuleBase mdl )
	{
		return Class.CastTo( m_Module, mdl );
	}
};