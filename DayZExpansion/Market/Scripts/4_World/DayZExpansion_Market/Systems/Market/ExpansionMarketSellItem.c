/**
 * ExpansionMarketSell.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSell
{
	bool Valid;

	int Price;
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "AddItem");
#endif

		if (!className && item)
		{
			className = item.GetType();
			className.ToLower();
		}
		
		ExpansionMarketSellItem itemSell = new ExpansionMarketSellItem;
		itemSell.RemainAmount = remainAmount;
		itemSell.ItemRep = item;
		itemSell.ClassName = className;
		Sell.Insert(itemSell);

		itemSell.AddStockAmount = takenAmount * incrementStockModifier;
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
		Print("ExpansionMarketSellItem::Debug - ClassName: " + ClassName);
		Print("ExpansionMarketSellItem::Debug - RemainAmount: " + RemainAmount);
		Print("ExpansionMarketSellItem::Debug - AddStockAmount: " + AddStockAmount);
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

class ExpansionMarketSellDebug
{
	float m_ZoneSellPricePercent;
	autoptr array<ref ExpansionMarketSellDebugItem> m_Items;

	void ExpansionMarketSellDebug(ExpansionMarketSell marketSell = null, ExpansionMarketTraderZone zone = null)
	{
		m_Items = new array<ref ExpansionMarketSellDebugItem>;

		if (!marketSell || !zone)
			return;

		m_ZoneSellPricePercent = zone.SellPricePercent;

		foreach (ExpansionMarketSellItem sellItem: marketSell.Sell)
		{
			auto debugItem = new ExpansionMarketSellDebugItem(sellItem, zone);
			m_Items.Insert(debugItem);
		}
	}

	TStringArray GetItemClassNames()
	{
		TStringArray classNames();

		foreach (ExpansionMarketSellDebugItem debugItem: m_Items)
		{
			classNames.Insert(debugItem.ClassName);
		}

		return classNames;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_ZoneSellPricePercent);

		ctx.Write(m_Items.Count());

		int i;
		foreach (ExpansionMarketSellDebugItem debugItem: m_Items)
		{
			debugItem.OnSend(ctx, i == 0);
			i++;
		}
	}

	void OnReceive(ParamsReadContext ctx, string mainItemClassName)
	{
		ctx.Read(m_ZoneSellPricePercent);

		int count;
		ctx.Read(count);

		for (int i = 0; i < count; i++)
		{
			auto debugItem = new ExpansionMarketSellDebugItem;
			debugItem.OnReceive(ctx, i == 0);
			if (i == 0)
				debugItem.ClassName = mainItemClassName;
			m_Items.Insert(debugItem);
		}
	}

	void Dump()
	{
		EXLogPrint("| Zone SellPricePercent: " + m_ZoneSellPricePercent);
		EXLogPrint("| Items: " + m_Items.Count());
		int n;
		foreach (ExpansionMarketSellDebugItem debugItem: m_Items)
		{
			n++;
			EXLogPrint("| Item #" + n);
			EXLogPrint("|   ClassName: " + debugItem.ClassName);
			EXLogPrint("|   MaxPriceThreshold: " + debugItem.MaxPriceThreshold);
			EXLogPrint("|   MinPriceThreshold: " + debugItem.MinPriceThreshold);
			EXLogPrint("|   SellPricePercent: " + debugItem.SellPricePercent);
			EXLogPrint("|   MaxStockThreshold: " + debugItem.MaxStockThreshold);
			EXLogPrint("|   MinStockThreshold: " + debugItem.MinStockThreshold);
			EXLogPrint("|   Stock: " + debugItem.Stock);
			EXLogPrint("|   AddStockAmount: " + debugItem.AddStockAmount);
		}
	}
}

class ExpansionMarketSellDebugItem
{
	string ClassName;

	int MaxPriceThreshold;
	int MinPriceThreshold;

	float SellPricePercent;

	int MaxStockThreshold;
	int MinStockThreshold;

	int Stock;

	float AddStockAmount;

	void ExpansionMarketSellDebugItem(ExpansionMarketSellItem sellItem = null, ExpansionMarketTraderZone zone = null)
	{
		if (!sellItem || !zone)
			return;

		ExpansionMarketItem item = GetExpansionSettings().GetMarket().GetItem(sellItem.ClassName);
		if (!item)
			return;

		ClassName = item.ClassName;
		MaxPriceThreshold = item.MaxPriceThreshold;
		MinPriceThreshold = item.MinPriceThreshold;
		SellPricePercent = item.SellPricePercent;
		MaxStockThreshold = item.MaxStockThreshold;
		MinStockThreshold = item.MinStockThreshold;
		Stock = zone.GetStock(item.ClassName);
		AddStockAmount = sellItem.AddStockAmount;
	}

	void OnSend(ParamsWriteContext ctx, bool isMainItem = false)
	{
		if (!isMainItem)
			ctx.Write(ClassName);

		ctx.Write(MaxPriceThreshold);
		ctx.Write(MinPriceThreshold);

		ctx.Write(SellPricePercent);

		ctx.Write(MaxStockThreshold);
		ctx.Write(MinStockThreshold);

		ctx.Write(Stock);

		ctx.Write(AddStockAmount);
	}

	void OnReceive(ParamsReadContext ctx, bool isMainItem = false)
	{
		if (!isMainItem)
			ctx.Read(ClassName);

		ctx.Read(MaxPriceThreshold);
		ctx.Read(MinPriceThreshold);

		ctx.Read(SellPricePercent);

		ctx.Read(MaxStockThreshold);
		ctx.Read(MinStockThreshold);

		ctx.Read(Stock);

		ctx.Read(AddStockAmount);
	}
}
