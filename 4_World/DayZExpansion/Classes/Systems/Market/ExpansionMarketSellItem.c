/**
 * ExpansionMarketSell.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSell
{
    bool Valid;

    ExpansionMarketCurrency Price;
    ExpansionTraderBase Trader;
    int Time;
    int TotalAmount;

    ExpansionMarketItem Item;

    ref array< ref ExpansionMarketSellItem > Sell;

    void ExpansionMarketSell()
    {
        Sell = new array< ref ExpansionMarketSellItem >;
    }

    void ~ExpansionMarketSell()
    {
        delete Sell;
    }

    void Debug()
    {
        #ifdef EXPANSIONEXLOGPRINT
		Print( "Valid: " + Valid );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		//! Print( "Price: " + Price );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		//! Print( "Trader: " + Trader );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "Time: " + Time );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "TotalAmount: " + TotalAmount );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "Item: " + Item );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "Sell[N=" + Sell.Count() + "]:" );
		#endif
        
        for ( int i = 0; i < Sell.Count(); i++ )
        {
            Sell[i].Debug( i );
        }
    }

    void AddItem( bool canSell, int remainAmount, int takenAmount, EntityAI item )
    {
        ExpansionMarketSellItem itemSell = new ExpansionMarketSellItem;
        itemSell.CanSell = canSell;
        itemSell.RemainAmount = remainAmount;
        itemSell.ItemRep = item;
        Sell.Insert( itemSell );

        TotalAmount += takenAmount;
    }
}

class ExpansionMarketSellItem
{
    bool CanSell;
    int RemainAmount;
    EntityAI ItemRep;

    void Debug( int i )
    {
        #ifdef EXPANSIONEXLOGPRINT
		Print( " ExpansionMarketSellItem[" + i + "]: " );
		#endif
        
        #ifdef EXPANSIONEXLOGPRINT
		Print( "  CanSell: " + CanSell );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "  RemainAmount: " + RemainAmount );
		#endif

        #ifdef EXPANSIONEXLOGPRINT
		Print( "  ItemRep: " + ItemRep );
		#endif
    }

    void DestroyItem()
    {
        if ( RemainAmount == 0 )
        {
            GetGame().ObjectDelete( ItemRep );
            return;
        }

		ItemBase itm;
        Magazine mag;
		if ( Class.CastTo( mag, ItemRep ) )
        {
		    mag.ServerSetAmmoCount( RemainAmount );
        } else if ( Class.CastTo( itm, ItemRep ) ) 
		{
			itm.SetQuantity( RemainAmount );
        }
    }
}