/**
 * Expansion_Vityaz_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Vityaz_Base: RifleBoltFree_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new AkmRecoil(this);
	}
	
	override int GetWeaponSpecificCommand(int weaponAction ,int subCommand)
	{
		if ( weaponAction == WeaponActions.RELOAD)
		{
			switch (subCommand)
			{
				case WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_BULLET:
					return WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_BULLET;
				
				case WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_BULLET:
					return WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_BULLET;
				
				case WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_NOBULLET:
					return WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET;
				
				case WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_NOBULLET:
					return WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET;
				
				default:
					return subCommand;
			}
		
		}
		return subCommand;
	}
	
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		
		if (optic && PSO1Optic.Cast(optic) || PSO11Optic.Cast(optic))
			return true;

		return super.CanEnterIronsights();
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{	
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		string slot_name = InventorySlots.GetSlotName(slot_id);   
		
		if ( slot_name == "weaponOpticsAK" )
		{
			return this.FindAttachmentBySlotName("weaponOpticsAK") != NULL;
		}
		if ( slot_name == "weaponOptics" )
		{
			return this.FindAttachmentBySlotName("weaponOpticsAK") == NULL;	
		}		
		return true;
	}	
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.GetInventory().CreateInInventory( "AK_RailHndgrd" );
			entity.GetInventory().CreateInInventory( "AK_PlasticBttstck" );
			entity.GetInventory().CreateInInventory( "UniversalLight" );
			entity.GetInventory().CreateInInventory( "ExpansionEXPS3HoloOptic" );
			entity.GetInventory().CreateInInventory( "Battery9V" );
			entity.GetInventory().CreateInInventory( "Battery9V" );
			entity.SpawnEntityOnGroundPos("Mag_Expansion_Vityaz_30Rnd", entity.GetPosition());
		}
	}
};

class Expansion_Vityaz : Expansion_Vityaz_Base {};