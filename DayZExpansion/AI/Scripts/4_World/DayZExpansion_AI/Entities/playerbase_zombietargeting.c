modded class PlayerBase
{
	protected bool m_Expansion_OverrideIsInVehicle;

	override bool IsInVehicle()
	{
		if (m_Expansion_OverrideIsInVehicle)
			return false;

		return super.IsInVehicle();
	}

	//! Allow targeting by zombies while in vehicle
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		ZombieBase zombie;
		if (Class.CastTo(zombie, ai) && zombie.GetExpansionInfectedType())
			return Expansion_OverrideIsInVehicle_CanBeTargetedByAI(ai);

		return super.CanBeTargetedByAI(ai);
	}

	bool Expansion_OverrideIsInVehicle_CanBeTargetedByAI(EntityAI ai)
	{
		m_Expansion_OverrideIsInVehicle = true;

		bool ret = super.CanBeTargetedByAI(ai);

		m_Expansion_OverrideIsInVehicle = false;

		return ret;
	}
}
