modded class DayZPlayerImplement
{
	static int DEBUG_EXPANSION_CLIMB = 0;
	
	private autoptr eAITargetInformation m_TargetInformation;

	private eAIGroup m_eAI_Group;
	protected typename m_eAI_FactionType;
	private int m_eAI_GroupID;
	private int m_eAI_GroupMemberIndex;
	private int m_eAI_GroupMemberIndexSynch;

	private bool m_eAI_IsPassive;

#ifndef SERVER
	autoptr array<Shape> m_Expansion_DebugShapes = new array<Shape>();
#endif

	void DayZPlayerImplement()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DayZPlayerImplement");
#endif

		m_TargetInformation = CreateTargetInformation();

		RegisterNetSyncVariableInt("m_eAI_GroupID");
		RegisterNetSyncVariableInt("m_eAI_GroupMemberIndexSynch");

		m_eAI_GroupID = -1;
		m_eAI_GroupMemberIndex = 0;
		m_eAI_GroupMemberIndex = m_eAI_GroupMemberIndexSynch;
	}

	override void Expansion_Init()
	{
		super.Expansion_Init();

		if (GetGame().IsServer() && m_eAI_FactionType)
		{
			//! @note w/o the cast to eAIFaction, the compiler warns about unsafe downcasting.
			//! Of course the compiler is wrong, because we're casting up, not down, so this cast here is just there to satisfy compiler shortcomings.
			//! Yes I wrote this comment for the sole reason that I'm annoyed by this.
			SetGroup(eAIGroup.CreateGroup(eAIFaction.Cast(m_eAI_FactionType.Spawn())));
		}
	}

	protected eAITargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAIPlayerTargetInformation(this);
	}

	eAITargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	bool IsAI()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "IsAI");
#endif

		return false;
	}

	void SetGroup(eAIGroup group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetGroup").Add(group);
#endif

		if (m_eAI_Group == group)
			return;

		if (m_eAI_Group)
		{
			m_eAI_Group.RemoveMember(this);

			m_eAI_GroupID = -1;
		}

		m_eAI_Group = group;

		if (m_eAI_Group)
		{
			SetGroupMemberIndex(m_eAI_Group.AddMember(this));

			m_eAI_GroupID = m_eAI_Group.GetID();
		}

		SetSynchDirty();
	}

	eAIGroup GetGroup()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetGroup");
#endif

		return m_eAI_Group;
	}

	int GetGroupID()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetGroupID");
#endif

		return m_eAI_GroupID;
	}

	void SetGroupMemberIndex(int index)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetGroupMemberIndex").Add(index);
#endif

		m_eAI_GroupMemberIndex = index;
		m_eAI_GroupMemberIndexSynch = index;

		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnVariablesSynchronized");
#endif

		super.OnVariablesSynchronized();

		if (GetGame().IsServer())
			return;

		if ((m_eAI_Group && m_eAI_Group.GetID() != m_eAI_GroupID))
		{
			// moved group

			m_eAI_Group.RemoveMember(this);

			m_eAI_Group = eAIGroup.GetGroupByID(m_eAI_GroupID, true);

			m_eAI_Group.Client_SetMemberIndex(this, m_eAI_GroupMemberIndexSynch);
		}
		else if (m_eAI_GroupID != -1 && !m_eAI_Group)
		{
			// joined group

			m_eAI_Group = eAIGroup.GetGroupByID(m_eAI_GroupID, true);

			m_eAI_Group.Client_SetMemberIndex(this, m_eAI_GroupMemberIndexSynch);
		}
		else if (m_eAI_GroupID == -1 && m_eAI_Group)
		{
			// left group

			m_eAI_Group.RemoveMember(this);

			m_eAI_Group = null;
		}
		else if (m_eAI_Group && m_eAI_GroupMemberIndexSynch != m_eAI_GroupMemberIndex)
		{
			// moved within group

			// @note: this has to be the last check as when moving/joining groups
			// the index is out of synch and will be handled in the above checks

			m_eAI_Group.Client_SetMemberIndex(this, m_eAI_GroupMemberIndexSynch);
		}
	}

	void eAI_SetPassive(bool state = true)
	{
		m_eAI_IsPassive = state;
	}

	bool eAI_IsPassive()
	{
		if (m_eAI_IsPassive)
			return true;
		if (m_eAI_Group)
			return m_eAI_Group.GetFaction().IsInherited(eAIFactionPassive);
		return false;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		//! Ignore hit from bullet we create in WeaponBase::EEFired for bullet impact effect to prevent NULL pointers
		//! due to source not being set when trying to determine damage animation
		if (damageType == DT_FIRE_ARM && !source)
			return false;

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEKilled(Object killer)
	{
		m_TargetInformation.OnDeath();

		super.EEKilled(killer);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		m_TargetInformation.OnHit();

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

#ifndef SERVER
	void AddShape(Shape shape)
	{
		m_Expansion_DebugShapes.Insert(shape);
	}
#endif

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
#ifndef SERVER
		for (int i = m_Expansion_DebugShapes.Count() - 1; i >= 0; i--)
			m_Expansion_DebugShapes[i].Destroy();
		m_Expansion_DebugShapes.Clear();
#endif

		if (DEBUG_EXPANSION_CLIMB != 0)
		{
			PlayerBase playerPB = PlayerBase.Cast(this);
	
			SHumanCommandClimbResult result();
			
			if (DEBUG_EXPANSION_CLIMB & 0x01 != 0)
			{
				HumanCommandClimb.DoClimbTest(playerPB, result, 0);
				ExpansionClimb.DebugClimb(playerPB, result, 0xAAFFFF00, 0xAA00FFFF);
			}
			
			if (DEBUG_EXPANSION_CLIMB & 0x10 != 0)
			{
				ExpansionClimb.DoClimbTest(playerPB, result);
				ExpansionClimb.DebugClimb(playerPB, result, 0xAAFF0000, 0xAA0000FF);
			}
		}

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}
};
