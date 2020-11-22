modded class Environment
{
	protected override bool OverridenHeatComfort(out float value)
	{
		if (super.OverridenHeatComfort(value))
			return true;

		if( IsInsideVehicle() )
		{
			if( m_Player.GetCommand_ExpansionVehicle() )
			{
				ExpansionVehicleBase cs = ExpansionVehicleBase.Cast(m_Player.GetCommand_ExpansionVehicle().GetTransport());
				if( cs )
				{
					value = cs.GetEnviroHeatComfortOverride();
				}
			
				return true;
			}
		}
		
		return false;
	}
};