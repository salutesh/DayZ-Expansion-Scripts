/**
 * Expansion_M16_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_M16_Base : RifleBoltLock_Base
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemAttached - Start");
		#endif		

		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsDown(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_16_Base::EEItemAttached - End");
		#endif		
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - Start");
		#endif

		super.EEItemDetached(item,slot_name);
		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsUp(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - End");
		#endif
	}	

	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::SpawnRecoilObject");
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
			entity.SpawnEntityOnGroundPos("Mag_STANAGCoupled_30Rnd", entity.GetPosition());
		}
	}
}