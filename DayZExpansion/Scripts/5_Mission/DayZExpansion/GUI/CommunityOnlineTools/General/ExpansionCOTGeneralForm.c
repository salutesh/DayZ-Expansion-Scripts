class ExpansionCOTGeneralForm: ExpansionCOTFormBase
{
	private ExpansionCOTGeneralModule m_Module;

	override bool SetExpansionModule( ref ExpansionCOTModuleBase mdl )
	{
		return Class.CastTo( m_Module, mdl );
	}
};