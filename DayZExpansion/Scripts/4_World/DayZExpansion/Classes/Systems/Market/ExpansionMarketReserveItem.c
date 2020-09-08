/**
 * ExpansionMarketReserveItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketReserve
{
	bool Valid;

	ExpansionMarketCurrency Price;
	ExpansionTraderBase Trader;
	int Time;

	ExpansionMarketItem RootItem;

	ref array< ref ExpansionMarketReserveItem > Reserved;

	void ExpansionMarketReserve()
	{
		Reserved = new array< ref ExpansionMarketReserveItem >;
	}

	void ~ExpansionMarketReserve()
	{
		delete Reserved;
	}

	void Debug()
	{
		/*
		#ifdef EXPANSIONEXLOGPRINT
		Print( "Valid: " + Valid );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Price: " + Price );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Trader: " + Trader );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Time: " + Time );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "RootItem: " + RootItem );
		#endif
		*/

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Reserved[N=" + Reserved.Count() + "]:" );
		#endif
		
		for ( int i = 0; i < Reserved.Count(); i++ )
		{
			Reserved[i].Debug( i );
		}
	}

	void AddReserved( ExpansionMarketTraderZone zone, string clsName, int amt, ExpansionMarketCurrency pce )
	{
		Price += pce;

		if ( zone )
		{
			zone.RemoveStock( clsName, amt, true );
		}

		Reserved.Insert( new ExpansionMarketReserveItem( clsName, amt, pce ) );
	}

	void ClearReserved( ExpansionMarketTraderZone zone )
	{
		if ( zone )
		{
			for ( int i = 0; i < Reserved.Count(); i++ )
			{
				zone.ClearReservedStock( Reserved[i].ClassName, Reserved[i].Amount );
			}
		}

		Reserved.Clear();
	}

	void SpawnObjects( ExpansionMarketTraderZone zone, ExpansionMarketModule module, Object parent )
	{
		for ( int i = 1; i < Reserved.Count(); i++ )
		{
			array< Object > objs = module.Spawn( Trader, EntityAI.Cast( parent ), Reserved[i] );

			zone.ClearReservedStock( Reserved[i].ClassName, Reserved[i].Amount );
			//zone.RemoveStock( Reserved[i].ClassName, Reserved[i].Amount, false );

			for ( int j = 0; j < objs.Count(); j++ )
			{
				GetGame().ObjectDelete( objs[j] );
			}
		}
	}
}

class ExpansionMarketReserveItem
{
	string ClassName;
	int Amount;
	ExpansionMarketCurrency Price;

	void Debug( int i )
	{
		/*
		#ifdef EXPANSIONEXLOGPRINT
		Print( " ExpansionMarketReserveItem[" + i + "]: " );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		Print( "  ClassName: " + ClassName );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "  Amount: " + Amount );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "  Price: " + Price );
		#endif
		*/
	}

	void ExpansionMarketReserveItem( string clsName, int amt, ExpansionMarketCurrency pce )
	{
		ClassName = clsName;
		Amount = amt;
		Price = pce;
	}
}