class eAIState_TakeItem_Base: eAIState
{
	ItemBase m_Item;
	float m_Time;
	bool m_LoweringWeapon;
	int m_Stance;

	override void OnEntry(string Event, ExpansionState From)
	{
	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, unit, m_Name + " " + m_Item.ToString());
	#endif

		m_Time = 0;
		m_LoweringWeapon = false;

		if (unit.GetEmoteManager().IsEmotePlaying())
			unit.GetEmoteManager().ServerRequestEmoteCancel();

		ChangeStance();
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		eAIState_TakeItem_Base state;
		if (Class.CastTo(state, To))
			state.m_Stance = m_Stance;
		else if (unit.eAI_GetStance() != m_Stance)
			unit.OverrideStance(m_Stance);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.GetEmoteManager().IsEmotePlaying() || unit.GetActionManager().GetRunningAction() || unit.GetWeaponManager().IsRunning() || !unit.GetCommand_Move())
		{
			m_Time += DeltaTime;
			if (m_Time > 10)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Item + ") - timeout");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		
		//! Taking items while raised breaks hands! Wait until lowered
		if (unit.IsRaised())
		{
			m_LoweringWeapon = true;
			unit.RaiseWeapon(false);
			m_Time = 0;
		}
		
		if (m_LoweringWeapon)
		{
			if (m_Time < 0.5)
			{
				m_Time += DeltaTime;
				return CONTINUE;
			}
			
			m_Time = 0;
			m_LoweringWeapon = false;
		}
		
		if (unit.eAI_IsChangingStance())
		{
			m_Time += DeltaTime;
			if (m_Time > 10)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Item + ")" + " - timeout during changing stance");
				m_Time = 0;
				return EXIT;
			}

			return CONTINUE;
		}
		
		if (OnUpdateEx(DeltaTime, SimulationPrecision) == EXIT)
			return EXIT;
		
		if (m_Time < 0.5)
		{
			m_Time += DeltaTime;
			return CONTINUE;
		}

		if (unit.GetActionManager().GetRunningAction())
			return CONTINUE;

		if (!m_Item || m_Item.GetHierarchyRootPlayer() == unit)
			m_Time = 0;  //! Taken
		
		return EXIT;
	}

	int OnUpdateEx(float DeltaTime, int SimulationPrecision)
	{
		return EXIT;
	}

	void ChangeStance()
	{
		Man owner = m_Item.GetHierarchyRootPlayer();

		if (owner == unit)
			return;

		if (m_Item.GetPosition()[1] > unit.GetPosition()[1] + 0.5)
			return;

		if (unit.m_Environment.Expansion_IsUnderRoofBuilding() && !owner)
			return;

		int curStance = unit.eAI_GetStance();

		int tgtStance;

		if (owner || curStance == DayZPlayerConstants.STANCEIDX_PRONE)
			tgtStance = DayZPlayerConstants.STANCEIDX_PRONE;
		else
			tgtStance = DayZPlayerConstants.STANCEIDX_CROUCH;

		if (curStance == tgtStance)
			return;

		m_Stance = curStance;

		unit.OverrideStance(tgtStance);
		unit.m_eAI_CommandMove.SetStance(tgtStance);  //! Forces eAI_IsChangingStance to return true instantly
	}
}
