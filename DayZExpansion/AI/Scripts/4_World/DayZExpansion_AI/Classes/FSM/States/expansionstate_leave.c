class ExpansionState_Leave: eAIState
{
	int phase;
	float time;
	float threshold;

	override void OnEntry(string Event, ExpansionState From)
	{
		phase = 0;
		time = 0;
		threshold = Math.RandomFloat(0.5, 1.5);
		
		unit.SetAllowDamage(false);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		auto group = unit.GetGroup();

		if (phase == 2 && group.m_ForcePatrolDespawn) return CONTINUE;

		if (unit.IsRaised()) unit.RaiseWeapon(false);

		time += DeltaTime;

		if (time < threshold) return CONTINUE;

		unit.OverrideTargetPosition(unit.GetPosition());

		if (unit.IsSwimming())
		{
			group.m_ForcePatrolDespawn = true;
			return CONTINUE;
		}

		if (unit.IsClimbing()) return CONTINUE;
		if (unit.IsFalling()) return CONTINUE;
		if (unit.eAI_IsChangingStance()) return CONTINUE;

		if (phase < 2 && unit.GetEmoteManager().IsEmotePlaying()) return CONTINUE;

		if (phase == 0)
		{
			phase = 1;
			time = 0;
			threshold = 0;

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

		if (phase == 1)
		{
			phase = 2;
			time = 0;
			threshold = 0;

			unit.Expansion_SetEmote(EmoteConstants.ID_EMOTE_SITA);
			unit.Expansion_PlayEmote();

			return CONTINUE;
		}

		if (phase == 2 && time < 4.5) return CONTINUE;  //! Wait for AI to sit

		group.m_ForcePatrolDespawn = true;

		return CONTINUE;
	}
}
