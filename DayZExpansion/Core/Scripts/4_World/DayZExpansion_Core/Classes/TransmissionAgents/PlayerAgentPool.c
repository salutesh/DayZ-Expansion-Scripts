modded class PlayerAgentPool
{
	override void AddAgent(int agent_id, float count)
	{
		//! Positive count means something bad is inserted (illness etc), negative count means something bad is removed (cured)
		//! If dmg is disabled, allow removal, but not insertion
		if (count > 0 && !m_Player.Expansion_CanBeDamaged())
			return;

		super.AddAgent(agent_id, count);
	}

#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		//! Unlike vanilla, we write count so we'll always be able to successfully read back what was actually saved
		int count = m_PluginTransmissionAgents.GetAgentList().Count();
		ctx.Write(count);

		array<int> agentList = m_PluginTransmissionAgents.GetAgentList().GetKeyArray();
		foreach (int agentId: agentList)
		{
			ctx.Write(agentId);
			ctx.Write(GetSingleAgentCount(agentId));

		#ifndef DAYZ_1_25
			//! 1.26+
			ctx.Write(GetTemporaryResistance(agentId));
		#endif
		}
	}
	
	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; ++i)
		{
			int agentId;
			if (!ctx.Read(agentId))
				return false;

			int agentCount;
			if (!ctx.Read(agentCount))
				return false;

			if (version >= 140)
			{
				float agentTemporaryResistanceTime;
				if (!ctx.Read(agentTemporaryResistanceTime))
					return false;

			#ifndef DAYZ_1_25
				//! 1.26+
				SetTemporaryResistance(agentId, agentTemporaryResistanceTime);
			#endif
			}
			
			SetAgentCount(agentId, agentCount);
		}
	
		return true;
	}
#endif
}
