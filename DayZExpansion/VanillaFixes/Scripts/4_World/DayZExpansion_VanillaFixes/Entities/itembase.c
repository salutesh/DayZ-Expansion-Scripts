modded class ItemBase
{
	float m_Expansion_CECreationTimestamp;

#ifdef EXPANSION_DEBUG_LIFETIME
	bool m_Expansion_PrintCELifetime;
#endif

	override int GetLiquidType()
	{
		if (m_VarLiquidType != LIQUID_NONE && !Liquid.m_LiquidInfosByType[m_VarLiquidType])
			EXError.ErrorOnce(this, "Item " + GetType() + " is misconfigured - liquid type " + m_VarLiquidType + " is not in CfgLiquidDefinitions!", {});

		return super.GetLiquidType();
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		//! @note lifetime for newly created items will always be zero in EEOnCECreate
		//! and set to a higher value by the game afterwards as a means of staggered de- and respawning
		//! (see https://feedback.bistudio.com/T194225#2873138 for likely explanation)
		//! To truly override the initial lifetime, we need to wait for the 1st OnCEUpdate after creation
		//! (lifetime will still tick down slightly too slowly, but we're not attempting to fix that
		//! since it could interfere with lifetime being adjusted by other means)
		if (GetExpansionSettings().GetCore().ForceExactCEItemLifetime)
			m_Expansion_CECreationTimestamp = g_Game.GetTickTime();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		if (m_Expansion_CECreationTimestamp > 0)
		{
			float elapsed = g_Game.GetTickTime() - m_Expansion_CECreationTimestamp;

			m_Expansion_CECreationTimestamp = 0;

			float lifetimeMax = GetLifetimeMax();

			if (GetLifetime() > lifetimeMax)
				SetLifetime(lifetimeMax - elapsed);

		#ifdef EXPANSION_DEBUG_LIFETIME
			m_Expansion_PrintCELifetime = true;
		#endif
		}

	#ifdef EXPANSION_DEBUG_LIFETIME
		if (m_Expansion_PrintCELifetime)
			EXTrace.Print(EXTrace.CE, this, string.Format("OnCEUpdate lifetime %1", GetLifetime()));
	#endif
	}
}
