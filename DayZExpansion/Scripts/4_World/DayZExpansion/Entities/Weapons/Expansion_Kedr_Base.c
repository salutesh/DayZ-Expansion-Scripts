/**
 * Expansion_Kedr_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Kedr_Base : RifleBoltLock_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_Kedr_Base::SpawnRecoilObject");
		#endif

		return new Cz61Recoil(this);
	}
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.SpawnEntityOnGroundPos("Mag_Expansion_Kedr_20Rnd", entity.GetPosition());
		}
	}
}