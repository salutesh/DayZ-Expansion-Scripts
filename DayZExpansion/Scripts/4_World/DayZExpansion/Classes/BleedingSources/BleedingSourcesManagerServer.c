/**
 * BleedingSourcesManagerServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BleedingSourcesManagerServer
{
	//! Damage must be to "blood" healthType
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		float dmg_max = m_Player.GetMaxHealth(zone, "Blood");
		float dmg = damage;
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		
		//! Debugging
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dmg_max = " + dmg_max);
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dmg = " + dmg);
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("bleed_threshold = " + bleed_threshold);
		#endif

		if ( source.IsZombie() )
		{
			int chance = Math.RandomInt(0,10); //! 10% Chance to get a bleeding source from a infected hit
			if (chance == 1)
			{
				AttemptAddBleedingSource(component);

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("BLEEDING ZOMBIE");
				#endif
			}
		}
		else if ( dmg > (dmg_max * (1 - bleed_threshold)) )
		{
			AttemptAddBleedingSource(component);

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("BLEEDING");
			#endif
		}
	}
};