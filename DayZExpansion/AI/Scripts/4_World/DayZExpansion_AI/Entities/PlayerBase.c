modded class PlayerBase
{
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

	//! @note Only called for players not eAIBase since the latter overrides the commandhandler completely,
	//! formation update if AI is leader is dealt with in eAICommandMove
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE && !pCurrentCommandFinished)
		{
			eAIGroup group = GetGroup();
			if (group && group.GetFormationLeader() == this && group.Count() > 1)
				group.GetFormation().Update(pDt);
		}

		return super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished);
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
