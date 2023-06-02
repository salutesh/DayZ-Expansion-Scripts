[eAIRegisterDynamicPatrolSpawner(BuildingBase)]
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

//! Bit annoying to have to do this, it's just for error checking of AI patrol settings which uses IsKindOf
//! and BuildingBase is not an rVConfig type...
[eAIRegisterDynamicPatrolSpawner(House)]
modded class House
{
};
