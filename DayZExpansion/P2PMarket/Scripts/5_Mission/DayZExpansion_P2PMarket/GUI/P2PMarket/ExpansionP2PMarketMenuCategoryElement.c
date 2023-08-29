/**
 * ExpansionP2PMarketMenuCategoryElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuCategoryElement: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuCategoryElementController m_P2PMarketMenuCategoryController;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected ref ExpansionP2PMarketMenuCategory m_Category;
	protected bool m_ShowSubCategories;
	protected int m_SubCategoriesCount;
	
	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected WrapSpacerWidget sub_category_content;
	protected ImageWidget element_icon;
	protected ImageWidget category_icon;
	protected TextWidget category_items_count;

	void ExpansionP2PMarketMenuCategoryElement(ExpansionP2PMarketMenu menu, ExpansionP2PMarketMenuCategory category)
	{
		Class.CastTo(m_P2PMarketMenuCategoryController, GetController());

		m_P2PMarketMenu = menu;
		m_Category = category;

		SetView();
	}

	void SetView()
	{
		m_P2PMarketMenuCategoryController.CategoryName = m_Category.GetDisplayName();
		m_P2PMarketMenuCategoryController.NotifyPropertyChanged("CategoryName");
		
		string iconPath = m_Category.GetIconPath();
		category_icon.LoadImageFile(0, iconPath);
		category_icon.SetImage(0);
		
		if (m_Category.GetSubCategories() && m_Category.GetSubCategories().Count() > 0)
		{
			element_icon.Show(true);
			
			int subCategoryIndex;
			array<ref ExpansionP2PMarketMenuSubCategory> subCategories = m_Category.GetSubCategories();

			foreach (ExpansionP2PMarketMenuSubCategory subCategory: subCategories)
			{
				subCategoryIndex++;
				ExpansionP2PMarketMenuSubCategoryElement subCategoryElement = new ExpansionP2PMarketMenuSubCategoryElement(m_P2PMarketMenu, subCategory);
				m_P2PMarketMenuCategoryController.SubCategories.Insert(subCategoryElement);
				
				if (m_Category.GetSubCategories().Count() > 1 && subCategoryIndex < m_Category.GetSubCategories().Count())
					subCategoryElement.ShowTreeElement(true);
					
			}
		}
		
		string itemsCountText = "[0]";
		int itemsCount = m_P2PMarketMenu.GetCategoryListingsCount(m_Category);
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";

		m_P2PMarketMenuCategoryController.CategoryItemsCount = itemsCountText;
		m_P2PMarketMenuCategoryController.NotifyPropertyChanged("CategoryItemsCount");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_category.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuCategoryElementController;
	}
	
	void ShowSubCategories(bool state)
	{
		m_ShowSubCategories = state;
		sub_category_content.Show(state);
		
		if (state)
		{
			element_icon.SetFlags(WidgetFlags.FLIPV, true);
		}
		else
		{
			element_icon.ClearFlags(WidgetFlags.FLIPV, true);
		}
	}

	void OnElementButtonClick()
	{
		if (!m_Category)
			return;
		
		if (m_P2PMarketMenuCategoryController.SubCategories.Count() > 0)
		{
			if (!m_ShowSubCategories)
			{
				element_icon.SetFlags(WidgetFlags.FLIPV, true);
				sub_category_content.Show(true);
			}
			else
			{
				element_icon.ClearFlags(WidgetFlags.FLIPV, true);
				sub_category_content.Show(false);
			}
			
			m_ShowSubCategories = !m_ShowSubCategories;
		}
		
		m_P2PMarketMenu.UpdateMenuCategory(m_Category);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == button_element)
			{
				button_element.SetColor(ARGB(255, 255, 255, 255));
				button_element_text.SetColor(ARGB(255, 0, 0, 0));
				element_icon.SetColor(ARGB(255, 0, 0, 0));
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
				element_icon.SetColor(ARGB(255, 255, 255, 255));
				category_icon.SetColor(ARGB(255, 255, 255, 255));
				category_items_count.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
		}

		return false;
	}
};

class ExpansionP2PMarketMenuCategoryElementController: ExpansionViewController
{
	string CategoryName;
	string CategoryItemsCount;
	ref ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement> SubCategories = new ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement>(this);
};

class ExpansionP2PMarketMenuSubCategoryElement: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuSubCategoryElementController m_P2PMarketMenuSubCategoryController;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected ref ExpansionP2PMarketMenuSubCategory m_SubCategory;
	
	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected Widget tree_panel_3;
	protected ImageWidget category_icon;
	
	void ExpansionP2PMarketMenuSubCategoryElement(ExpansionP2PMarketMenu menu, ExpansionP2PMarketMenuSubCategory subCategory)
	{
		Class.CastTo(m_P2PMarketMenuSubCategoryController, GetController());

		m_P2PMarketMenu = menu;
		m_SubCategory = subCategory;
		
		SetView();
	}
	
	void SetView()
	{
		m_P2PMarketMenuSubCategoryController.SubCategoryName = m_SubCategory.GetDisplayName();
		m_P2PMarketMenuSubCategoryController.NotifyPropertyChanged("SubCategoryName");
		
		string iconPath = m_SubCategory.GetIconPath();
		category_icon.LoadImageFile(0, iconPath);
		category_icon.SetImage(0);
		
		string itemsCountText = "[0]";
		int itemsCount = m_P2PMarketMenu.GetCategoryListingsCount(m_SubCategory);
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";
		
		m_P2PMarketMenuSubCategoryController.SubCategoryItemsCount = itemsCountText;
		m_P2PMarketMenuSubCategoryController.NotifyPropertyChanged("SubCategoryItemsCount");
	}
	
	void ShowTreeElement(bool state)
	{
		tree_panel_3.Show(state);
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_subcategory.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuSubCategoryElementController;
	}
	
	void OnElementButtonClick()
	{
		m_P2PMarketMenu.UpdateMenuCategory(m_SubCategory);
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
				return true;
			}
		}

		return false;
	}
}

class ExpansionP2PMarketMenuSubCategoryElementController: ExpansionViewController
{
	string SubCategoryName;
	string SubCategoryItemsCount;
};
