[eAIRegisterDynamicPatrolSpawner(BuildingBase)]
modded class BuildingBase
{
	static ref TStringArray s_eAI_PreventClimb;

	ref eAIDynamicPatrolSpawner<BuildingBase> m_eAI_DynamicPatrolSpawner;
	ref map<int, int> m_eAI_LastDoorInteractionTime = new map<int, int>;
	ref map<int, ref eAIDoorTargetInformation> m_eAI_DoorTargetInformation;
	bool m_eAI_PreventClimb;

	void BuildingBase()
	{
		if (!GetGame().IsServer())
			return;

		m_eAI_DynamicPatrolSpawner = new eAIDynamicPatrolSpawner<BuildingBase>(this);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (!GetGame().IsServer())
			return;

		//! Do not init patrols on CrashBase unless created by CE (EEOnCECreate/AfterStoreLoad will take care of init in that case)
		if (!IsInherited(CrashBase))
		{
			m_eAI_DynamicPatrolSpawner.Init();
		}

		if (!s_eAI_PreventClimb)
		{
			//! Hardcoded defaults (AI will vault those buildings' edges otherwise or climb on ledges unnecessarily)
			s_eAI_PreventClimb = {
				"Land_CementWorks",
				"Land_Garage",
				"Land_House",
				"Land_Misc",
				"Land_Mil",
				"Land_Office",
				"Land_Shed",
				"Land_Wreck"
			};

			s_eAI_PreventClimb.InsertAll(GetExpansionSettings().GetAI().PreventClimb);
		}

		string type = GetType();
		foreach (string preventClimb: s_eAI_PreventClimb)
		{
			if (type.IndexOf(preventClimb) == 0)
			{
				m_eAI_PreventClimb = true;
				break;
			}
		}
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_eAI_DynamicPatrolSpawner.Init();
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_eAI_DynamicPatrolSpawner.Init();
	}

#ifdef DIAG
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		EXTrace.PrintHit(EXTrace.AI, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
#endif

	eAIDoorTargetInformation eAI_GetDoorTargetInformation(int doorIndex, vector position)
	{
		if (!m_eAI_DoorTargetInformation)
			m_eAI_DoorTargetInformation = new map<int, ref eAIDoorTargetInformation>;

		eAIDoorTargetInformation info;
		if (!m_eAI_DoorTargetInformation.Find(doorIndex, info))
		{
			info = new eAIDoorTargetInformation(this, doorIndex, position);
			m_eAI_DoorTargetInformation[doorIndex] = info;
		}

		return info;
	}
};

//! Bit annoying to have to do this, it's just for error checking of AI patrol settings which uses IsKindOf
//! and BuildingBase is not an rVConfig type...
[eAIRegisterDynamicPatrolSpawner(House)]
modded class House
{
};
