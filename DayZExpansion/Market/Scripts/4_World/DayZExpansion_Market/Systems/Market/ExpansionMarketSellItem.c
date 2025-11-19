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

typedef Param2<int, float> ExpansionMarketItemPriceTier;
typedef array<ref ExpansionMarketItemPriceTier> ExpansionMarketItemPriceTiers;

class ExpansionMarketSell
{
	bool Valid;

	int Price;
	ExpansionTraderObjectBase Trader;
	int Time;

	//! Total amount to sell (without modifiers and attachments)
	int TotalAmount;

	ref ExpansionMarketItem Item;

	ref array<ref ExpansionMarketSellItem> Sell;
	
	PlayerBase m_Player;

	// ------------------------------------------------------------
	// ExpansionMarketSell Constructor
	// ------------------------------------------------------------
	void ExpansionMarketSell(PlayerBase player = null)
	{
		Sell = new array<ref ExpansionMarketSellItem>;
		m_Player = player;
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
	
	ExpansionMarketSellItem AddSellItem(int takenAmount, float addStockAmount, EntityAI item, string className = "")
	{
		EXError.ErrorOnce(this, "DEPRECATED, use AddSellItemEx");
		int soldAmount = addStockAmount;
		ExpansionMarketItem marketItem;
		if (className)
			marketItem = ExpansionMarketCategory.GetGlobalItem(className);
		return AddSellItemEx(takenAmount, soldAmount, 1.0, item, marketItem);
	}

	ExpansionMarketSellItem AddSellItem(int takenAmount, int soldAmount, float incrementStockModifier, EntityAI item, string className = "")
	{
		EXError.ErrorOnce(this, "DEPRECATED, use AddSellItemEx");
		ExpansionMarketItem marketItem;
		if (className)
			marketItem = ExpansionMarketCategory.GetGlobalItem(className);
		return AddSellItemEx(takenAmount, soldAmount, 1.0, item, marketItem);
	}

	ExpansionMarketSellItem AddSellItemEx(int takenAmount, int soldAmount, float incrementStockModifier, EntityAI item, ExpansionMarketItem marketItem)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "AddItem");
#endif
		
		ExpansionMarketSellItem itemSell = new ExpansionMarketSellItem;
		itemSell.m_Item = marketItem;
		itemSell.TakenAmount = takenAmount;
		itemSell.SoldAmount = soldAmount;
		itemSell.ItemRep = item;  //! Can be NULL! (ammo in mags/ammopiles are not entities)
		itemSell.ClassName = marketItem.ClassName;
		if (item)
			itemSell.IsEntity = true;
		Sell.Insert(itemSell);

		itemSell.AddStockAmount = soldAmount * incrementStockModifier;
		return itemSell;
	}
}

class ExpansionMarketSellItem
{
	ExpansionMarketItem m_Item;
	int TakenAmount;
	int SoldAmount;
	EntityAI ItemRep;
	string ClassName;
	bool IsEntity;

	//! Amount to be added to trader stock when selling (including modifiers)
	float AddStockAmount;

	//! For debug purposes
	int Price;
	ref ExpansionMarketItemPriceTiers PriceTiers = {};
		
	// ------------------------------------------------------------
	// ExpansionMarketSellItem Debug
	// ------------------------------------------------------------
	void Debug(int i)
	{
		Print("ExpansionMarketSellItem::Debug - ExpansionMarketSellItem[" + i + "]: ");
		Print("ExpansionMarketSellItem::Debug - ClassName: " + ClassName);
		Print("ExpansionMarketSellItem::Debug - TakenAmount: " + TakenAmount);
		Print("ExpansionMarketSellItem::Debug - AddStockAmount: " + AddStockAmount);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketSellItem DestroyItem
	// ------------------------------------------------------------
	void DestroyItem()
	{
		if (!ItemRep)
			return;

		//! @note if TakenAmount is zero item is destroyed without checking amount
		int remainAmount;
		if (TakenAmount > 0)
		{
			int amount = ExpansionMarketModule.GetInstance().GetItemAmount(ItemRep);
			remainAmount = amount - TakenAmount;
		}

		if (remainAmount <= 0)
		{
			#ifdef EXPANSIONMODVEHICLE
			auto vehicle = ExpansionVehicle.Get(ItemRep);
			if (vehicle)
			{
				//! Delete all keys for vehicle
				array< ExpansionCarKey > keys;
				ExpansionCarKey.GetKeysForVehicle(vehicle, keys);
				foreach (ExpansionCarKey key : keys)
				{
					g_Game.ObjectDelete(key);
				}
			}
			#endif

			g_Game.ObjectDelete(ItemRep);
			return;
		}

		ItemBase itm;
		Magazine mag;
		Ammunition_Base ammo;
		if (Class.CastTo(mag, ItemRep))
		{
			mag.ServerSetAmmoCount(remainAmount);
		}
		else if (Class.CastTo(ammo, ItemRep))
		{
			ammo.ServerSetAmmoCount(remainAmount);
		}
		else if ( Class.CastTo(itm, ItemRep)) 
		{
			itm.SetQuantity(remainAmount);
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

	void OnReceive(ParamsReadContext ctx, int mainItemID)
	{
		ctx.Read(m_ZoneSellPricePercent);

		int count;
		ctx.Read(count);

		for (int i = 0; i < count; i++)
		{
			auto debugItem = new ExpansionMarketSellDebugItem;
			debugItem.OnReceive(ctx, i == 0);
			if (i == 0)
				debugItem.ItemID = mainItemID;
			ExpansionMarketItem item = ExpansionMarketCategory.GetGlobalItem(debugItem.ItemID, false);
			if (item)
				debugItem.ClassName = item.ClassName;
			else
				debugItem.ClassName = "ERROR: ITEM NOT FOUND";
			if (debugItem.ParentID > -1)
			{
				ExpansionMarketItem parent = ExpansionMarketCategory.GetGlobalItem(debugItem.ParentID, false);
				if (parent)
					debugItem.Parent = parent.ClassName;
				else
					debugItem.Parent = "ERROR: ITEM NOT FOUND";
			}
			else
			{
				debugItem.Parent = "";
			}
			m_Items.Insert(debugItem);
		}
	}

	void Dump()
	{
		EXLogPrint("| Zone SellPricePercent: " + m_ZoneSellPricePercent);
		EXLogPrint("| Items: " + m_Items.Count());
		foreach (int i, ExpansionMarketSellDebugItem debugItem: m_Items)
		{
			EXLogPrint("| Item " + (i + 1).ToString());
			EXLogPrint("|   ItemID: " + debugItem.ItemID);
			EXLogPrint("|   ClassName: " + debugItem.ClassName);
			EXLogPrint("|   MaxPriceThreshold: " + debugItem.MaxPriceThreshold);
			EXLogPrint("|   MinPriceThreshold: " + debugItem.MinPriceThreshold);
			EXLogPrint("|   SellPricePercent: " + debugItem.SellPricePercent);
			EXLogPrint("|   MaxStockThreshold: " + debugItem.MaxStockThreshold);
			EXLogPrint("|   MinStockThreshold: " + debugItem.MinStockThreshold);
			EXLogPrint("|   Stock: " + debugItem.Stock);
			EXLogPrint("|   SoldAmount: " + debugItem.SoldAmount);
			EXLogPrint("|   AddStockAmount: " + debugItem.AddStockAmount);
			EXLogPrint("|   Price: " + debugItem.Price);
			EXLogPrint("|   PriceTiers");
			foreach (auto itemPrice: debugItem.PriceTiers)
			{
				EXLogPrint("|     Price at stock #: " + itemPrice.param1 + " at " + itemPrice.param2);
			}
		}
	}
}

class ExpansionMarketSellDebugItem
{
	int ItemID;
	string ClassName;

	int ParentID;
	string Parent;

	int MaxPriceThreshold;
	int MinPriceThreshold;

	float SellPricePercent;

	int MaxStockThreshold;
	int MinStockThreshold;

	bool StockOnly;
	int Stock;

	int EntityHealthLevel;
	float EntityQuantity;
	int EntityFoodStage;

	int SoldAmount;
	float AddStockAmount;
	int Price;
	ref ExpansionMarketItemPriceTiers PriceTiers;

	void ExpansionMarketSellDebugItem(ExpansionMarketSellItem sellItem = null, ExpansionMarketTraderZone zone = null)
	{
		if (!sellItem || !zone)
			return;

		ExpansionMarketItem item = sellItem.m_Item;
		if (!item)
			return;

		ItemID = item.ItemID;
		ClassName = item.ClassName;
		if (item.m_Parent)
		{
			ParentID = item.m_Parent.ItemID;
			Parent = item.m_Parent.ClassName;
		}
		else
		{
			ParentID = -1;
			Parent = "";
		}
		MaxPriceThreshold = item.MaxPriceThreshold;
		MinPriceThreshold = item.MinPriceThreshold;
		SellPricePercent = item.SellPricePercent;
		MaxStockThreshold = item.MaxStockThreshold;
		MinStockThreshold = item.MinStockThreshold;
		if (item.m_StockOnly)
			StockOnly = true;
		if (item.IsStaticStock())
			Stock = ExpansionMarketStock.Static;
		else
			Stock = zone.GetStock(item.ClassName);
		if (sellItem.ItemRep)
		{
			EntityHealthLevel = sellItem.ItemRep.GetHealthLevel();
			EntityQuantity = sellItem.ItemRep.GetQuantity();
			Edible_Base edible;
			if (Class.CastTo(edible, sellItem.ItemRep) && edible.HasFoodStage())
				EntityFoodStage = edible.GetFoodStageType();
			else
				EntityFoodStage = FoodStageType.NONE;
		}
		else
		{
			EntityHealthLevel = ExpansionMarketHealthLevel.NOT_APPLICABLE;
			EntityQuantity = -1;
			EntityFoodStage = FoodStageType.NONE;
		}
		SoldAmount = sellItem.SoldAmount;
		AddStockAmount = sellItem.AddStockAmount;
		Price = sellItem.Price;
		PriceTiers = sellItem.PriceTiers;
	}

	void OnSend(ParamsWriteContext ctx, bool isMainItem = false)
	{
		if (!isMainItem)
			ctx.Write(ItemID);

		ctx.Write(ParentID);

		ctx.Write(MaxPriceThreshold);
		ctx.Write(MinPriceThreshold);

		//ctx.Write(SellPricePercent);

		ctx.Write(MaxStockThreshold);
		ctx.Write(MinStockThreshold);

		//ctx.Write(StockOnly);
		ctx.Write(Stock);

		//ctx.Write(EntityHealthLevel);
		ctx.Write(EntityQuantity);
		//ctx.Write(EntityFoodStage);

		//! Network optimization: Pack StockOnly, EntityHealthLevel, EntityFoodStage and SellPricePercent into one 32-bit int
		//! (8 bits for StockOnly and EntityHealthLevel combined, 8 bits for EntityFoodStage, 16 bits for SellPricePercent)
		//! @note for EntityFoodStage, we use 0x0..0x7f for 0..127 and 0x80..0xff for -128..-1, this needs to be dealt with when decoding!
		//! @note for SellPricePercent, we use 0x0..0x00007fff for 0..32767 and 0x00008000..0x0000ffff for -32768..-1, this needs to be dealt with when decoding!
		int param1 = StockOnly;
		param1 |= EntityHealthLevel << 4;
		int sellPricePercentEncoded = CF_Cast<float, int>.Reinterpret(SellPricePercent) >> 16;  //! Convert to integer representation of bfloat16
		int packed = ((param1 & 0xff) << 24) | ((EntityFoodStage & 0xff) << 16) | (sellPricePercentEncoded & 0x0000ffff);
		ctx.Write(packed);

		ctx.Write(SoldAmount);
		ctx.Write(AddStockAmount);
		ctx.Write(Price);

		ctx.Write(PriceTiers.Count());
		foreach (auto itemPrice: PriceTiers)
		{
			ctx.Write(itemPrice.param1);  //! price
			ctx.Write(itemPrice.param2);  //! stock
		}
	}

	void OnReceive(ParamsReadContext ctx, bool isMainItem = false)
	{
		if (!isMainItem)
			ctx.Read(ItemID);

		ctx.Read(ParentID);

		ctx.Read(MaxPriceThreshold);
		ctx.Read(MinPriceThreshold);

		//ctx.Read(SellPricePercent);

		ctx.Read(MaxStockThreshold);
		ctx.Read(MinStockThreshold);

		//ctx.Read(StockOnly);
		ctx.Read(Stock);

		//ctx.Read(EntityHealthLevel);
		ctx.Read(EntityQuantity);
		//ctx.Read(EntityFoodStage);

		int packed;
		ctx.Read(packed);

		int param1 = packed >> 24;

		StockOnly = param1 & 0x0f;
		EntityHealthLevel = (param1 & 0xf0) >> 4;

		EntityFoodStage = (packed & 0x00ff0000) >> 16;
		if (EntityFoodStage > 0x7f)
			EntityFoodStage -= 0x100;

		int sellPricePercentEncoded = packed & 0x0000ffff;
		if (sellPricePercentEncoded > 0x00007fff)
			sellPricePercentEncoded -= 0x00010000;

		//! Convert integer representation of bfloat16 back to float
		SellPricePercent = CF_Cast<int, float>.Reinterpret(sellPricePercentEncoded << 16);

		ctx.Read(SoldAmount);
		ctx.Read(AddStockAmount);
		ctx.Read(Price);

		int count;
		ctx.Read(count);
		PriceTiers = {};
		if (count < 0)  //! Safety against infinite recursion
			return;
		while (count--)
		{
			int price;
			ctx.Read(price);
			float stock;
			ctx.Read(stock);
			PriceTiers.Insert(new ExpansionMarketItemPriceTier(price, stock));
		}
	}
}
