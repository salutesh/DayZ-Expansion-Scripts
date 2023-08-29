/**
 * ExpansionP2PMarketMenuListHeader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuListHeader: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuListHeaderController m_P2PMarketMenuListHeaderController;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;

	protected ButtonWidget item_name_button;
	protected TextWidget item_name_text;
	protected ImageWidget item_name_icon;

	protected ButtonWidget time_button;
	protected TextWidget time_text;
	protected ImageWidget time_icon;

	protected ButtonWidget price_button;
	protected TextWidget price_text;
	protected ImageWidget price_icon;
	
	protected ButtonWidget player_name_button;
	protected TextWidget player_name_text;
	protected ImageWidget player_name_icon;

	protected bool m_NameSortState;
	protected bool m_TimeSortState;
	protected bool m_PriceSortState;
	protected bool m_OwnerSortState;

	void ExpansionP2PMarketMenuListHeader(ExpansionP2PMarketMenu menu)
	{
		Class.CastTo(m_P2PMarketMenuListHeaderController, GetController());
		m_P2PMarketMenu = menu;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_listings_header.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuListHeaderController;
	}

	void OnNameSortClick()
	{
		m_NameSortState = !m_NameSortState;

		if (m_NameSortState)
		{
			item_name_icon.SetFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_ClassNameZA();
		}
		else
		{
			item_name_icon.ClearFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_ClassNameAZ();
		}
	}

	void OnPriceSortClick()
	{
		m_PriceSortState = !m_PriceSortState;

		if (m_PriceSortState)
		{
			price_icon.SetFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_PriceHL();
		}
		else
		{
			price_icon.ClearFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_PriceLH();
		}
	}

	void OnTimeSortClick()
	{
		m_TimeSortState = !m_TimeSortState;

		if (m_TimeSortState)
		{
			time_icon.SetFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_TimeLS();
		}
		else
		{
			time_icon.ClearFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_TimeSL();
		}
	}
	
	void OnOwnerNameSortClick()
	{
		m_OwnerSortState = !m_OwnerSortState;

		if (m_OwnerSortState)
		{
			player_name_icon.SetFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_OwnerNameZA();
		}
		else
		{
			player_name_icon.ClearFlags(WidgetFlags.FLIPV);
			m_P2PMarketMenu.Listings_Filter_OwnerNameAZ();
		}
	}


	override void OnShow()
	{
		super.OnShow();

		/*item_name_icon.SetFlags(WidgetFlags.FLIPV);
		time_icon.SetFlags(WidgetFlags.FLIPV);
		price_icon.SetFlags(WidgetFlags.FLIPV);
		player_name_icon.SetFlags(WidgetFlags.FLIPV);*/
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == item_name_button)
			{
				item_name_button.SetColor(ARGB(255, 255, 255, 255));
				item_name_text.SetColor(ARGB(255, 0, 0, 0));
				item_name_icon.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == time_button)
			{
				time_button.SetColor(ARGB(255, 255, 255, 255));
				time_text.SetColor(ARGB(255, 0, 0, 0));
				time_icon.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == price_button)
			{
				price_button.SetColor(ARGB(255, 255, 255, 255));
				price_text.SetColor(ARGB(255, 0, 0, 0));
				price_icon.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == player_name_button)
			{
				player_name_button.SetColor(ARGB(255, 255, 255, 255));
				player_name_text.SetColor(ARGB(255, 0, 0, 0));
				player_name_icon.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == item_name_button)
			{
				item_name_button.SetColor(ARGB(255, 0, 0, 0));
				item_name_text.SetColor(ARGB(255, 255, 255, 255));
				item_name_icon.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == time_button)
			{
				time_button.SetColor(ARGB(255, 0, 0, 0));
				time_text.SetColor(ARGB(255, 255, 255, 255));
				time_icon.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == price_button)
			{
				price_button.SetColor(ARGB(255, 0, 0, 0));
				price_text.SetColor(ARGB(255, 255, 255, 255));
				price_icon.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == player_name_button)
			{
				player_name_button.SetColor(ARGB(255, 0, 0, 0));
				player_name_text.SetColor(ARGB(255, 255, 255, 255));
				player_name_icon.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
		}

		return false;
	}
};
class ExpansionP2PMarketMenuListHeaderController: ExpansionViewController {};