modded class WeaponStateBase
{
	void eAI_Vanilla_OnEntry(WeaponEventBase e)
	{
		if (HasFSM() && !m_fsm.IsRunning())
		{
			if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString() + "  Has Sub-FSM! Starting submachine..."); }
			m_fsm.Start(e);
		}
		else
			if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString()); }
	}

	void eAI_WpnPrint(string s)
	{
	#ifdef WPN_DEBUG
		PrintToRPT(s);
	#else
		eAIBase ai;
		if (Class.CastTo(ai, m_weapon.GetHierarchyParent()))
			EXTrace.Print(EXTrace.AI, this, ai.ToString() + " " + s);
	#endif
	}

	void wpnDebugPrint(string s)
	{
		eAI_WpnPrint(s);
	}

	void wpnPrint(string s)
	{
		eAI_WpnPrint(s);
	}
}
