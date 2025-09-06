modded class CarScript
{
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		if (!super.CanBeTargetedByAI(ai))
			return false;

		ZombieBase zombie;
		if (Class.CastTo(zombie, ai) && zombie.GetExpansionInfectedType())
		{
			for (int i = 0; i < CrewSize(); ++i)
			{
				PlayerBase pb;
				if (Class.CastTo(pb, CrewMember(i)))
				{
					//! Allow targeting vehicle if a targetable player is inside
					if (pb.Expansion_OverrideIsInVehicle_CanBeTargetedByAI(ai))
						return true;
				}
			}

			//! Disallow targeting vehicle if no targetable player inside and velocity is below 1 km/h
			if (dBodyIsActive(this) && GetVelocity(this).LengthSq() < 0.277778)
				return false;
		}

		return true;
	}
}
