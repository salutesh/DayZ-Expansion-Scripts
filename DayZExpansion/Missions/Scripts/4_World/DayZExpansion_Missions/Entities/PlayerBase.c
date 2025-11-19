modded class PlayerBase
{
	bool m_Expansion_ContaminatedArea;
	bool m_Expansion_ContaminatedAreaSynchRemote;
	
	int m_Expansion_ContaminatedAreaPPERequesterIdx;

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableBool("m_Expansion_ContaminatedAreaSynchRemote");
	}

	bool IsInContaminatedArea()
	{
		return m_Expansion_ContaminatedArea;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_Expansion_ContaminatedAreaSynchRemote && !m_Expansion_ContaminatedArea)
		{
			OnEnterZone(ExpansionZoneType.CONTAMINATED);
		}
		else if (!m_Expansion_ContaminatedAreaSynchRemote && m_Expansion_ContaminatedArea)
		{
			OnExitZone(ExpansionZoneType.CONTAMINATED);
		}
	}

	override void OnEnterZone(ExpansionZoneType type)
	{
		super.OnEnterZone(type);

		if (type != ExpansionZoneType.CONTAMINATED)
			return;

		m_Expansion_ContaminatedArea = true;
		m_Expansion_ContaminatedAreaSynchRemote = true;

		if (g_Game.IsServer())
		{
			GetModifiersManager().ActivateModifier(eModifiers.MDF_AREAEXPOSURE);
		}

		if (!g_Game.IsDedicatedServer())
		{
			auto area = ExpansionContaminatedArea.GetArea(GetPosition());
			if (!area)
			{
				Error("No nearby contaminated area");
				return;
			}
			
			m_Expansion_ContaminatedAreaPPERequesterIdx = area.m_PPERequesterIdx;
			
			SetContaminatedEffect(true, m_Expansion_ContaminatedAreaPPERequesterIdx, area.m_AroundParticleID, area.m_TinyParticleID);
		}
	}

	override void OnExitZone(ExpansionZoneType type)
	{
		super.OnExitZone(type);

		if (type != ExpansionZoneType.CONTAMINATED)
			return;

		m_Expansion_ContaminatedArea = false;
		m_Expansion_ContaminatedAreaSynchRemote = false;

		if (g_Game.IsServer())
		{
			GetModifiersManager().DeactivateModifier(eModifiers.MDF_AREAEXPOSURE);
		}

		if (!g_Game.IsDedicatedServer())
		{
			SetContaminatedEffect(false, m_Expansion_ContaminatedAreaPPERequesterIdx);
		}
	}
};
