/**
 * ExpansionMarketSell.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSell
{
	bool Valid;

	ExpansionMarketCurrency Price;
	ref ExpansionTraderObjectBase Trader;
	int Time;

	//! Total amount to sell (without modifiers and attachments)
	int TotalAmount;

	ref ExpansionMarketItem Item;

	ref array<ref ExpansionMarketSellItem> Sell;
	
	// ------------------------------------------------------------
	// ExpansionMarketSell Constructor
	// ------------------------------------------------------------
	void ExpansionMarketSell()
	{
		Sell = new array<ref ExpansionMarketSellItem>;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketSell Destructor
	// ------------------------------------------------------------
	void ~ExpansionMarketSell()
	{
		delete Sell;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketSell Debug
	// ------------------------------------------------------------
	void Debug()
	{
		Print("ExpansionMarketSell::Debug - Valid: " + Valid);
		Print("ExpansionMarketSell::Debug - Price: " + Price);
		Print("ExpansionMarketSell::Debug - Trader: " + Trader);
		Print("ExpansionMarketSell::Debug - Time: " + Time);
		Print("ExpansionMarketSell::Debug - TotalAmount: " + TotalAmount);
		Print("ExpansionMarketSell::Debug - Item: " + Item);
		Print("ExpansionMarketSell::Debug - ClassName: " + Item.ClassName);
		Print("ExpansionMarketSell::Debug - Sell[N=" + Sell.Count() + "]:");
		
		for (int i = 0; i < Sell.Count(); i++)
		{
			Sell[i].Debug(i);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketSell AddItem
	// ------------------------------------------------------------
	void AddItem(int remainAmount, int takenAmount, float incrementStockModifier, EntityAI item, string className = "")
	{
		Print( "ExpansionMarketSell::AddItem - Start - remaining " + remainAmount + ", taken " + takenAmount + ", increment stock modifier " + incrementStockModifier);
		
		ExpansionMarketSellItem itemSell = new ExpansionMarketSellItem;
		itemSell.RemainAmount = remainAmount;
		itemSell.ItemRep = item;
		if (!className && item)
			className = item.GetType();
		itemSell.ClassName = className;
		Sell.Insert(itemSell);

		itemSell.AddStockAmount = takenAmount * incrementStockModifier;
		
		Print( "ExpansionMarketSell::AddItem - End");
	}
}

class ExpansionMarketSellItem
{
	int RemainAmount;
	EntityAI ItemRep;
	string ClassName;

	//! Amount to be added to trader stock when selling (including modifiers)
	float AddStockAmount;
		
	// ------------------------------------------------------------
	// ExpansionMarketSellItem Debug
	// ------------------------------------------------------------
	void Debug(int i)
	{
		Print("ExpansionMarketSellItem::Debug - ExpansionMarketSellItem[" + i + "]: ");
		Print("ExpansionMarketSellItem::Debug - RemainAmount: " + RemainAmount);
		Print("ExpansionMarketSellItem::Debug - ClassName: " + ClassName);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketSellItem DestroyItem
	// ------------------------------------------------------------
	void DestroyItem()
	{
		if (!ItemRep)
			return;

		if (RemainAmount == 0)
		{
			#ifdef EXPANSIONMODVEHICLE
			if (ItemRep.IsInherited(CarScript) || ItemRep.IsInherited(ExpansionVehicleBase))
			{
				//! Delete all keys for vehicle
				array< ExpansionCarKey > keys;
				ExpansionCarKey.GetKeysForVehicle(ItemRep, keys);
				foreach (ExpansionCarKey key : keys)
				{
					GetGame().ObjectDelete(key);
				}
			}
			#endif

			GetGame().ObjectDelete(ItemRep);
			return;
		}

		ItemBase itm;
		Magazine mag;
		Ammunition_Base ammo;
		if (Class.CastTo(mag, ItemRep))
		{
			mag.ServerSetAmmoCount(RemainAmount);
		}
		else if (Class.CastTo(ammo, ItemRep))
		{
			ammo.ServerSetAmmoCount(RemainAmount);
		}
		else if ( Class.CastTo(itm, ItemRep)) 
		{
			itm.SetQuantity(RemainAmount);
		}
	}
}