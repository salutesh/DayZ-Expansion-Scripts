/**
 * ExpansionP2PMarketRecivedData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketRecivedData
{
	ref array<ref ExpansionP2PMarketListing> m_Listings = {}; 
	int m_TraderID = -1; 
	bool m_IsGlobal;
	string m_TraderName = "";
	string m_IconName = ""; 
	ref TStringArray m_Currencies = {}; 
	int m_DisplayCurrencyValue = -1; 
	string m_DisplayCurrencyName = ""; 
	int m_ListingsCount = 0; 
	int m_SoldListingsCount = 0;
	int m_SoldTotalIncome = 0;
	int m_OwnedListingsCount = 0;
	int m_ValidListingsCount = 0;
	
	void Debug()
	{
		if (!EXTrace.P2PMARKET)
			return;

		ErrorEx("Listings: " + m_Listings + " | Count: " + m_Listings.Count(), ErrorExSeverity.INFO);
		ErrorEx("Trader ID: " + m_TraderID, ErrorExSeverity.INFO);
		ErrorEx("Is global: " + m_IsGlobal, ErrorExSeverity.INFO);
		ErrorEx("Trader name: " + m_TraderName, ErrorExSeverity.INFO);
		ErrorEx("Icon path: " + m_IconName, ErrorExSeverity.INFO);
		ErrorEx("Currencies: " + ExpansionString.JoinStrings(m_Currencies), ErrorExSeverity.INFO);
		ErrorEx("Display currency value: " + m_DisplayCurrencyValue, ErrorExSeverity.INFO);
		ErrorEx("Display currency name: " + m_DisplayCurrencyName, ErrorExSeverity.INFO);
		ErrorEx("Listings count: " + m_ListingsCount, ErrorExSeverity.INFO);
		ErrorEx("Sold listings count: " + m_SoldListingsCount, ErrorExSeverity.INFO);
		ErrorEx("Sold total income: " + m_SoldTotalIncome, ErrorExSeverity.INFO);
		ErrorEx("Owned listings count: " + m_OwnedListingsCount, ErrorExSeverity.INFO);
		ErrorEx("Valid listings count: " + m_ValidListingsCount, ErrorExSeverity.INFO);
	}
};