class eAIDamageHandler
{
	static ref TStringArray s_HumanDmgZonesForRedirect = {
		"Torso",
		"LeftArm",
		"LeftHand",
		"RightArm",
		"RightHand",
		"LeftLeg",
		"LeftFoot",
		"RightLeg",
		"RightFoot"
	};

	EntityAI m_Entity;
	eAIEntityTargetInformation m_TargetInformation;
	bool m_ProcessDamage;
	int m_HitCounter;
	float m_LastHitTime;
	eAIBase m_LastSourceAI;

	void eAIDamageHandler(EntityAI entity, eAIEntityTargetInformation info)
	{
		m_Entity = entity;
		m_TargetInformation = info;
	}

	bool OnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
	#ifdef DIAG_DEVELOPER
		m_HitCounter++;

		EXTrace.PrintHit(EXTrace.AI, m_Entity, ToString() + "::OnDamageCalculated[" + m_HitCounter + "]", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (DayZPlayerImplement.s_eAI_DebugDamage && m_Entity.GetHierarchyRoot().IsMan())
		{
			//! Prevent death by health or blood dmg (can still bleed out from cuts)

			float dmg = damageResult.GetDamage(dmgZone, "Health");
			float transferToGlobalCoef = 1.0;

			if (dmgZone)
			{
				string path;

				if (m_Entity.IsMagazine())
					path = CFG_MAGAZINESPATH;
				else if (m_Entity.IsWeapon())
					path = CFG_WEAPONSPATH;
				else
					path = CFG_VEHICLESPATH;

				path += " " + m_Entity.GetType() + " DamageSystem DamageZones " + dmgZone + " Health transferToGlobalCoef";

				if (GetGame().ConfigIsExisting(path))
					transferToGlobalCoef = GetGame().ConfigGetFloat(path);
			}

			EXTrace.Print(EXTrace.AI, m_Entity, "Global damage: " + (dmg * transferToGlobalCoef));
			EXTrace.Print(EXTrace.AI, m_Entity, "Global health: " + m_Entity.GetHealth("", "Health"));

			if (dmgZone)
				EXTrace.Print(EXTrace.AI, m_Entity, dmgZone + " health: " + m_Entity.GetHealth(dmgZone, "Health"));

			if (dmg * transferToGlobalCoef >= Math.Floor(m_Entity.GetHealth("", "Health")))
				return false;

			if (m_Entity.IsMan())
			{
				if (dmgZone == "Head")
				{
					//! If head health goes to zero, character dies
					if (dmg >= Math.Floor(m_Entity.GetHealth(dmgZone, "Health")))
						return false;
				}

				//! Any damage to brain is certain death, HP don't matter
				if (dmgZone == "Brain")
					return false;

				if (Math.Floor(m_Entity.GetHealth("", "Blood")) - damageResult.GetDamage(dmgZone, "Blood") <= 2600)
					return false;
			}
		}
	#endif

		DayZPlayerImplement sourcePlayer;
		if (source)  //! Source can be null if actual source is not an EntityAI but (e.g.) a static object during a vehicle collision
		{
			Transport transport;
			if (Class.CastTo(transport, source))
				sourcePlayer = DayZPlayerImplement.Cast(transport.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER));
			else
				sourcePlayer = DayZPlayerImplement.Cast(source.GetHierarchyRootPlayer());
		}

		if (!m_ProcessDamage)
		{
			EntityAI rootEntity = m_Entity.GetHierarchyRoot();

			eAIBase ai;
			if (Class.CastTo(ai, sourcePlayer))
			{
				if (damageType == DT_FIRE_ARM)
				{
					EntityAI hitscanEntity = ai.m_eAI_HitscanEntity;

					//! Work-around for 1st shot on new entity hitting previously hit entity due to vanilla bug with Weapon::Fire
					if (hitscanEntity)
					{
						if (hitscanEntity.GetHierarchyRoot() != rootEntity && rootEntity.IsDamageDestroyed() && !ai.m_eAI_QueuedShots)
						{
							//! Only redirect for root entity, children will be dealt with by parent dmg handler

							if (rootEntity == m_Entity)
							{
								//! Redirect damage to correct entity

								//! Make sure that damage transfer to attachments works correctly
								if (hitscanEntity.IsMan())
								{
									//! Do not redirect consecutive hits (within 2 ms, well below fastest fire rate of any firearm) to human target.
									//! Consecutive hits can happen if entity is not wearing clothing items that could stop the projectile when it 
									//! penetrates, so to avoid checking clothing of hitscan entity, we just ignore the subsequent hit.
									//! An example of consecutive hits would be LeftArm followed by Torso from the same projectile if the shot 
									//! originated from the left side of entity.

									float time = GetGame().GetTickTime();

									if (ai == m_LastSourceAI && time - m_LastHitTime < 0.002)
									{
									#ifdef DIAG_DEVELOPER
										EXTrace.Print(EXTrace.AI, ai, "Wrong entity hit " + ExpansionStatic.GetHierarchyInfo(m_Entity) + ", ignoring dmg because last hit from " + ai + " was less than 2 ms ago");
									#endif
										return false;
									}

									m_LastHitTime = time;
									m_LastSourceAI = ai;

									if (!m_Entity.IsMan())
									{
										//! Do not redirect from non-human entity to Head or Brain, redirect to Torso instead
										if (s_HumanDmgZonesForRedirect.Find(dmgZone) == -1)
											dmgZone = "Torso";
									}
								}
								else if (hitscanEntity.IsZombie())
								{
									if (!m_Entity.IsZombie())
										dmgZone = "Head";
								}
								else if (hitscanEntity.IsAnimal())
								{
									if (!m_Entity.IsAnimal())
										dmgZone = "Zone_Head";
								}

								vector aiPos = ai.GetPosition();
								vector hitscanPos = hitscanEntity.GetPosition();

								float distSq = vector.DistanceSq(aiPos, m_Entity.GetPosition());
								float hitscanDistSq = vector.DistanceSq(aiPos, hitscanPos);

								if (Math.AbsFloat(distSq - hitscanDistSq) > 4.0)
								{
									//! Recalculate speed coef if distance difference of entity and hitscan entity to AI is more than 2 m

									Weapon_Base weapon;
									if (Class.CastTo(weapon, source))
									{
										//! Since there's no way to get damageOverride in script, calculated coef may be too low.
										//! Nothing we can do about it though.
										float dmgCoef = weapon.eAI_CalculateProjectileDamageCoefAtPosition(ammo, hitscanPos);
										if (dmgCoef < speedCoef)
											speedCoef = dmgCoef;
									}
									else
									{
										//! Should not be possible, but just in case
									#ifdef DIAG_DEVELOPER
										EXTrace.Print(EXTrace.AI, ai, "Wrong entity hit " + ExpansionStatic.GetHierarchyInfo(m_Entity) + " dist " + Math.Sqrt(distSq) + ", ignoring dmg because " + ExpansionStatic.GetHierarchyInfo(hitscanEntity) + " dist " + Math.Sqrt(hitscanDistSq) + " is too far and cannot determine AI weapon");
									#endif
										return false;
									}
								}

							#ifdef DIAG_DEVELOPER
								bool dbgObjEnabled = DayZPlayerImplement.s_Expansion_DebugObjects_Enabled;
								DayZPlayerImplement.s_Expansion_DebugObjects_Enabled = true;
								vector dir = vector.Direction(aiPos, modelPos);
								dir[1] = 0;
								ai.Expansion_DebugObject(-1, modelPos, "ExpansionDebugNoticeMe_Red", dir);
								dir = vector.Direction(aiPos, hitscanPos);
								dir[1] = 0;
								ai.Expansion_DebugObject(-2, hitscanPos, "ExpansionDebugNoticeMe", dir);
								DayZPlayerImplement.s_Expansion_DebugObjects_Enabled = dbgObjEnabled;
								EXTrace.Print(EXTrace.AI, ai, "Wrong entity hit " + ExpansionStatic.GetHierarchyInfo(m_Entity) + " dist " + Math.Sqrt(distSq) + ", redirecting dmg to " + ExpansionStatic.GetHierarchyInfo(hitscanEntity) + " dist " + Math.Sqrt(hitscanDistSq) + " " + dmgZone + " " + speedCoef);
							#endif

								hitscanEntity.ProcessDirectDamage(damageType, source, dmgZone, ammo, modelPos, speedCoef);
							}
						#ifdef DIAG_DEVELOPER
							else
							{
								EXTrace.Print(EXTrace.AI, ai, "Wrong entity hit " + ExpansionStatic.GetHierarchyInfo(m_Entity) + ", ignoring dmg");
							}
						#endif

							return false;
						}
					}
				}
			}

			float damageMultiplier = 1.0;

			DayZPlayerImplement player;
			bool isPlayerItem;
			if (Class.CastTo(player, rootEntity))
			{
				if (m_Entity.IsInventoryItem())
				{
					//! Entity is item on player

					if (player.m_eAI_DamageHandler.m_ProcessDamage)
						return true;  //! Process damage if player processes damage

					isPlayerItem = true;
				}

				if (player.IsAI())
					damageMultiplier *= player.m_eAI_DamageReceivedMultiplier;
			}

			if (sourcePlayer)
			{
				switch (damageType)
				{
					case DT_FIRE_ARM:
						if (ai)
						{
							if (sourcePlayer == m_Entity)
							{
								//! This shouldn't be possible because AI don't use suicide emote
								EXError.Warn(this, "WARNING: Game encountered an impossible state (AI damage source is firearm in AI's own hands)", {});
								return false;
							}

							damageMultiplier *= ai.m_eAI_DamageMultiplier;
						}

						break;

					case DT_CLOSE_COMBAT:
						eAIGroup group = sourcePlayer.GetGroup();
						if (group)
						{
							eAIFaction faction = group.GetFaction();
							float yeetForce;

							if (!isPlayerItem && !m_Entity.GetHierarchyParent())
							{
								eAIEntityTargetInformation info;

								if (player && player.Expansion_IsInVehicleCmd())
								{
									CarScript vehicle;
									if (Class.CastTo(vehicle, player.GetParent()))
										info = vehicle.GetTargetInformation();
								}
								else
								{
									info = m_TargetInformation;
								}

								if (info)
								{
									yeetForce = faction.GetMeleeYeetForce();
									eAIMeleeCombat.eAI_ApplyYeetForce(info, yeetForce, sourcePlayer.GetPosition(), faction.GetMeleeYeetFactors());
								}
							}

							//! @note for player targets, melee dmg mult above 1 and yeet are mutually exclusive
							//! since you can't send players flying in the moment of death
							float meleeDamageMultiplier = faction.GetMeleeDamageMultiplier();
							if (meleeDamageMultiplier < 1.0 || (meleeDamageMultiplier > 1.0 && (!player || (!isPlayerItem && !yeetForce))))
								damageMultiplier *= meleeDamageMultiplier;
						}

						break;
				}
			}

			if (damageMultiplier != 1.0)
			{
				damageMultiplier *= speedCoef;

			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.AI, m_Entity, ToString() + "::OnDamageCalculated[" + m_HitCounter + "] override dmg coef " + speedCoef + " -> " + damageMultiplier);
			#endif

				if (!isPlayerItem && damageMultiplier != 0.0)
				{
					//! Need to use Call() to avoid inconsistent damage
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(ProcessDamage, damageType, source, sourcePlayer, dmgZone, ammo, modelPos, damageMultiplier);
				}

				return false;
			}
		}
		else
		{
			m_ProcessDamage = false;
		}

		if (m_Entity.IsMan() && sourcePlayer && sourcePlayer != m_Entity)
			sourcePlayer.m_eAI_LastAggressionTime = ExpansionStatic.GetTime(true);  //! Aggro guards in area (if any)

		return true;
	}

	void ProcessDamage(int damageType, EntityAI source, DayZPlayerImplement sourcePlayer, string dmgZone, string ammo, vector modelPos, float damageCoef = 1.0)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.PrintHit(EXTrace.AI, m_Entity, ToString() + "::ProcessDamage[" + m_HitCounter + "]", null, damageType, source, -1, dmgZone, ammo, modelPos, damageCoef);
	#endif

		if (!source)
		{
			//! This can happen since this is not the same frame if (e.g.) AI weapon got deleted in the meantime due to being cloned

			if (sourcePlayer)
				source = sourcePlayer;
			else
				source = m_Entity;
		}

		m_ProcessDamage = true;
		m_Entity.ProcessDirectDamage(damageType, source, dmgZone, ammo, modelPos, damageCoef);
	}
}
