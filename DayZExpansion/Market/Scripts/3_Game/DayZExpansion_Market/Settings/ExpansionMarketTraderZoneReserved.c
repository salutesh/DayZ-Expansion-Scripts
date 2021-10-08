/**
 * ExpansionMarketTraderZoneReserved.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderZoneReserved
{
	ref map<string, int> ReservedStock;
	
	void ExpansionMarketTraderZoneReserved()
	{
		ReservedStock = new map<string, int>;
	}
		
	void SetReservedStock( string className, int stock )
	{
		className.ToLower();

		ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem( className );
		if ( !marketItem )
			return;
		
		if ( ReservedStock.Contains( className ) )
		{
			ReservedStock.Set( className, stock );
		} 
		else 
		{
			ReservedStock.Insert( className, stock );
		}
	}
	
	int GetReservedStock( string className )
	{
		className.ToLower();
		
		ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem( className );
		if ( !marketItem )
			return -1;
		
		int reserved = 0;
		if ( ReservedStock.Contains( className ) )
		{
			reserved = ReservedStock.Get( className );
		}
		
		return reserved;
	}
}