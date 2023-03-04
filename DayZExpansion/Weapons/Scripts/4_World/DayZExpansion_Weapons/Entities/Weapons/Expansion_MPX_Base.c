/**
 * Expansion_MPX_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_MPX_Base: RifleBoltLock_Base
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "EEItemAttached");
#endif

		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{
			Expansion_FoldOpticsDown();
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "EEItemDetached");
#endif

		super.EEItemDetached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			Expansion_FoldOpticsUp();
		}
	}	

	override RecoilBase SpawnRecoilObject()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "SpawnRecoilObject");
#endif

		return new M4a1Recoil(this);
	}
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.GetInventory().CreateInInventory( "UniversalLight" );
			entity.GetInventory().CreateInInventory( "ExpansionEXPS3HoloOptic" );
			entity.GetInventory().CreateInInventory( "Battery9V" );
			entity.GetInventory().CreateInInventory( "Battery9V" );
			entity.SpawnEntityOnGroundPos("Mag_Expansion_MPX_50Rnd", entity.GetPosition());
		}
	}
};

class Expansion_MPX: Expansion_MPX_Base {};