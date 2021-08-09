/**
 * ExpansionCraftingModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCraftingModule
 * @brief		This class handle expansion crafting module
 **/

class ExpansionCraftingModule: JMModuleBase
{	
	// ------------------------------------------------------------
	// ExpansionCraftingModule Constructor
	// ------------------------------------------------------------
	void ExpansionCraftingModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCraftingModule::ExpansionMonitorModule - Start");
		#endif
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCraftingModule::ExpansionMonitorModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		
	}
	
	override bool IsEnabled()	
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule GetRPCMin
	// ------------------------------------------------------------
	/*override int GetRPCMin()
	{
		return ExpansionCraftingModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionCraftingModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule OnRPC
	// Called on client
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCraftingModule::OnRPC - Start");
		#endif

		switch (rpc_type)
		{
		case ExpansionCraftingModuleRPC.RequestPrepareCrafting:
			RPC_RequestPrepareCrafting(ctx, sender, target);
			break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCraftingModule::OnRPC - End");
		#endif
	}*/
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule RequestPrepareCrafting
	// Called on client
	// ------------------------------------------------------------
	void RequestPrepareCrafting(ExpansionBookCraftingRecipe recipe)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPrepareCrafting - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		Exec_RequestPrepareCrafting(PlayerBase.Cast(GetGame().GetPlayer()), recipe);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPrepareCrafting - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule RequestCrafting
	// Called on client
	// ------------------------------------------------------------
	/*void RequestPrepareCrafting(ExpansionBookCraftingRecipe recipe)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPrepareCrafting - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(recipe);
		rpc.Send(null, ExpansionCraftingModuleRPC.RequestPrepareCrafting, true, null);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMonitorModule::RequestPrepareCrafting - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule RPC_RequestPrepareCrafting
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPrepareCrafting(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!IsMissionHost())
			return;
		
		ExpansionBookCraftingRecipe recipe;
		if (!ctx.Read(recipe))
		{
			Error("ExpansionCraftingModule::RPC_RequestPrepareCrafting - Could not get crafting recipe!");
			return;
		}
		
		Exec_RequestPrepareCrafting(senderRPC, recipe);
	}*/
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule Exec_RequestPrepareCrafting
	// Called on server
	// ------------------------------------------------------------
	private void Exec_RequestPrepareCrafting(PlayerBase player, ExpansionBookCraftingRecipe recipe)
	{
		//PlayerBase player = GetPlayerObjectByIdentity(sender);
		if (player)
		{	
			array<EntityAI> playerIngredients1 = new array<EntityAI>;
			array<EntityAI> playerIngredients2 = new array<EntityAI>;
			
			Print("Exec_RequestPrepareCrafting - HasIngedient: " + HasIngedient(player, recipe.Ingredients[0], playerIngredients1));
			Print("Exec_RequestPrepareCrafting - HasIngedient: " + HasIngedient(player, recipe.Ingredients[0], playerIngredients2));
			
			if (HasIngedient(player, recipe.Ingredients[0], playerIngredients1) && HasIngedient(player, recipe.Ingredients[1], playerIngredients2))
			{
				int i;
				ItemBase item_base;
				float stackable;
				int networkLow;
				int networkHigh;
				ScriptRPC rpc;
				
				for (i = 0; i < playerIngredients1.Count(); ++i) 
				{					
					if ( player.GetHumanInventory().CanAddEntityIntoHands( playerIngredients1[i] ) )
					{
						item_base = ItemBase.Cast( playerIngredients1[i] );
						stackable = item_base.GetTargetQuantityMax(-1);
						
						if ( stackable == 0 || item_base.GetQuantity() <= stackable )
						{
							player.PredictiveTakeEntityToHands( item_base );		
						}
						else if ( stackable != 0 && stackable <= item_base.GetQuantity() )
						{
							item_base.SplitIntoStackMaxHandsClient( player );
						}
						
						item_base.GetNetworkID(networkLow, networkHigh);
						
						rpc = new ScriptRPC();
						rpc.Write(networkLow);
						rpc.Write(networkHigh);
						rpc.Send(null, ExpansionCraftingModuleRPC.RequestPrepareCrafting, true, null);
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DropItem, 3, false, player, item_base);
					}
				}
				
				for (i = 0; i < playerIngredients2.Count(); ++i) 
				{					
					if ( player.GetHumanInventory().CanAddEntityIntoHands( playerIngredients2[i] ) )
					{
						item_base = ItemBase.Cast( playerIngredients2[i] );
						stackable = item_base.GetTargetQuantityMax(-1);
						
						if ( stackable == 0 || item_base.GetQuantity() <= stackable )
						{
							player.PredictiveTakeEntityToHands( item_base );		
						}
						else if ( stackable != 0 && stackable <= item_base.GetQuantity() )
						{
							item_base.SplitIntoStackMaxHandsClient( player );
						}
						
						item_base.GetNetworkID(networkLow, networkHigh);
						
						rpc = new ScriptRPC();
						rpc.Write(networkLow);
						rpc.Write(networkHigh);
						rpc.Send(null, ExpansionCraftingModuleRPC.RequestPrepareCrafting, true, null);
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DropItem, 3, false, player, item_base);
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule RPC_RequestPrepareCrafting
	// ------------------------------------------------------------	
	private void RPC_RequestPrepareCrafting(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!IsMissionHost())
			return;
		
		int networkLow;
		if (!ctx.Read(networkLow))
		{
			Error("ExpansionCraftingModule::RPC_RequestPrepareCrafting - Could not get networkLow ID!");
			return;
		}
		
		int networkHigh;
		if (!ctx.Read(networkHigh))
		{
			Error("ExpansionCraftingModule::RPC_RequestPrepareCrafting - Could not get networkHigh ID!");
			return;
		}
		
		PlayerBase player = GetPlayerObjectByIdentity(senderRPC);
		if (!player)
		{
			Error("ExpansionCraftingModule::RPC_RequestPrepareCrafting - Could not get player!");
			return;
		}
		
		EntityAI item = EntityAI.Cast(GetGame().GetObjectByNetworkId(networkLow, networkHigh));
		if (!item)
		{
			Error("ExpansionCraftingModule::RPC_RequestPrepareCrafting - Could not get item_base!");
			return;
		}
		
		//DropItem(player, item);
		//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DropItem, 3, false, player, item);
		//player.PhysicalPredictiveDropItem(player.GetItemInHands());
	}
	
	// ------------------------------------------------------------
	// ExpansionCraftingModule DropItem
	// ------------------------------------------------------------	
	void DropItem(PlayerBase player, EntityAI item)
	{
		bool ret;
		if ( GetGame().IsMultiplayer() )
        {
            ret = player.GetInventory().DropEntity( InventoryMode.SERVER, player, item );
        }
        else
        {
            ret = player.GetInventory().DropEntity( InventoryMode.LOCAL, player, item );
        }
		
		//player.PhysicalPredictiveDropItem(player.GetItemInHands());
	}

	// ------------------------------------------------------------
	// Expansion bool HasIngedient
	// Check if player has given ingredients
	// ------------------------------------------------------------	
	private bool HasIngedient(PlayerBase player, array<ref ExpansionBookCraftingItem> ingedients, out array<EntityAI> usableEntiys)
	{
		if (!usableEntiys)
			usableEntiys = new array<EntityAI>;
		
		array<EntityAI> playerItems = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, playerItems);
		
		for (int j = 0; j < ingedients.Count(); ++j) 
		{
			foreach (string name: ingedients[j].ClassNames)
			{
				int playerAmount = GetAmountInInventory(name, playerItems, usableEntiys);
				int amountNeeded = ingedients[j].Amount;
				if (playerAmount >= amountNeeded)
				{
					return true;
				}		
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion Int GetAmountInInventory
	// Gets the amount of crafting items the player has in his inventroy
	// ------------------------------------------------------------
	//! Returns positive number if at least one usable item found, negative number if only unusable items found
	int GetAmountInInventory(string itemName, array< EntityAI > entitys, out array<EntityAI> usableEntiys)
	{
		itemName.ToLower();
		
		int usable;
		int unusable;

		if (!usableEntiys)
			usableEntiys = new array<EntityAI>;
		
		for (int i = 0; i < entitys.Count(); i++)
		{
			EntityAI entity = entitys[i];
			if (entity == NULL)
				continue;

			string entName = entity.GetType();
			entName.ToLower();
			
			if (entName != itemName)
				continue;

			int amount = GetItemAmount(entity);

			if (amount > 0)
			{
				usable += amount;
				usableEntiys.Insert(entity);
			}
			else
			{
				unusable += amount;
			}
		}

		if (usable > 0)
		{
			return usable;
		}
		else
		{
			return unusable;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetItemAmount
	// Returns the amount of the given item the player has, taking into account whether it's stackable or not.
	// If the item is not usable (might be a permanent condition because it's ruined
	// or a temporary one because it's attached to something or has cargo) returns -1
	// If the item is not stackable returns 1.
	// ------------------------------------------------------------
	int GetItemAmount(EntityAI item)
	{
		int amount = -1;
		
		ItemBase itemBase = ItemBase.Cast(item);
			
		if (!CanUseItem(item, true))
			return -1;
		
		if (item.IsKindOf("Container_Base"))
		{
			amount = 1;
		}
		else if (item.IsKindOf("ExpansionSpraycanBase"))
		{
			amount = 1;
		}
		else if (item.IsKindOf("Edible_Base"))
		{
			//! Food and liquid containers
			amount = 1;
		}
		else if (item.IsInherited(MagazineStorage))
		{
			amount = 1;
		}
		else if (item.IsKindOf("Ammunition_Base"))
		{
			if (item.IsAmmoPile())
			{
				//! This looks like a wierd method but this how we get the actual ammo amount from an ammo pile
				Magazine magazine = Magazine.Cast(item);
				amount = magazine.GetAmmoCount();
			}
		}
		else if (item.HasEnergyManager())
		{
			amount = 1;
		}
		else if (itemBase && itemBase.HasQuantity())
		{
			amount = item.GetQuantity();
		}
		else
		{
			amount = 1;
		}
		
		return amount;
	}
	
	// ------------------------------------------------------------
	// Expansion Bool CanSellItem
	// ------------------------------------------------------------
	bool CanUseItem(EntityAI item, bool checkIfRuined = true)
	{
		if (checkIfRuined && item.IsRuined())
			return false;

		if (item.GetInventory())
		{
			//! Check if the item has a container and any items in it
			if (item.HasAnyCargo())
				return false;

			bool isVehicle;

			if (item.IsInherited(CarScript))
				isVehicle = true;
			#ifdef EXPANSIONMODVEHICLE
			else if (item.IsInherited(ExpansionVehicleBase))
				isVehicle = true;
			#endif

			if (isVehicle)
			{
				//! Check if any of the vehicle's attachments has any cargo
				for (int i = 0; i < item.GetInventory().AttachmentCount(); i++)
				{
					EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(i);
					if (attachment && attachment.HasAnyCargo())
						return false;
				}
			}
			else if (item.GetInventory().AttachmentCount() > 0)
			{
				return false;
			}

			//! Check if the item is attached to something that is not the player
			//! (allows selling empty clothes that you are wearing)
			if (item.GetInventory().IsAttachment() && !item.GetHierarchyParent().IsInherited(PlayerBase))
				return false;
		}
		
		//! Check if the item is a magazine and has any ammo in it
		if (item.IsInherited(MagazineStorage))
		{
			MagazineStorage magStorage = MagazineStorage.Cast(item);
			if (magStorage.GetAmmoCount() > 0)
				return false;
		}

		#ifdef EXPANSIONMODVEHICLE
		//! If this is a master key of a vehicle, don't allow to sell it
		ExpansionCarKey key;
		if (Class.CastTo(key, item) && key.IsMaster())
			return false;
		#endif

		return true;
	}
}