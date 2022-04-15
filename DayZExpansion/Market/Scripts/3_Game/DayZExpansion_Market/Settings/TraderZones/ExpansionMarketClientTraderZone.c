/**
 * ExpansionMarketClientTraderZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketClientTraderZone: ExpansionMarketTraderZone
{
	void ExpansionMarketClientTraderZone()
	{
		m_ZoneName = "Client";
	}

	override void Defaults()
	{
		
	}

	override void RemoveStock( string className, int stock, bool inReserve = false )
	{
		className.ToLower();

		//! Print("[ExpansionMarketClientTraderZone] RemoveStock " + m_ZoneName + " " + className + " " + stock);

		ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem( className );
		if ( !marketItem )
			return;

		if ( Stock.Contains( className ) )
		{
			//! Print("[ExpansionMarketClientTraderZone] RemoveStock contains " + className );

			if (inReserve)
			{
				
			} 
			else
			{
				int new_stock = 0;

				if ( !marketItem.IsStaticStock() )
				{
					new_stock = Stock.Get( className ) - stock;
					
					if ( new_stock < 0 )
						new_stock = 0;
				}

				Stock.Set( className, new_stock );
			}

			//! Print("[ExpansionMarketClientTraderZone] RemoveStock set " + className + " new_stock : " + new_stock);
		} 
		else 
		{
			//! Print("[ExpansionMarketClientTraderZone] RemoveStock does not contain " + className);
			Stock.Insert( className, 0 );
		}
	}

	override int GetStock( string className, bool actual = false )
	{
		className.ToLower();

		if ( !ItemExists( className ) )
			return -3; // -3 means the item does not exist in this trader zone

		if (actual)
			Error(ToString() + "::GetStock - cannot get actual (non-reserved) stock on client!");

		return Stock.Get( className );
	}
};