class eAIDamageHandler
{
	EntityAI m_Entity;
	eAIEntityTargetInformation m_TargetInformation;
	bool m_ProcessDamage;
	int m_HitCounter;

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
			float damageMultiplier = 1.0;

			DayZPlayerImplement player;
			bool isPlayerItem;
			if (Class.CastTo(player, m_Entity.GetHierarchyRootPlayer()))
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
						eAIBase ai;
						if (Class.CastTo(ai, sourcePlayer))
						{
							if (sourcePlayer == m_Entity)
							{
								//! This shouldn't be possible because AI don't use suicide emote
								EXPrint(ToString() + " WARNING: Game encountered an impossible state (AI damage source is firearm in AI's own hands)");
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
