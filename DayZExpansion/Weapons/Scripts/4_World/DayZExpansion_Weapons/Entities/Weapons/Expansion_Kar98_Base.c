/**
 * Expansion_Kar98_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Kar98_Base: BoltActionRifle_InnerMagazine_Base
{	
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		if (optic && PUScopeOptic.Cast(optic))
			return true;
		return super.CanEnterIronsights();
	}
	override RecoilBase SpawnRecoilObject()
	{
		return new MosinRecoil(this);
	}	
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.GetInventory().CreateInInventory( "ExpansionKar98ScopeOptic" );
			entity.GetInventory().CreateInInventory( "Expansion_Kar98_Bayonet" );
		}
	}
};