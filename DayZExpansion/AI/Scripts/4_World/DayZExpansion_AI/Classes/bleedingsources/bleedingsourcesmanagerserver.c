modded class BleedingSourcesManagerServer
{
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		if (!m_Player.IsAI() || !source || !source.IsDayZCreature())
		{
			//! Not AI or not a creature hit
			super.ProcessHit(damage, source, component, zone, ammo, modelPos);
		}
		else
		{
			//! AI and creature hit
			eAI_ProcessHit(damage, source, component, zone, ammo, modelPos);
		}
	}

	//! Like vanilla, except 10% of vanilla bleed chance
	void eAI_ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		float dmg_max = m_Player.GetMaxHealth(zone, "Blood");
		float bleed_threshold = GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied bleedThreshold");
		string damageTypeString = GetGame().ConfigGetTextOut("CfgAmmo " + ammo + " DamageApplied type");
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		float bleedingChance;
		bool createBleedingSource = false;
		
		// 'true' only when the damageTypeString is handled there
		if (BleedChanceData.CalculateBleedChance(damageTypeString, damage, bleed_threshold,bleedingChance))
		{
			bleedingChance *= 0.1;  //! 10% of vanilla bleed chance

			float roll = Math.RandomFloat01();
			createBleedingSource = bleedingChance != 0 && bleedingChance >= roll;
			
			#ifdef ENABLE_LOGGING
			if (LogManager.IsBleedingChancesLogEnable())
			{	
				Debug.BleedingChancesLog(roll.ToString(), "BleedingSourcesManagerServer" , "n/a", "bleeding random roll:");
			}
			#endif
		}
		else if (source && source.IsZombie())
		{
			int chance = Math.RandomInt(0,1000);  //! 10% of vanilla bleed chance
			if (chance <= damage)
			{
				createBleedingSource = true;
			}
		}
		else if (damage > (dmg_max * (1 - bleed_threshold * 0.1)) )  //! 10% of vanilla bleed chance
		{
			createBleedingSource = true;
		}
		
		if (createBleedingSource)
		{
			#ifdef ENABLE_LOGGING
			if (LogManager.IsBleedingChancesLogEnable())
			{	
				Debug.BleedingChancesLog("true", "BleedingSourcesManagerServer" , "n/a", "Attempting to create bleeding source");
			}
			#endif
			AttemptAddBleedingSource(component);
		}
	}
}
