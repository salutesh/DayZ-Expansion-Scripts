/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	ref ExpansionHumanST m_ExpansionST;

	protected bool m_Expansion_CanBeLooted = true;

	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<DayZPlayerImplement>(this);

	protected bool m_Expansion_IsInSafeZone;

	ref ExpansionNetsyncData m_Expansion_NetsyncData;

	void DayZPlayerImplement()
	{
		Expansion_Init();
	}

	void Expansion_Init()
	{
	}

	void OnEnterZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnEnterZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			m_Expansion_IsInSafeZone = true;
		}
	}

	void OnExitZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnExitZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			m_Expansion_IsInSafeZone = false;
		}
	}
	
	bool IsInSafeZone()
	{
		return m_Expansion_IsInSafeZone;
	}

	void Expansion_SetCanBeLooted(bool canBeLooted)
	{
		m_Expansion_CanBeLooted = canBeLooted;
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (!m_Expansion_CanBeLooted)
			//! 10134 = 2 | 4 | 16 | 128 | 256 | 512 | 1024 | 8192
			ExpansionStatic.LockInventoryRecursive(this, 10134);
	}

	override bool CanBeSkinned()
	{
		if (!m_Expansion_CanBeLooted)
			return false;

		return super.CanBeSkinned();
	}

	override bool NameOverride(out string output)
	{
		if (m_Expansion_NetsyncData && m_Expansion_NetsyncData.Get(0, output))
			return true;
		else
			return super.NameOverride(output);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (m_Expansion_NetsyncData)
			m_Expansion_NetsyncData.OnRPC(sender, rpc_type, ctx);
	}
};
