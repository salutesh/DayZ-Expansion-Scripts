class eAIState_Leave: eAIState
{
	int m_Phase;
	float m_Time;
	float m_Threshold;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Phase = 0;
		m_Time = 0;
		m_Threshold = Math.RandomFloat(0.5, 1.5);
		
		unit.SetAllowDamage(false);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		auto group = unit.GetGroup();

		if (m_Phase == 2 && group.m_ForcePatrolDespawn) return CONTINUE;

		if (unit.IsRaised()) unit.RaiseWeapon(false);

		m_Time += DeltaTime;

		if (m_Time < m_Threshold) return CONTINUE;

		unit.OverrideTargetPosition(unit.GetPosition());

		if (unit.IsSwimming())
		{
			group.m_ForcePatrolDespawn = true;
			return CONTINUE;
		}

		if (unit.eAI_IsChangingStance()) return CONTINUE;

		if (m_Phase < 2 && unit.GetEmoteManager().IsEmotePlaying()) return CONTINUE;

		if (m_Phase == 0)
		{
			m_Phase = 1;
			m_Time = 0;
			m_Threshold = 0;

			if (!unit.IsRestrained())
			{
				int emoteId;

				switch (Math.RandomInt(0, 3))
				{
					case 0:
						emoteId = EmoteConstants.ID_EMOTE_TAUNTELBOW;
						break;

					case 1:
						emoteId = EmoteConstants.ID_EMOTE_THROAT;
						break;

					case 2:
						emoteId = EmoteConstants.ID_EMOTE_DABBING;
						break;
				}

				unit.Expansion_SetEmote(emoteId);
				unit.Expansion_PlayEmote();

				return CONTINUE;
			}
		}

		if (m_Phase == 1)
		{
			m_Phase = 2;
			m_Time = 0;
			m_Threshold = 0;

			unit.Expansion_SetEmote(EmoteConstants.ID_EMOTE_SITA);
			unit.Expansion_PlayEmote();

			return CONTINUE;
		}

		if (m_Phase == 2 && m_Time < 4.5) return CONTINUE;  //! Wait for AI to sit

		group.m_ForcePatrolDespawn = true;

		return CONTINUE;
	}

	int Guard()
	{
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		
		auto group = unit.GetGroup();
		if (!group) return eAITransition.FAIL;
		if (!group.CanLeave()) return eAITransition.FAIL;

		if (unit.IsClimbing()) return eAITransition.FAIL;
		if (unit.IsClimbingLadder()) return eAITransition.FAIL;
		if (unit.IsFalling()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
}
