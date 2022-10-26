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

	ref map<string, bool> m_Expansion_DisabledAmmoDamage = new map<string, bool>;

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

	bool Expansion_CanBeLooted()
	{
		return m_Expansion_CanBeLooted;
	}

	void Expansion_SetAllowDamage(string ammoType, bool state)
	{
		//! @note State is inverted so we can easily check m_Expansion_DisabledAmmoDamage[ammoType] to see whether it's disabled
		m_Expansion_DisabledAmmoDamage[ammoType] = !state;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (m_Expansion_DisabledAmmoDamage[ammo])
			return false;

		return true;
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

	vector Expansion_GetHeadingVector()
	{
		return Vector(-GetInputController().GetHeadingAngle() * Math.RAD2DEG, 0, 0).AnglesToVector();
	}

	float Expansion_GetMovementSpeed()
	{
		HumanCommandMove hcm = GetCommand_Move();
		if (hcm)
			return hcm.GetCurrentMovementSpeed();

		return 0.0;
	}

	float Expansion_GetMovementAngle()
	{
		HumanCommandMove hcm = GetCommand_Move();
		if (hcm)
			return hcm.GetCurrentMovementAngle();

		return 0.0;
	}
};

modded class DayZPlayerCommandDeathCallback
{
	override void OnSimulationEnd()
	{
		EntityAI itemInHands;
		if (GetGame().IsServer())
			itemInHands = m_pPlayer.GetHumanInventory().GetEntityInHands();

		super.OnSimulationEnd();

		if (itemInHands && !m_pPlayer.Expansion_CanBeLooted())
		{
			ExpansionItemBaseModule.SetLootable(itemInHands, false);
			itemInHands.SetLifetimeMax(120);  //! Make sure it despawns quickly when left alone
		}
	}
};
