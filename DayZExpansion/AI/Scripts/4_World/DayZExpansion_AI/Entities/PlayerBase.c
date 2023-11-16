modded class PlayerBase
{
	bool m_Expansion_IsFiring = false;

	ref TIntArray m_eAI_FactionModifiers;

	override void Init()
	{
		super.Init();

		if (!IsAI())
			RegisterNetSyncVariableFloat("m_eAI_LastAggressionTimeout", 0.0, 16777215.0, 2);
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionRecruitAI, InputActionMap);
		AddAction(ExpansionActionDismissAI, InputActionMap);
	}

	/**
	 * @brief gets the sound a player is making from movement or firing a weapon.
	 */
	int Expansion_GetMovementSound(float noise = 0)
	{
		if (IsAlive())
		{
			if (m_Expansion_IsFiring)
			{
				noise = 5;
			}
			else
			{
				noise += NoiseAIEvaluate.GetNoiseMultiplier(this);
				noise = Math.Round(noise * 5);
				Math.Clamp(noise, 0, 5);
			}
		}

		return noise;
	}

	void Expansion_SetIsFiring(bool a) 
	{
		m_Expansion_IsFiring = a;
	}

	bool Expansion_GetIsFiring() 
	{
		return m_Expansion_IsFiring;
	}

	override void eAI_SetFactionTypeID(int id)
	{
		super.eAI_SetFactionTypeID(id);

		if (GetGame().IsServer())
		{
			eAI_RemoveFactionModifiers();
			if (GetGroup())
				eAI_ApplyFactionModifiers(GetGroup().GetFaction());
		}
	}

	/**
	 * @brief Apply faction modifiers (if any) e.g. brain disease, flies
	 */
	void eAI_ApplyFactionModifiers(eAIFaction faction)
	{
		auto trace = EXTrace.Start(EXTrace.AI,this);

		TIntArray modifiers = faction.GetModifiers();

		if (!modifiers || !modifiers.Count())
			return;

		EXTrace.Add(trace, modifiers.Count());

		m_eAI_FactionModifiers = {};

		foreach (int modifier: modifiers)
		{
			//! Don't track if already active
			ModifierBase modifierBase = m_ModifiersManager.GetModifier(modifier);
			if (modifierBase && modifierBase.IsActive())
				continue;

			EXTrace.Print(EXTrace.AI, this, typename.EnumToString(eModifiers, modifier));

			switch (modifier)
			{
				case eModifiers.MDF_BRAIN:
					InsertAgent(eAgents.BRAIN, PluginTransmissionAgents.GetAgentMaxCount(eAgents.BRAIN));
					break;
			}

			m_ModifiersManager.ActivateModifier(modifier);
			m_eAI_FactionModifiers.Insert(modifier);
		}
	}

	/**
	 * @brief Remove faction modifiers (if any) e.g. brain disease, flies
	 */
	void eAI_RemoveFactionModifiers()
	{
		auto trace = EXTrace.Start(EXTrace.AI,this);

		if (!m_eAI_FactionModifiers)
			return;

		EXTrace.Add(trace, m_eAI_FactionModifiers.Count());

		foreach (int modifier: m_eAI_FactionModifiers)
		{
			if (!m_ModifiersManager.GetModifier(modifier).IsActive())
				continue;

			switch (modifier)
			{
				case eModifiers.MDF_BRAIN:
					RemoveAgent(eAgents.BRAIN);
					break;
			}

			m_ModifiersManager.DeactivateModifier(modifier);
		}

		m_eAI_FactionModifiers = null;
	}
};
