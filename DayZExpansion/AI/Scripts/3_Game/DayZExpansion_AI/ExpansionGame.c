modded class ExpansionGame
{
	protected ref eAICommandManager m_CommandManager;

	private bool m_InGroup;

	eAICommandManager GetCommandManager()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetCommandManager");
#endif

		return m_CommandManager;
	}

	/**
	 * @note Client only
	 */
	bool InGroup()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "InGroup");
#endif

		return m_InGroup;
	}

	/**
	 * @note Client only
	 */
	void SetInGroup(bool group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetInGroup");
#endif

		m_InGroup = group;
	}

#ifndef SERVER
	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		switch (rpc_type)
		{
			case ExpansionRPC.FirearmEffects:
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					break;
				int componentIndex;
				vector pos;
				vector surfNormal;
				vector inSpeed;
				string ammoType;
				if (!ctx.Read(componentIndex))
					break;
				if (!ctx.Read(pos))
					break;
				if (!ctx.Read(surfNormal))
					break;
				if (!ctx.Read(inSpeed))
					break;
				if (!ctx.Read(ammoType))
					break;
				Object directHit = target;
				string surface = ExpansionStatic.GetImpactSurfaceType(directHit, pos, inSpeed);
				vector exitPos;
				vector outSpeed;
				bool isWater = surface == "Hit_Water";
				bool deflected = Math.RandomIntInclusive(0, 1);
				//! Call FirearmEffects the same way vanilla would (source = NULL on client)
				GetDayZGame().FirearmEffects(null, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
				EffectSound sound = SEffectManager.Expansion_PlaySound("bulletFlyBy_SoundSet", pos);
				//! FIXME: No impact sound :-(
				return true;
		}

		return super.OnRPC(sender, target, rpc_type, ctx);
	}
#endif
};
