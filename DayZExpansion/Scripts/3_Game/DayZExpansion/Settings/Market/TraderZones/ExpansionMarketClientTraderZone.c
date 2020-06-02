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

		ExpansionMarketItem marketItem;
		if ( !GetExpansionSettings().GetMarket().GetItem( className, marketItem ) )
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

				if ( !marketItem.StaticStock )
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

		//! Print("[ExpansionMarketClientTraderZone] GetStock " + m_ZoneName + " " + className );

		return Stock.Get( className );
	}
}