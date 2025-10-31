class eAIState_Vehicles: eAIState
{
	int OnUpdateEx(float DeltaTime, int SimulationPrecision, EntityAI entity, int seat)
	{
		auto group = unit.GetGroup();
		if (!group) return EXIT;
		if (!unit.IsInTransport() && unit.GetThreatToSelf() >= 0.4) return EXIT;
		
		auto leader = group.GetFormationLeader();
		//! @note leader can only briefly be null (disconnected/killed).
		//! We wait until a new leader has been determined (which may be the current unit).
		if (!leader) return CONTINUE;
		
		if (!entity) return EXIT;
		
		if ((leader != unit && leader.IsInTransport() && leader.GetParent() == entity) || unit.IsInTransport()) return CONTINUE;
		
		//! Wait until AI has left the vehicle and closed the door (if any)
		bool hasDoor;
		string source;
		ExpansionFSMHelper.DoorAnimationSource(entity, seat, hasDoor, source);
		if (hasDoor && entity.GetAnimationPhase(source) > 0.5) return CONTINUE;
		
		return EXIT;
	}

	int GuardEx(inout EntityAI entity, inout int seat)
	{
		auto group = unit.GetGroup();
		if (!group) return eAITransition.FAIL;
		
		if (group.GetFormationState() != eAIGroupFormationState.IN) return eAITransition.FAIL;
		
		auto leader = group.GetFormationLeader();
		if (!leader || leader == unit) return eAITransition.FAIL;
		
		if (!leader.IsInTransport()) return eAITransition.FAIL;
		
		CarScript car;
		if (!Class.CastTo(car, leader.GetParent())) return eAITransition.FAIL;
		
		//TODO: make this event based instead and store as a variable within CarScript.
		//Prevents looping through the crew and insteads just compares a bool.
		for (int i = 1; i < car.CrewSize(); i++)
		{
			if (car.CrewMember(i) == null && !car.Expansion_IsSeatReservedByOther(i, unit) && car.IsAreaAtDoorFree(i))
			{
				seat = i;
				entity = car;
				car.Expansion_ReserveSeat(i, unit);
				
				return eAITransition.SUCCESS;
			}
		}
		
		return eAITransition.FAIL;
	}

	int GuardIdle()
	{
		if (unit.IsInTransport()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
