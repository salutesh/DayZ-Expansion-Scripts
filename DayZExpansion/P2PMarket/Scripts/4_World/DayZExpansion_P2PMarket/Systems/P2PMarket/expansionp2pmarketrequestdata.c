/**
 * ExpansionP2PMarketRequestData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketRequestData
{
	bool m_Init = false;
	int m_TraderID = -1;
	int m_ListingTraderID = -1;
	int m_PageIndex = 0;
	int m_CategoryIndex = -1;
	int m_SubCategoryIndex = -1;
	ref array<string> m_SearchTypeNames = new array<string>;
	ExpansionP2PMarketModuleCallback m_CallbackType = ExpansionP2PMarketModuleCallback.None;
	string m_MessageTypeName = "";
	int m_MessagePrice = 0;
	int m_MessagePriceString = 0;
	bool m_OwnedListings = false;
	string m_GlobalIDText = "";
	
	void Debug()
	{
		if (!EXTrace.P2PMARKET)
			return;

		ErrorEx("Init: " + m_Init, ErrorExSeverity.INFO);
		ErrorEx("Trader ID: " + m_TraderID, ErrorExSeverity.INFO);
		ErrorEx("Listing trader ID: " + m_ListingTraderID, ErrorExSeverity.INFO);
		ErrorEx("Page Index: " + m_PageIndex, ErrorExSeverity.INFO);
		ErrorEx("Category Index: " + m_CategoryIndex, ErrorExSeverity.INFO);
		ErrorEx("Sub-Category Index: " + m_SubCategoryIndex, ErrorExSeverity.INFO);
		ErrorEx("Search tyoe names: " + m_SearchTypeNames.Count(), ErrorExSeverity.INFO);
		ErrorEx("Callback type: " + typename.EnumToString(ExpansionP2PMarketModuleCallback, m_CallbackType), ErrorExSeverity.INFO);
		ErrorEx("Message typename: " + m_MessageTypeName, ErrorExSeverity.INFO);
		ErrorEx("Message price: " + m_MessagePrice, ErrorExSeverity.INFO);
		ErrorEx("Message price string: " + m_MessagePriceString, ErrorExSeverity.INFO);
		ErrorEx("Owned listings: " + m_OwnedListings, ErrorExSeverity.INFO);
		ErrorEx("Global ID text: " + m_GlobalIDText, ErrorExSeverity.INFO);
	}
};