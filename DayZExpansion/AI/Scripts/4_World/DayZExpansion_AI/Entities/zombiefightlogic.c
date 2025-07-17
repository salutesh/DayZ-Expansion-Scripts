/**
 * @brief Enables zombies hitting vehicles and players inside
 * 
 * ONLY deals with the case not handled by vanilla (player in vehicle or target is vehicle)
 * 
 * @note car doors/windows will protect players until destroyed/broken
 */
class ExpansionZombieFightLogic
{
	protected static ref array<typename> s_TargetableTypes = {PlayerBase, AnimalBase, CarScript};

	protected ZombieBase m_Zombie;
	protected EntityAI m_OriginalTarget;
	protected EntityAI m_RecentTarget;

	void ExpansionZombieFightLogic(ZombieBase zombie)
	{
		m_Zombie = zombie;
	}

	protected bool TargetSelection(vector origin, vector dirNorm, float maxDist, out vector ray, out vector hitPos, out int component, out EntityAI hitEnt, Object ignore = null, float radius = 0.0, CollisionFlags flags = CollisionFlags.NEARESTCONTACT, bool validateDistance = false)
	{
		vector begPos = origin + dirNorm * radius;
		vector endPos = begPos + dirNorm * (maxDist - radius * 2);
		
		RaycastRVParams params = new RaycastRVParams(begPos, endPos, ignore, radius);
		params.with = m_Zombie;
		params.flags = flags;
		params.type = ObjIntersectIFire;
		if (flags != CollisionFlags.NEARESTCONTACT && flags != CollisionFlags.FIRSTCONTACT)
			params.sorted = true;  //! @note sorted by distance in descending order!

		array<ref RaycastRVResult> results = {};
		array<Object> excluded;
		if (ignore)
			excluded = {ignore};

		ray = vector.Zero;
		hitPos = vector.Zero;
		component = -1;
		hitEnt = null;

		if (DayZPhysics.RaycastRVProxy(params, results, excluded))
		{
			/**
			 * @note raycast result entries are sometimes improperly sorted due to hit position not being correct.
			 * We ignore these entries if validateDistance = true (only for attachments).
			 * 
			 * 9 HatchbackDoors_CoDriver<4cd9f170> hierLevel=1 distance=0.00109183  <-- incorrect (raycast from driver side)
			 * 8 OffroadHatchback<b1401410> hierLevel=0 distance=0.451828
			 * 7 OffroadHatchback<b1401410> hierLevel=0 distance=0.456719
			 * 6 OffroadHatchback<b1401410> hierLevel=0 distance=1.62744
			 * 5 SurvivorBase<795d5160> hierLevel=0 distance=1.70653
			 * 4 SurvivorBase<795d5160> hierLevel=0 distance=1.79095
			 * 3 SurvivorBase<795d5160> hierLevel=0 distance=1.83849
			 * 2 SurvivorBase<795d5160> hierLevel=0 distance=1.84204
			 * 1 HatchbackDoors_CoDriver<4cd9f170> hierLevel=1 distance=2.32233
			 * 0 HatchbackDoors_CoDriver<4cd9f170> hierLevel=1 distance=2.36941
			 */

			int count = results.Count();
			RaycastRVResult result;
			RaycastRVResult selected;
			EntityAI entity;

		#ifdef DIAG_DEVELOPER
			bool dbg;
			if (radius == 0)
				dbg = true;
			if (dbg)
				PrintFormat("%1 <<< RAY", m_Zombie);
		#endif

			for (int i = count - 1; i >= 0; --i)
			{
				result = results[i];

			#ifdef DIAG_DEVELOPER
				if (dbg)
					PrintFormat("   %1 %2 hierLevel=%3 distance=%4", i, result.obj, result.hierLevel, vector.Distance(origin, result.pos));
			#endif

				if (!result.obj)
					continue;

				if (result.obj.IsBush())
					continue;

				if (result.obj.IsTree() || result.obj.IsBuilding())
					break;

				if (!Class.CastTo(entity, result.obj))
					continue;

				EntityAI root = entity.GetHierarchyRoot();

				foreach (typename targetableType: s_TargetableTypes)
				{
					if (root.IsInherited(targetableType))
					{
						if (result.pos == origin)
						{
						#ifdef DIAG_DEVELOPER
							PrintFormat("%1 SKIP %2 parent %3 pos == origin", m_Zombie, result.obj, result.parent);
						#endif
							continue;
						}

						if (!hitEnt || (validateDistance && result.hierLevel > 0 && result.parent != selected.obj && vector.DistanceSq(origin, entity.GetPosition()) < vector.DistanceSq(origin, hitEnt.GetPosition())))
						{
						#ifdef DIAG_DEVELOPER
							if (dbg && selected)
								PrintFormat("%1 OVERRIDE %2 hierLevel=%3 distance=%4 -> %5 hierLevel=%6 distance=%7", m_Zombie, selected.obj, selected.hierLevel, vector.Distance(origin, hitEnt.GetPosition()), result.obj, result.hierLevel, vector.Distance(origin, entity.GetPosition()));
						#endif

							hitEnt = entity;
							selected = result;

							if (!validateDistance)
								break;
						}
					}
				}
			}

		#ifdef DIAG_DEVELOPER
			if (dbg)
			{
				PrintFormat("%1 RAY >>>", m_Zombie);
				if (validateDistance && selected)
					PrintFormat("%1 SELECTED %2 hierLevel=%3 distance=%4", m_Zombie, selected.obj, selected.hierLevel, vector.Distance(origin, selected.pos));
			}

			if (count > 0 && params.flags != CollisionFlags.NEARESTCONTACT)
			{
				params.flags = CollisionFlags.NEARESTCONTACT;
				array<ref RaycastRVResult> dbgResults = {};
				if (DayZPhysics.RaycastRVProxy(params, dbgResults))
				{
					result = dbgResults[0];
					if (result.obj != results[count - 1].obj)
					{
						if (dbg)
							PrintFormat("%1 NEAREST %2 hierLevel=%3 distance=%4", m_Zombie, result.obj, result.hierLevel, vector.Distance(origin, result.pos));
					}
				}
			}
		#endif

			if (selected)
			{
				hitPos = selected.pos;
				component = selected.component;
				ray = hitPos - origin;

				return true;
			}
		}

		return false;
	}

	bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_ATTACK)
		{
			//! @note calling DayZInfectedCommandAttack::WasHit RESETS its return value internally, so after calling it ONCE,
			//! it will return false on subsequent invocations! This means we can't call the vanilla FightLogic first
			//! but instead have to early return on all cases that should be handled by it

			if (!m_OriginalTarget)
				return false;

			PlayerBase pb = PlayerBase.Cast(m_Zombie.m_ActualTarget);

			//! Let vanilla handle player targets that are not in vehicle command
			if (pb && !pb.GetCommand_Vehicle())
				return false;

			//! Let vanilla handle non-transport targets if not a player
			if (!pb && !m_Zombie.m_ActualTarget.IsTransport())
				return false;

			DayZInfectedCommandAttack attackCommand = m_Zombie.GetCommand_Attack();
			if (attackCommand && attackCommand.WasHit())
			{
			#ifdef DIAG_DEVELOPER
				PrintFormat("%1 attack type=%2 hit", m_Zombie, m_Zombie.m_ActualAttackType.m_Type);
			#endif

				vector origin;
				EntityAI target;
				vector targetPos;

				if (m_Zombie.IsCrawling())
					origin = m_Zombie.GetPosition() + "0 0.3 0";
				else
					origin = m_Zombie.GetPosition() + "0 0.8 0";

				Transport transport;
				if (Class.CastTo(transport, m_OriginalTarget))
				{
					float minDist = float.MAX;
					Human closest;

					for (int i = 0; i < transport.CrewSize(); ++i)
					{
						Human crewMember =  transport.CrewMember(i);
						if (crewMember)
						{
							float distSq = vector.DistanceSq(origin, crewMember.GetPosition());
							if (distSq < minDist)
							{
								minDist = distSq;
								closest = crewMember;
							}
						}
					}

					if (closest)
						target = closest;
					else
						target = m_OriginalTarget;
				}
				else
				{
					target = m_OriginalTarget;
				}

				m_OriginalTarget = null;

			#ifdef DIAG_DEVELOPER
				Class.CastTo(pb, target);
				TStringArray dbgSpheres = {
					"ExpansionDebugSphereSmall",
					"ExpansionDebugSphereSmall_Orange"
				};
				int hash = m_Zombie.ToString().Hash();
			#endif

				targetPos = target.GetPosition() + "0 0.3 0";

				vector dir = vector.Direction(origin, targetPos);

				//! Prevent targeting of objects behind zombie
				vector dirY0 = Vector(dir[0], 0, dir[2]);
				float thresh = 0.0;
				if (vector.Dot(m_Zombie.GetDirection(), dirY0.Normalized()) < thresh)
					return false;

				vector dirNorm = dir.Normalized();

				float maxDist = m_Zombie.m_ActualAttackType.m_Distance + 0.3;

				float radius = 0.0;  //! Radius 0, because it influences hitPos and thus where hit effects are shown
				vector ray;
				vector hitPosWS;
				int component;
				EntityAI hitEnt;
				array<ref Param3<vector, int, EntityAI>> hits = {};
				hits.Reserve(2);

				//! re-target (enemy can have moved out of range or disappeared)
				if (TargetSelection(origin, dirNorm, maxDist, ray, hitPosWS, component, hitEnt, m_Zombie, radius, CollisionFlags.ALLOBJECTS, true))
				{
					hits.Insert(new Param3<vector, int, EntityAI>(hitPosWS, component, hitEnt));

					string windowDmgZone = "";
					bool damageWindow;

					if (hitEnt.IsInherited(CarDoor) && Math.RandomInt(0, 2))
					{
						DamageZoneMap zoneMap = hitEnt.GetEntityDamageZoneMap();

						if (zoneMap.Contains("Window"))
						{
							windowDmgZone = "Window";
						}
						else
						{
							foreach (string zone, array<string> components: zoneMap)
							{
								if (zone.Contains("Window"))
								{
									windowDmgZone = zone;
									break;
								}
							}
						}

						if (windowDmgZone)
						{
							//! Allow hitting crew through destroyed window (if any)
							if (hitEnt.GetHealth("Window", "Health") == 0)
							{
								maxDist = Math.Min(0.6, maxDist - ray.Length());
								EntityAI hitEnt2;
								if (maxDist > 0 && TargetSelection(hitPosWS, dirNorm, maxDist, ray, hitPosWS, component, hitEnt2, hitEnt.GetHierarchyRoot(), radius))
								{
									if (hitEnt2.GetHierarchyRoot().IsMan())
										hits.Insert(new Param3<vector, int, EntityAI>(hitPosWS, component, hitEnt2));
								}
							}
							else
							{
								damageWindow = true;
							}
						}
					}

					foreach (int j, auto hit: hits)
					{
						hitPosWS = hit.param1;
						component = hit.param2;
						hitEnt = hit.param3;

						string dmgZone;

						EntityAI root = hitEnt.GetHierarchyRoot();
						if (!root.IsMan())
						{
							if (hitEnt.IsInherited(CarDoor))
							{
								if (damageWindow)
								{
									dmgZone = windowDmgZone;
									hitPosWS = hitEnt.GetDamageZonePos(dmgZone);

									if (hitEnt.GetHierarchyParent())
										hitPosWS = hitEnt.ModelToWorld(hitPosWS);
								}
								else
								{
									dmgZone = hitEnt.GetDamageZoneNameByComponentIndex(component);
								}
							}
							else
							{
								dmgZone = hitEnt.GetDamageZoneNameByComponentIndex(component);
							}

							TStringArray componentNames = {};
							DamageSystem.GetComponentNamesFromDamageZone(hitEnt, dmgZone, componentNames);
							foreach (string componentName: componentNames)
							{
							#ifdef DIAG_DEVELOPER
								string health = (hitEnt.GetHealth01(dmgZone, "Health") * 100).ToString() + "%%";
								PrintFormat("%1 HIT %2 hitComponent=%3 dmgZone=%4 health=%5 pos=%6", m_Zombie, hitEnt, componentName, dmgZone, health, hitPosWS.ToString());
							#endif

								DamageSystem.CloseCombatDamageName(m_Zombie, hitEnt, componentName, m_Zombie.m_ActualAttackType.m_AmmoType, hitPosWS);
							}

							CarScript cs;
							if (!hitEnt.IsInherited(CarWheel) && (Class.CastTo(cs, root) || Class.CastTo(cs, root.GetParent())) && GetExpansionSettings().GetAI().EnableZombieVehicleAttackPhysics)
							{
								float bodyMass = dBodyGetMass(cs);
								float invBodyMass = 1.0 / bodyMass;
								//! Zombies have a mass of 70.8333 (for comparison: players have a mass of 87.5)
								float force = bodyMass * Math.Min(dBodyGetMass(m_Zombie) * 3, 300) * cs.GetPushForceCoefficientMultiplier();
								//vector impulse 	= dirNorm * force;
								vector impulse 	= vector.Up * force;  //! Up vector gives the proper shaking up the vehicle feel
								impulse 		= impulse * invBodyMass;

								dBodyApplyImpulseAt(cs, impulse, hitPosWS);
							}
						}
						else
						{
							string hitComponentName = root.GetHitComponentForAI();

							string componentNameLower = hitComponentName;
							componentNameLower.ToLower();
							DamageSystem.GetDamageZoneFromComponentName(root, componentNameLower, dmgZone);
							hitPosWS = root.GetDamageZonePos(dmgZone);

						#ifdef DIAG_DEVELOPER
							string bodyPartHealth = (root.GetHealth01(dmgZone, "Health") * 100).ToString() + "%%";
							PrintFormat("%1 HIT %2 hitComponent=%3 dmgZone=%4 health=%5 pos=%6", m_Zombie, root, hitComponentName, dmgZone, bodyPartHealth, hitPosWS.ToString());
						#endif

							DamageSystem.CloseCombatDamageName(m_Zombie, root, hitComponentName, m_Zombie.m_ActualAttackType.m_AmmoType, hitPosWS);
						}

					#ifdef DIAG_DEVELOPER
						if (pb)
							pb.Expansion_DebugObject(hash + j, hitPosWS, dbgSpheres[j], vector.Zero, origin, 3.0, ShapeFlags.NOZBUFFER);
					#endif
					}

				#ifdef DIAG_DEVELOPER
					DestroyDebugObjects(pb, hits.Count());
				#endif

					return true;
				}
			#ifdef DIAG_DEVELOPER
				else
				{
					PrintFormat("%1 target selection failed (maxDist %2)", m_Zombie, maxDist);
					DestroyDebugObjects(pb, 1);
					if (pb)
						pb.Expansion_DebugObject(hash, origin + dirNorm * maxDist, "ExpansionDebugSphereSmall_Red", vector.Zero, origin, 3.0, ShapeFlags.NOZBUFFER);
				}
			#endif
			}
		}

		return false;
	}

	void DestroyDebugObjects(PlayerBase pb, int index)
	{
		if (pb)
		{
			int max = 2;

			while (index >= max)
			{
				max++;
			}

			for (int i = index; i < max; ++i)
			{
				int hash = m_Zombie.ToString().Hash() + index;
				Object dbgObj = pb.m_Expansion_DebugObjects[hash];
				if (dbgObj)
					pb.Expansion_DebugObject(hash, vector.Zero, dbgObj.GetType());
			}
		}
	}

	bool AttackLogic(DayZInfectedAttackGroupType type, float angle, DayZInfectedInputController pInputController, float pDt)
	{
		m_Zombie.m_ActualTarget = pInputController.GetTargetEntity();

		if (!m_Zombie.m_ActualTarget)
		{
			if (!m_RecentTarget || vector.DistanceSq(m_Zombie.GetPosition(), m_RecentTarget.GetPosition()) > 10000)
			{
				m_OriginalTarget = null;
				return false;
			}

			m_Zombie.m_ActualTarget = m_RecentTarget;
		}
		else
		{
			m_RecentTarget = m_Zombie.m_ActualTarget;
		}

		PlayerBase pb;
		if (!m_Zombie.m_ActualTarget.IsTransport())
		{
			if (!Class.CastTo(pb, m_Zombie.m_ActualTarget) || !pb.GetCommand_Vehicle())
			{
				//! If no player target or player not in vehicle, return
				return false;
			}
		}

		vector origin;
		vector targetPos;

		if (m_Zombie.IsCrawling())
			origin = m_Zombie.GetPosition() + "0 0.3 0";
		else
			origin = m_Zombie.GetPosition() + "0 0.8 0";

		targetPos = m_Zombie.m_ActualTarget.GetPosition() + "0 0.3 0";

		vector dir = vector.Direction(origin, targetPos);

		if (dir.LengthSq() > 19.36)
		{
		#ifdef DIAG_DEVELOPER
			DestroyDebugObjects(pb, 0);
			DestroyDebugObjects(pb, 2);
		#endif
			return false;
		}

		vector dirNorm = dir.Normalized();

		float maxDist = 2.4;

		float radius = 0.3;
		vector ray;
		vector hitPosWS;
		int component;
		EntityAI hitEnt;
		vector zombieDir = m_Zombie.GetDirection();
		float zombieAngle = zombieDir.VectorToAngles()[0];

	#ifdef DIAG_DEVELOPER
		string dbgObjType = "ExpansionDebugConeSmall";
		Object dbgObj;
		int dbgIndex_Show = 2;
		int dbgIndex_Hide = 3;
		int hash = m_Zombie.ToString().Hash();
	#endif

		if (TargetSelection(origin, dirNorm, maxDist, ray, hitPosWS, component, hitEnt, m_Zombie, radius, CollisionFlags.ALLOBJECTS))
		{
			//! Prevent targeting of objects behind zombie
			float targetAngle = dir.VectorToAngles()[0];
			float angleDiff = ExpansionMath.AngleDiff2(zombieAngle, targetAngle);
			if (Math.AbsFloat(angleDiff) > 60)
			{
				if (pInputController.GetMovementSpeed() == 0)
				{
					//! Help rotate zombie towards target
					m_Zombie.SetOrientation(Vector(ExpansionMath.RelAngle(zombieAngle + angleDiff * pDt * 3), 0, 0));
				#ifdef DIAG_DEVELOPER
					if (pb)
					{
						dbgObjType = "ExpansionDebugConeSmall_Red";
						dbgIndex_Hide = 2;
						dbgIndex_Show = 3;
					}
				#endif
				}
			#ifdef DIAG_DEVELOPER
				if (pb)
				{
					dbgObj = pb.m_Expansion_DebugObjects[hash + dbgIndex_Hide];
					if (dbgObj)
						pb.Expansion_DebugObject(hash + dbgIndex_Hide, vector.Zero, dbgObj.GetType());
					dbgObj = pb.Expansion_DebugObject(hash + dbgIndex_Show, m_Zombie.GetBonePositionWS(m_Zombie.GetBoneIndexByName("Head")) + "0 0.3 0", dbgObjType);
					if (dbgObj)
						dbgObj.SetOrientation(Vector(zombieAngle - 180, 90, 0));
				}
			#endif
				return false;
			}

		#ifdef DIAG_DEVELOPER
			if (pb)
			{
				dbgObj = pb.m_Expansion_DebugObjects[hash + dbgIndex_Hide];
				if (dbgObj)
					pb.Expansion_DebugObject(hash + dbgIndex_Hide, vector.Zero, dbgObj.GetType());
				dbgObj = pb.Expansion_DebugObject(hash + dbgIndex_Show, m_Zombie.GetBonePositionWS(m_Zombie.GetBoneIndexByName("Head")) + "0 0.3 0", dbgObjType);
				if (dbgObj)
					dbgObj.SetOrientation(Vector(zombieAngle - 180, 90, 0));
			}
		#endif

			m_OriginalTarget = m_Zombie.m_ActualTarget;

			if (pb && hitEnt.GetHierarchyRoot() != pb)
			{
				//! Override actual target to prevent attack damage handling in vanilla FightLogic
				//! (vanilla FightLogic does nothing if in attack command and target is non-alignable like vehicles)
				m_Zombie.m_ActualTarget = pb.GetCommand_Vehicle().GetTransport();
			}

		#ifdef DIAG_DEVELOPER
			string dbg = string.Format("%1 %2 %3", m_Zombie, typename.EnumToString(DayZInfectedAttackGroupType, type), m_Zombie.m_ActualTarget);
		#endif

			float dist = ray.Length();
			int pitch;
			if (type == DayZInfectedAttackGroupType.CHASE)
				pitch = -1;
			else
				pitch = m_Zombie.GetAttackPitch(m_Zombie.m_ActualTarget);

			if (Math.RandomInt(0, 2))
				m_Zombie.m_ActualAttackType = m_Zombie.GetDayZInfectedType().ChooseAttack(type, dist, pitch);
			else
				m_Zombie.m_ActualAttackType = null;

			if (!m_Zombie.m_ActualAttackType)
				m_Zombie.m_ActualAttackType = m_Zombie.GetExpansionInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, 1.7, 0);

			if (m_Zombie.m_ActualAttackType)
			{
			#ifdef DIAG_DEVELOPER
				DayZInfectedAttackType attackType = m_Zombie.m_ActualAttackType;
				PrintFormat("%1 distance=%2 pitch=%3 type=%4 subtype=%5 %6 heavy=%7 cooldown=%8 probability=%9", dbg, attackType.m_Distance, attackType.m_Pitch, attackType.m_Type, attackType.m_Subtype, attackType.m_AmmoType, attackType.m_IsHeavy, attackType.m_Cooldown, attackType.m_Probability);
			#endif

				m_Zombie.m_ActualAttackType.m_Distance = maxDist;

				m_Zombie.StartCommand_Attack(m_Zombie.m_ActualTarget, m_Zombie.m_ActualAttackType.m_Type, m_Zombie.m_ActualAttackType.m_Subtype);
				m_Zombie.m_AttackCooldownTime = m_Zombie.m_ActualAttackType.m_Cooldown;
				return true;
			}
		#ifdef DIAG_DEVELOPER
			else
			{
				PrintFormat("%1 no attack type for dist=%2 pitch=%3", dbg, dist, pitch);
			}
		#endif
		}

	#ifdef DIAG_DEVELOPER
		if (pb)
		{
			dbgObj = pb.m_Expansion_DebugObjects[hash + dbgIndex_Hide];
			if (dbgObj)
				pb.Expansion_DebugObject(hash + dbgIndex_Hide, vector.Zero, dbgObj.GetType());
			dbgObj = pb.Expansion_DebugObject(hash + dbgIndex_Show, m_Zombie.GetBonePositionWS(m_Zombie.GetBoneIndexByName("Head")) + "0 0.3 0", dbgObjType);
			if (dbgObj)
				dbgObj.SetOrientation(Vector(zombieAngle - 180, 90, 0));
			DestroyDebugObjects(pb, 1);
			pb.Expansion_DebugObject(hash, origin + dirNorm * maxDist, "ExpansionDebugSphereSmall_Red", vector.Zero, origin, 3.0, ShapeFlags.NOZBUFFER);
		}
	#endif

		return false;
	}

	bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		return AttackLogic(DayZInfectedAttackGroupType.CHASE, m_Zombie.TARGET_CONE_ANGLE_CHASE, pInputController, pDt);
	}
	
	bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		PlayerBase pb = PlayerBase.Cast(pInputController.GetTargetEntity());
		if (pb && pb.GetCommand_Vehicle())
		{
			//! We only need to decrement attack cooldown if actual target is player in vehicle,
			//! else vanilla FightAttackLogic will handle it
			if (m_Zombie.m_AttackCooldownTime > 0)
			{
				m_Zombie.m_AttackCooldownTime -= pDt * GameConstants.AI_ATTACKSPEED;
				return false;
			}
		}

		return AttackLogic(DayZInfectedAttackGroupType.FIGHT, m_Zombie.TARGET_CONE_ANGLE_FIGHT, pInputController, pDt);
	}
}
