[eAIRegisterDynamicPatrolSpawner(BuildingBase)]
modded class BuildingBase
{
	static autoptr TStringArray s_eAI_PreventClimb = {"Land_House", "Land_Mil_Airfield_HQ"};

	ref eAIDynamicPatrolSpawner<BuildingBase> m_eAI_DynamicPatrolSpawner;
	ref map<int, int> m_eAI_LastDoorInteractionTime = new map<int, int>;
	bool m_eAI_PreventClimb;

	void BuildingBase()
	{
		if (!GetGame().IsServer())
			return;

		m_eAI_DynamicPatrolSpawner = new eAIDynamicPatrolSpawner<BuildingBase>(this);

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
};

//! Bit annoying to have to do this, it's just for error checking of AI patrol settings which uses IsKindOf
//! and BuildingBase is not an rVConfig type...
[eAIRegisterDynamicPatrolSpawner(House)]
modded class House
{
};
