class eAIState_Fighting_FireWeapon: eAIState
{
	float m_Time;
	eAITarget m_Target;
	Weapon_Base m_Weapon;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.RaiseWeapon(true);
		
		m_Time = 0;
		
		unit.m_eAI_LastFireTime = g_Game.GetTime();
		
		bool adjustStance = unit.eAI_AdjustStance(m_Weapon, unit.m_eAI_DistanceToTargetSq);
		
		#ifdef DIAG
		if (adjustStance)
			EXTrace.Print(EXTrace.AI, unit, "eAI_AdjustStance " + typename.EnumToString(eAIStance, unit.eAI_GetStance()));
		#endif
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (!m_Target)
			return EXIT;
		
		auto lowPosition = m_Target.GetPosition(false);
		auto aimPosition = lowPosition + m_Target.GetAimOffset();
		
		m_Time += DeltaTime;
		unit.OverrideTargetPosition(m_Target);
		
		if (!unit.IsRaised() || !unit.IsWeaponRaiseCompleted())
		{
			unit.RaiseWeapon(true);
		
			if (m_Time >= 0.5)
			{
				m_Time = 0;
				return EXIT;
			}
		
			// waiting for the weapon to be raised
			return CONTINUE;
		}
		
		auto neck = unit.GetBonePositionWS(unit.GetBoneIndexByName("neck"));
		auto direction = vector.Direction(neck, aimPosition).Normalized();
		auto weapon = unit.GetHumanInventory().GetEntityInHands();
		
		float threshold;
		if (weapon && weapon.ShootsExplosiveAmmo())
			threshold = 0.96875;
		else
			threshold = 0.875;
		
		if (vector.Dot(unit.GetAimDirection(), direction) < threshold)
		{
			if (m_Time >= 0.5)
			{
				m_Time = 0;
				return EXIT;
			}
		
			// waiting for unit to face target
			return CONTINUE;
		}
		
		unit.TryFireWeapon();
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsRestrained()) return eAITransition.FAIL;
		
		// we are aiming at something?
		m_Target = unit.GetTarget();
		if (!m_Target) return eAITransition.FAIL;
		
		PlayerBase player;
		EntityAI targetEntity = m_Target.GetEntity();
		ItemBase itemTarget;
		float dist = m_Target.GetDistance();
		if (!targetEntity)
		{
			return eAITransition.FAIL;
		}
		else if (Class.CastTo(player, targetEntity))
		{
			if (player.IsUnconscious())
				return eAITransition.FAIL;
		}
		else if (Class.CastTo(itemTarget, targetEntity) && !itemTarget.Expansion_IsDanger())
		{
			return eAITransition.FAIL;
		}
		
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		if (!Class.CastTo(m_Weapon, unit.GetItemInHands()) || m_Weapon.IsDamageDestroyed()) return eAITransition.FAIL;
		
		if (!unit.CanRaiseWeapon() || !m_Target.m_LOS) return eAITransition.FAIL;
		
		if (!m_Weapon.Expansion_IsChambered()) return eAITransition.FAIL;
		
		if (unit.GetWeaponManager().CanUnjam(m_Weapon)) return eAITransition.FAIL;
		
		float minDist;
		
		if (m_Weapon.ShootsExplosiveAmmo())
			minDist = m_Weapon.Expansion_GetMinSafeFiringDistance();
		
		if (m_Target.IsExplosive())
			minDist = Math.Max(m_Target.GetMinDistance(), minDist);
		
		if (minDist)
		{
			//! Avoid firing if within minDist
			if (dist < minDist) return eAITransition.FAIL;
			
			//! Avoid firing if other friendly units within minDist from tgt
			float minDistSq = minDist * minDist;
			
			vector aimPosition = m_Target.GetPosition() + m_Target.GetAimOffset();
			
			vector min = Vector(aimPosition[0] - minDist, aimPosition[1] - minDist, aimPosition[2] - minDist);
			vector max = Vector(aimPosition[0] + minDist, aimPosition[1] + minDist, aimPosition[2] + minDist);
			
			array<EntityAI> entities = {};
			
			DayZPlayerUtils.SceneGetEntitiesInBox(min, max, entities);
			
			vector hitPos;
			vector hitDir;
			int hitComponent;
			set<Object> results = new set<Object>;
			
			foreach (EntityAI entity: entities)
			{
				vector entityCenter = entity.GetCenter();
				
				if (vector.DistanceSq(entityCenter, aimPosition) > minDistSq)
					continue;
				
				if (!unit.PlayerIsEnemy(entity))
				{
					//! Friendly unit in blast radius
					if (DayZPhysics.RaycastRV(aimPosition, entityCenter, hitPos, hitDir, hitComponent, results, null, targetEntity, false, false, ObjIntersectFire, 0.2))
					{
						Object parentObject;
						
						if (results.Find(entity) > -1 || (Class.CastTo(parentObject, entity.GetParent()) && results.Find(parentObject) > -1))
						{
							if (!unit.IsRaised())
								unit.RaiseWeapon(true);
							
							return eAITransition.FAIL;  //! Friendly unit in blast radius and nothing blocking the blast
						}
					}
				}
			}
		}
		
		return eAITransition.SUCCESS;
	}
};
