/**
 * ExpansionRespawnHandlerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRespawnHandlerModule: JMModuleBase
{
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule IsServer
	// ------------------------------------------------------------	
	override bool IsServer()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule IsClient
	// ------------------------------------------------------------
	override bool IsClient()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetExpansionStartingGear
	// ------------------------------------------------------------
	void SetExpansionStartingGear(PlayerBase player)
	{
		Print("ExpansionRespawnHandlerModule::SetExpansionStartingGear - Start");

		int i;
		EntityAI gear_item;
		EntityAI itemTop;
		EntityAI itemPants;
		EntityAI itemBag;
		EntityAI itemVest;
		
		if ( GetExpansionSettings().GetSpawn() )
		{
			ExpansionStartingGear gear;
			if ( Class.CastTo(gear, GetExpansionSettings().GetSpawn().StartingGear) )
			{
				//! Add items to top/shirt/jacked
				if ( gear.UseUpperGear )
				{
					itemTop = player.FindAttachmentBySlotName("Body");					
					if ( itemTop )
					{
						for ( i = 0; i < gear.UpperGear.Count(); i++ )
						{
							itemTop.GetInventory().CreateInInventory( gear.UpperGear[i] );
						}
					}
				}
				
				//! Add items to pants
				if ( gear.UsePantsGear )
				{
					itemPants = player.FindAttachmentBySlotName("Legs");
					if ( itemPants )
					{					
						for ( i = 0; i < gear.PantsGear.Count(); i++ )
						{
							itemPants.GetInventory().CreateInInventory( gear.PantsGear[i] );
						}
					}
				}
				
				//! Add items to backpack
				if ( gear.UseBackpackGear )
				{
					itemBag = player.FindAttachmentBySlotName("Back");
					if ( itemBag )
					{	
						for ( i = 0; i < gear.BackpackGear.Count(); i++ )
						{
							gear_item = itemBag.GetInventory().CreateInInventory( gear.BackpackGear[i] );
						}
					}
				}
				
				//! Add items to vest
				if ( gear.UseVestGear )
				{
					itemVest = player.FindAttachmentBySlotName("Vest");
					if ( itemVest )
					{					
						for ( i = 0; i < gear.VestGear.Count(); i++ )
						{
							gear_item = itemVest.GetInventory().CreateInInventory( gear.VestGear[i] );
						}
					}
				}
				
				//! Add primary weapon and its attachments
				if ( gear.UsePrimaryWeapon )
				{
					EntityAI item_primaryWeapon = player.GetInventory().CreateAttachmentEx( gear.PrimaryWeapon, InventorySlots.SHOULDER );			
					if ( item_primaryWeapon && item_primaryWeapon.IsWeapon() )
					{
						for ( i = 0; i < gear.PrimaryWeaponAttachments.Count(); i++ )
						{
							gear_item = item_primaryWeapon.GetInventory().CreateAttachment( gear.PrimaryWeaponAttachments[i] );
							if (!gear_item)
							{
								Error2( "ExpansionRespawnHandlerModule::SetExpansionStartingGear", "Could not create attachment item " + gear.PrimaryWeaponAttachments[i] + " for weapon " + gear.PrimaryWeapon + ". Please make sure you spawn the attachments in the correct order and the item is a valid attachment for the weapon " + gear.PrimaryWeapon + "! For further information please visit the wiki page for the spawn settings: \nhttps:\/\/github.com/salutesh/DayZ-Expansion-Scripts/wiki/%5BServer-Hosting%5D-SpawnSettings\n");
								continue;
							}
						}
					}
				}
				
				//! Add secondary weapon and its attachments
				if ( gear.UseSecondaryWeapon )
				{
					EntityAI item_secondaryWeapon = player.GetInventory().CreateAttachmentEx( gear.SecondaryWeapon, InventorySlots.MELEE );
					if ( item_secondaryWeapon && item_secondaryWeapon.IsWeapon() )
					{
						for ( i = 0; i < gear.SecondaryWeaponAttachments.Count(); i++ )
						{
							gear_item = item_secondaryWeapon.GetInventory().CreateAttachment( gear.SecondaryWeaponAttachments[i] );
							if (!gear_item)
							{
								Error2( "ExpansionRespawnHandlerModule::SetExpansionStartingGear", "Could not create attachment item " + gear.SecondaryWeaponAttachments[i] + " for weapon " + gear.SecondaryWeapon + ". Please make sure you spawn the attachments in the correct order and the item is a valid attachment for the weapon " + gear.SecondaryWeapon + "! For further information please visit the wiki page for the spawn settings: \nhttps:\/\/github.com/salutesh/DayZ-Expansion-Scripts/wiki/%5BServer-Hosting%5D-SpawnSettings\n");
								continue;
							}
						}
					}
				}
				
				array< EntityAI > items = new array< EntityAI >;
				player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
				for ( i = 0; i < items.Count(); i++ )
				{
					if ( gear.ApplyEnergySources && items[i].HasEnergyManager() )
						items[i].GetInventory().CreateAttachment( "Battery9V" );
					
					if ( gear.SetRandomHealth )
						SetRandomHealth( items[i] );
				}
			}
		}
		
		Print("ExpansionRespawnHandlerModule::SetExpansionStartingGear - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule ExpansionEquipCharacter
	// ------------------------------------------------------------
	void ExpansionEquipCharacter(PlayerBase player)
	{
		Print("DayZExpansion::ExpansionEquipCharacter - Start");
		
		ref array<EntityAI> clothingArray = new array<EntityAI>;
		
		ExpansionStartingClothing startingClothing;
		if (Class.CastTo(startingClothing, GetExpansionSettings().GetSpawn().StartingClothing))
		{
			EntityAI item_head = player.GetInventory().CreateAttachmentEx(startingClothing.Headgear.GetRandomElement(), InventorySlots.HEADGEAR);
			clothingArray.Insert(item_head);
			
			EntityAI item_eyewear = player.GetInventory().CreateAttachmentEx(startingClothing.Glasses.GetRandomElement(), InventorySlots.EYEWEAR);
			clothingArray.Insert(item_eyewear);
			
			EntityAI item_mask = player.GetInventory().CreateAttachmentEx(startingClothing.Masks.GetRandomElement(), InventorySlots.MASK);
			clothingArray.Insert(item_mask);
			
			EntityAI item_body = player.GetInventory().CreateAttachmentEx(startingClothing.Tops.GetRandomElement(), InventorySlots.BODY);
			clothingArray.Insert(item_body);
			
			EntityAI item_vest = player.GetInventory().CreateAttachmentEx(startingClothing.Vests.GetRandomElement(), InventorySlots.VEST);
			clothingArray.Insert(item_vest);
			
			EntityAI item_gloves = player.GetInventory().CreateAttachmentEx(startingClothing.Gloves.GetRandomElement(), InventorySlots.GLOVES);
			clothingArray.Insert(item_gloves);
			
			EntityAI item_legs = player.GetInventory().CreateAttachmentEx(startingClothing.Pants.GetRandomElement(), InventorySlots.LEGS);
			clothingArray.Insert(item_legs);
			
			EntityAI item_hips = player.GetInventory().CreateAttachmentEx(startingClothing.Belts.GetRandomElement(), InventorySlots.HIPS);
			clothingArray.Insert(item_hips);
			
			EntityAI item_feet = player.GetInventory().CreateAttachmentEx(startingClothing.Shoes.GetRandomElement(), InventorySlots.FEET);
			clothingArray.Insert(item_feet);
			
			EntityAI item_arm = player.GetInventory().CreateAttachmentEx(startingClothing.Armbands.GetRandomElement(), InventorySlots.ARMBAND);
			clothingArray.Insert(item_arm);
			
			EntityAI item_back = player.GetInventory().CreateAttachmentEx(startingClothing.Backpacks.GetRandomElement(), InventorySlots.BACK);
			clothingArray.Insert(item_back);
			
			if (GetExpansionSettings().GetSpawn().StartingClothing.SetRandomHealth)
			{
				for (int i = 0; i < clothingArray.Count(); i++ )
				{
					SetRandomHealth(clothingArray[i]);
				}
			}
			
			SetExpansionStartingGear(player);
		}
		
		Print("DayZExpansion::ExpansionEquipCharacter - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetRandomHealth
	// ------------------------------------------------------------
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}
}