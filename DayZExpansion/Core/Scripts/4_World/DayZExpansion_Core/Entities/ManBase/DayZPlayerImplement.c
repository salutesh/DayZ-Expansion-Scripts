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
	protected bool m_Expansion_IsInSafeZone_DeprecationWarning;

	ref ExpansionNetsyncData m_Expansion_NetsyncData;

	ref map<string, bool> m_Expansion_DisabledAmmoDamage = new map<string, bool>;

	vector m_Expansion_HeadBonePositionMS;
	float m_Expansion_HeadBoneIdleTime;
	vector m_Expansion_LHandBonePositionMS;
	float m_Expansion_LHandBoneIdleTime;
	vector m_Expansion_RHandBonePositionMS;
	float m_Expansion_RHandBoneIdleTime;
	vector m_Expansion_LFootBonePositionMS;
	float m_Expansion_LFootBoneIdleTime;
	vector m_Expansion_RFootBonePositionMS;
	float m_Expansion_RFootBoneIdleTime;

	bool m_Expansion_EnableBonePositionUpdate;

	int m_Expansion_CurrentPhxInteractionLayer;

	void DayZPlayerImplement()
	{
		Expansion_Init();
	}

	//! For NPCs that don't inherit from PlayerBase
	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;

		return m_Expansion_CanBeLooted;
	}

	void Expansion_Init()
	{
	}

	bool Expansion_HasAdminToolGodMode()
	{
		return false;
	}

	bool Expansion_HasAdminToolInvisibility()
	{
		return false;
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
		Expansion_Error("DEPRECATED: Please use Expansion_IsInSafeZone", m_Expansion_IsInSafeZone_DeprecationWarning);
		return Expansion_IsInSafeZone();
	}
	
	bool Expansion_IsInSafeZone()
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

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
#ifdef DIAG
		int interactionLayer = dBodyGetInteractionLayer(this);
#endif

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);

		if (m_Expansion_EnableBonePositionUpdate)
			Expansion_UpdateBonePositionTimes(pDt);

#ifdef DIAG
		if (interactionLayer != m_Expansion_CurrentPhxInteractionLayer)
		{
			m_Expansion_CurrentPhxInteractionLayer = interactionLayer;
			EXTrace.Print(EXTrace.PLAYER, this, "::CommandHandler current interaction layer " + ExpansionStatic.BitmaskEnumToString(PhxInteractionLayers, interactionLayer));
		}
#endif
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

	void Expansion_UpdateBonePositionTimes(float pDt)
	{
		vector headPosition = GetBonePositionMS(GetBoneIndexByName("Head"));
		if (vector.DistanceSq(headPosition, m_Expansion_HeadBonePositionMS) < 0.0004)
		{
			m_Expansion_HeadBoneIdleTime += pDt;
		}
		else
		{
			m_Expansion_HeadBonePositionMS = headPosition;
			m_Expansion_HeadBoneIdleTime = 0;
		}
		vector lHandPosition = GetBonePositionMS(GetBoneIndexByName("LeftHand"));
		if (vector.DistanceSq(lHandPosition, m_Expansion_LHandBonePositionMS) < 0.01)
		{
			m_Expansion_LHandBoneIdleTime += pDt;
		}
		else
		{
			m_Expansion_LHandBonePositionMS = lHandPosition;
			m_Expansion_LHandBoneIdleTime = 0;
		}
		vector rHandPosition = GetBonePositionMS(GetBoneIndexByName("RightHand"));
		if (vector.DistanceSq(rHandPosition, m_Expansion_RHandBonePositionMS) < 0.01)
		{
			m_Expansion_RHandBoneIdleTime += pDt;
		}
		else
		{
			m_Expansion_RHandBonePositionMS = rHandPosition;
			m_Expansion_RHandBoneIdleTime = 0;
		}
		vector lFootPosition = GetBonePositionMS(GetBoneIndexByName("LeftFoot"));
		if (vector.DistanceSq(lFootPosition, m_Expansion_LFootBonePositionMS) < 0.0004)
		{
			m_Expansion_LFootBoneIdleTime += pDt;
		}
		else
		{
			m_Expansion_LFootBonePositionMS = lFootPosition;
			m_Expansion_LFootBoneIdleTime = 0;
		}
		vector rFootPosition = GetBonePositionMS(GetBoneIndexByName("RightFoot"));
		if (vector.DistanceSq(rFootPosition, m_Expansion_RFootBonePositionMS) < 0.0004)
		{
			m_Expansion_RFootBoneIdleTime += pDt;
		}
		else
		{
			m_Expansion_RFootBonePositionMS = rFootPosition;
			m_Expansion_RFootBoneIdleTime = 0;
		}
	}

	bool Expansion_IsAnimationIdle()
	{
		if (m_Expansion_HeadBoneIdleTime < 0.25)
			return false;
		if (m_Expansion_LHandBoneIdleTime < 0.25)
			return false;
		if (m_Expansion_RHandBoneIdleTime < 0.25)
			return false;
		if (m_Expansion_LFootBoneIdleTime < 0.25)
			return false;
		if (m_Expansion_RFootBoneIdleTime < 0.25)
			return false;
		return true;
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
