modded class BuildingBase
{
	ref eAIDynamicPatrolSpawner<BuildingBase> m_eAI_DynamicPatrolSpawner;

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
};
