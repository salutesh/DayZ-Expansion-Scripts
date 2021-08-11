/**
 * Expansion_MP5_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_MP5_Base : RifleBoltLock_Base
{
	void Expansion_MP5_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp5kRecoil(this);
	}
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.SpawnEntityOnGroundPos("Mag_MP5_30Rnd", entity.GetPosition());
		}
	}
};