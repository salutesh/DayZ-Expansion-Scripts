/**
 * ExpansionP2PMarketMenuInventoryCategoryElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuInventoryCategoryElement: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuInventoryCategoryElementController m_P2PMarketMenuInventoryCategoryController;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected string m_SlotName;

	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected ImageWidget category_icon;
	protected TextWidget category_items_count;

	void ExpansionP2PMarketMenuInventoryCategoryElement(ExpansionP2PMarketMenu menu, string slotName)
	{
		Class.CastTo(m_P2PMarketMenuInventoryCategoryController, GetController());

		m_P2PMarketMenu = menu;
		m_SlotName = slotName;

		SetView();
	}

	void SetView()
	{
		m_P2PMarketMenuInventoryCategoryController.SlotName = m_SlotName;
		m_P2PMarketMenuInventoryCategoryController.NotifyPropertyChanged("SlotName");
		
		string slotnameLower = m_SlotName;
		slotnameLower.ToLower();
		
		string imagePath;
		if (slotnameLower == "shoulder")
			slotnameLower = "shoulderright";
		else if (slotnameLower == "melee")
			slotnameLower = "shoulderleft";
		
		category_icon.LoadImageFile(0, "set:dayz_inventory image:" + slotnameLower);
		category_icon.SetImage(0);
		
		string itemsCountText = "[0]";
		int itemsCount = m_P2PMarketMenu.GetInventoryCategoryItemsCount(m_SlotName);
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";

		m_P2PMarketMenuInventoryCategoryController.CategoryItemsCount = itemsCountText;
		m_P2PMarketMenuInventoryCategoryController.NotifyPropertyChanged("CategoryItemsCount");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_inventory_category.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuInventoryCategoryElementController;
	}

	void OnItemButtonClick()
	{
		m_P2PMarketMenu.UpdateInventorySlotFilter(m_SlotName);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == button_element)
			{
				button_element.SetColor(ARGB(255, 255, 255, 255));
				button_element_text.SetColor(ARGB(255, 0, 0, 0));
				category_icon.SetColor(ARGB(255, 0, 0, 0));
				category_items_count.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == button_element)
			{
				button_element.SetColor(ARGB(255, 0, 0, 0));
				button_element_text.SetColor(ARGB(255, 255, 255, 255));
				category_icon.SetColor(ARGB(255, 255, 255, 255));
				category_items_count.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
		}

		return false;
	}
};

class ExpansionP2PMarketMenuInventoryCategoryElementController: ExpansionViewController
{
	string SlotName;
	string CategoryItemsCount;
};