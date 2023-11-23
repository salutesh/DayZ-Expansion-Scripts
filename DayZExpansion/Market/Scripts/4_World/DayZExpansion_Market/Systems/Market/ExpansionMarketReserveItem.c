/**
 * ExpansionMarketReserveItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketReserve
{
	bool Valid;

	int Price;
	ExpansionTraderObjectBase Trader;
	int Time;

	ref ExpansionMarketItem RootItem;

	//! Total amount to buy (without modifiers and attachments)
	int TotalAmount;

	ref array< ref ExpansionMarketReserveItem > Reserved;

	void ExpansionMarketReserve()
	{
		Reserved = new array< ref ExpansionMarketReserveItem >;
	}

	void Debug()
	{
		Print("ExpansionMarketReserve - Valid: " + Valid);
		Print("ExpansionMarketReserve - Price: " + string.ToString(Price));
		Print("ExpansionMarketReserve - Trader: " + Trader);
		Print("ExpansionMarketReserve - Time: " + Time);
		Print("ExpansionMarketReserve - RootItem: " + RootItem);
		Print("ExpansionMarketReserve - ClassName: " + RootItem.ClassName);
		Print("ExpansionMarketReserve - Reserved[N=" + Reserved.Count() + "]:");
	
		for ( int i = 0; i < Reserved.Count(); i++ )
		{
			Reserved[i].Debug( i );
		}
	}

	void AddReserved(ExpansionMarketTraderZone zone, string clsName, int amt, int pce)
	{
		Price += pce;
		
		if (zone)
		{
			zone.RemoveStock(clsName, amt, true);
		}

		Reserved.Insert(new ExpansionMarketReserveItem(clsName, amt, pce ));
	}

	void ClearReserved( ExpansionMarketTraderZone zone )
	{
		Price = 0;
		TotalAmount = 0;

		if (zone)
		{
			for (int i = 0; i < Reserved.Count(); i++)
			{
				zone.ClearReservedStock(Reserved[i].ClassName, Reserved[i].Amount);
			}
		}

		Reserved.Clear();
	}
}

class ExpansionMarketReserveItem
{
	string ClassName;
	int Amount;
	int Price;

	void Debug(int i)
	{
		Print("ExpansionMarketReserveItem[" + i + "]: ");
		Print("ClassName: " + ClassName);
		Print("Amount: " + Amount);
		Print("Price: " + string.ToString(Price));
	}

	void ExpansionMarketReserveItem( string clsName, int amt, int pce )
	{
		ClassName = clsName;
		Amount = amt;
		Price = pce;
	}
}