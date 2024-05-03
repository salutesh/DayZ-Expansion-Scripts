modded class ExplosivesBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		//! For ExplosivesBase, ammo types are set in respective explosive constructor,
		//! but target info is initialized in ExplosivesBase constructor,
		//! so we need a delay to get correct ammo types
		eAIItemTargetInformation info = GetTargetInformation();
		if (info)
			info.DeferredInit();
	}

	override protected eAIItemTargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAIItemHazardTargetInformation(this);
	}
};
