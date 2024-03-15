modded class ExplosivesBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		eAIItemTargetInformation info = GetTargetInformation();
		if (info)
			info.DeferredInit();
	}

	override protected eAIItemTargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAIExplosivesTargetInformation(this);
	}

	TStringArray Expansion_GetAmmoTypes()
	{
		return m_AmmoTypes;
	}
};
