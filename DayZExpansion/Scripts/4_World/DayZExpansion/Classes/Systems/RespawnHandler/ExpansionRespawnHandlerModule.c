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
				
		if ( GetExpansionSettings().GetSpawn() )
		{
			ExpansionStartingGear gear;
			if ( Class.CastTo(gear, GetExpansionSettings().GetSpawn().StartingGear) )
			{
				if ( gear.UseUpperGear )
				{
					EntityAI itemTop = player.FindAttachmentBySlotName("Body");
					
					if ( itemTop )
					{
						for ( i = 0; i < gear.UpperGear.Count(); i++ )
						{
							itemTop.GetInventory().CreateInInventory( gear.UpperGear[i] );
						}
					}
				}
				
				if ( gear.UsePantsGear )
				{
					EntityAI itemPants = player.FindAttachmentBySlotName("Legs");
					
					if ( itemPants )
					{					
						for ( i = 0; i < gear.PantsGear.Count(); i++ )
						{
							itemPants.GetInventory().CreateInInventory( gear.PantsGear[i] );
						}
					}
				}
				
				if ( gear.UseBackpackGear )
				{
					EntityAI itemBag = player.FindAttachmentBySlotName("Back");
					if ( itemBag )
					{					
						for ( i = 0; i < gear.BackpackGear.Count(); i++ )
						{
							gear_item = itemBag.GetInventory().CreateInInventory( gear.BackpackGear[i] );
							if ( gear_item.CanPutAsAttachment( itemBag ) )
							{
								AddAttachment( itemBag, gear_item );
							}
						}
					}
				}
				
				if ( gear.UseVestGear )
				{
					EntityAI itemVest = player.FindAttachmentBySlotName("Vest");
					if ( itemVest )
					{					
						for ( i = 0; i < gear.VestGear.Count(); i++ )
						{
							gear_item = itemVest.GetInventory().CreateInInventory( gear.VestGear[i] );
							if ( gear_item.CanPutAsAttachment( itemVest ) )
							{
								AddAttachment( itemVest, gear_item );
							}
						}
					}
				}
				
				if ( gear.UsePrimaryWeapon )
				{
					EntityAI item_primary = player.GetInventory().CreateAttachmentEx( gear.PrimaryWeapon, InventorySlots.SHOULDER );
					if ( item_primary )
					{
						for ( i = 0; i < gear.PrimaryWeaponAttachments.Count(); i++ )
						{
							gear_item = item_primary.GetInventory().CreateInInventory( gear.PrimaryWeaponAttachments[i] );
							if ( gear_item.CanPutAsAttachment( item_primary ) )
							{
								AddAttachment( item_primary, gear_item );
							}
						}
					}
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
			
			if (GetExpansionSettings().GetSpawn().StartingClothing.SetRandomHeath)
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
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetRandomHealth
	// ------------------------------------------------------------
	void AddAttachment(EntityAI item, EntityAI attachment)
	{
		//! Check if attachment can fit to the item
		string attachment_name = attachment.ClassName();
		int slot = InventorySlots.GetSlotIdFromString( attachment_name );
		string slot_name = InventorySlots.GetSlotName( slot );

		Print("ExpansionRespawnHandlerModule::AddAttachment - attachment_name: " + attachment_name);
		Print("ExpansionRespawnHandlerModule::AddAttachment - slot: " + slot);
		Print("ExpansionRespawnHandlerModule::AddAttachment - slot_name: " + slot_name);
		
		if ( item.CanReceiveAttachment( attachment, slot ) )
			item.GetInventory().CreateAttachmentEx( attachment_name, slot );
	}
}