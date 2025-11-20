modded class AnimalBase
{
	EntityAI m_Expansion_LastDamageSource;

	ExpansionNavmeshCarver m_Expansion_NavmeshCarver;

	protected int m_Expansion_NavmeshCarverNetIdLow;
	protected int m_Expansion_NavmeshCarverNetIdHigh;

	void AnimalBase()
	{
		if (ConfigGetBool("useExpansionNavMeshCarver"))
		{
			RegisterNetSyncVariableInt("m_Expansion_NavmeshCarverNetIdLow");
			RegisterNetSyncVariableInt("m_Expansion_NavmeshCarverNetIdHigh");
		}
	}

	void ~AnimalBase()
	{
		if (!g_Game)
			return;

		if (g_Game.IsServer() && ConfigGetBool("useExpansionNavMeshCarver"))
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_CheckCreateNavmeshCarver);
	}

	//! Workaround for vanilla passing killed entity as killer to EEKilled since 1.20
	//! Just check if killer is killed entity in EEKilled and then use m_Expansion_LastDamageSource instead of killer
	//! Vanilla events are fired in this order for each hit:
	//! EEOnDamageCalculated
	//! EEHitBy (IsAlive() still true)
	//!   EEKilled (within EEHitBy)
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool applyDamage = super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

#ifdef DIAG_DEVELOPER
		EXTrace.PrintHit(EXTrace.MISC, this, "EEOnDamageCalculated - apply damage? " + applyDamage, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
#endif

		if (applyDamage)
			m_Expansion_LastDamageSource = source;

		return applyDamage;
	}

#ifdef DIAG_DEVELOPER
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		EXTrace.PrintHit(EXTrace.MISC, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
#endif

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		//! Since some dead animals have collision, carve navmesh so AI don't get stuck
		if (g_Game.IsServer() && ConfigGetBool("useExpansionNavMeshCarver"))
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_CheckCreateNavmeshCarver, 100, true);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (m_Expansion_NavmeshCarver)
			g_Game.ObjectDelete(m_Expansion_NavmeshCarver);
	}

	void Expansion_CheckCreateNavmeshCarver()
	{
		//! Wait until ragdoll finished
		if (!dBodyIsActive(this))
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_CheckCreateNavmeshCarver);

			Expansion_CreateNavmeshCarver();
		}
	}

	void Expansion_CreateNavmeshCarver()
	{
		vector pos;
		vector dir;

		int idx1;
		int idx2;
		int idx3;

		idx1 = GetBoneIndexByName("head");
		idx2 = GetBoneIndexByName("pelvis");
		idx3 = GetBoneIndexByName("rbleg2");

		if (idx1 > -1 && idx2 > -1 && idx3 > -1)
		{
			vector pos1 = GetBonePositionWS(idx1);
			vector pos2 = GetBonePositionWS(idx2);
			vector pos3 = GetBonePositionWS(idx3);

			pos3[1] = pos2[1];

			dir = vector.Direction(pos1, pos3);

			EXTrace.Print(EXTrace.MISC, this, string.Format("head to back leg %1 m", dir.Length()));

			//! Animal_UrsusArctos = ~1.7..1.8 m
			//! Animal_BosTaurus = ~2.1 m
			if (dir.Length() < 1.7)  //! Not a large animal
				return;

			pos = pos1 + dir * 0.55;
			pos[1] = GetPosition()[1];
		}
		else
		{
			dir = GetDirection();
			pos = GetPosition();
		}

		EXTrace.Print(EXTrace.MISC, this, "Creating navmesh carver at " + pos);
		if (Class.CastTo(m_Expansion_NavmeshCarver, g_Game.CreateObjectEx("ExpansionLargeAnimalNavmeshCarver", pos, ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_KEEPHEIGHT)))
		{
			m_Expansion_NavmeshCarver.SetDirection(dir);
			m_Expansion_NavmeshCarver.Expansion_SetAssociatedObject(this);
			m_Expansion_NavmeshCarver.GetNetworkID(m_Expansion_NavmeshCarverNetIdLow, m_Expansion_NavmeshCarverNetIdHigh);
		}
	}

	override void SetAsSkinned()
	{
		if (m_Expansion_NavmeshCarver)
			m_Expansion_NavmeshCarver.SetPosition(vector.Zero);  //! So skinned items are not placed on top of navmesh carver

		super.SetAsSkinned();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (!m_Expansion_NavmeshCarver && (m_Expansion_NavmeshCarverNetIdLow != 0 || m_Expansion_NavmeshCarverNetIdHigh != 0))
		{
			if (Class.CastTo(m_Expansion_NavmeshCarver, g_Game.GetObjectByNetworkId(m_Expansion_NavmeshCarverNetIdLow, m_Expansion_NavmeshCarverNetIdHigh)))
			{
				EXTrace.Print(EXTrace.MISC, this, "OnVariablesSynchronized - navmesh carver " + m_Expansion_NavmeshCarver);
				m_Expansion_NavmeshCarver.Expansion_SetAssociatedObject(this);
			}
		}
	}
}
