/**
 * ExpansionP2PMarketMenuCategoryElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuCategoryElement: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuCategoryElementController m_P2PMarketMenuCategoryController;
	protected ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected ExpansionP2PMarketMenuCategory m_Category;
	protected ExpansionP2PMarketCategoryListings m_CategoryData;
	protected bool m_ShowSubCategories;
	protected int m_SubCategoriesCount;
	protected bool m_Selected;

	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected WrapSpacerWidget sub_category_content;
	protected ImageWidget category_icon;
	protected TextWidget category_items_count;

	void ExpansionP2PMarketMenuCategoryElement(ExpansionP2PMarketMenu menu, ExpansionP2PMarketMenuCategory category)
	{
		Class.CastTo(m_P2PMarketMenuCategoryController, GetController());

		m_P2PMarketMenu = menu;
		m_Category = category;

		m_CategoryData = ExpansionP2PMarketModule.GetModuleInstance().GetCategoryListingsData(m_Category.GetCategoryIndex(), -1, m_P2PMarketMenu.GetTraderID(), m_P2PMarketMenu.IsGlobalTrader());
		
		SetView();
	}

	void SetView()
	{
		button_element.SetColor(ARGB(255, 0, 0, 0));
		m_P2PMarketMenuCategoryController.CategoryName = m_Category.GetDisplayName();
		m_P2PMarketMenuCategoryController.NotifyPropertyChanged("CategoryName");
		
		string iconPath = m_Category.GetIconPath();
		category_icon.LoadImageFile(0, iconPath);
		category_icon.SetImage(0);
		
		if (m_Category.GetSubCategories() && m_Category.GetSubCategories().Count() > 0)
		{
			int subCategoryIndex;
			array<ref ExpansionP2PMarketMenuSubCategory> subCategories = m_Category.GetSubCategories();
			
			for (int i = 0; i < subCategories.Count(); ++i)
			{
				ExpansionP2PMarketMenuSubCategory subCategory = subCategories[i];
				subCategoryIndex++;
				ExpansionP2PMarketMenuSubCategoryElement subCategoryElement = new ExpansionP2PMarketMenuSubCategoryElement(m_P2PMarketMenu, subCategory, this);
				m_P2PMarketMenuCategoryController.SubCategories.Insert(subCategoryElement);
				
				if (m_Category.GetSubCategories().Count() > 1 && subCategoryIndex < m_Category.GetSubCategories().Count())
					subCategoryElement.ShowTreeElement(true);
					
			}
		}
		
		string itemsCountText = "[0]";
		if (m_CategoryData && m_CategoryData.m_Count > 0)
		{
			itemsCountText = "[" + m_CategoryData.m_Count + "]";
		}

		m_P2PMarketMenuCategoryController.CategoryItemsCount = itemsCountText;
		m_P2PMarketMenuCategoryController.NotifyPropertyChanged("CategoryItemsCount");
	}
	
	void UpdateData()
	{
		m_CategoryData = ExpansionP2PMarketModule.GetModuleInstance().GetCategoryListingsData(m_Category.GetCategoryIndex(), -1, m_P2PMarketMenu.GetTraderID(), m_P2PMarketMenu.IsGlobalTrader());
		
		string itemsCountText = "[0]";
		if (m_CategoryData && m_CategoryData.m_Count > 0)
		{
			itemsCountText = "[" + m_CategoryData.m_Count + "]";
		}

		m_P2PMarketMenuCategoryController.CategoryItemsCount = itemsCountText;
		m_P2PMarketMenuCategoryController.NotifyPropertyChanged("CategoryItemsCount");

		for (int i = 0; i < m_P2PMarketMenuCategoryController.SubCategories.Count(); ++i)
		{
			ExpansionP2PMarketMenuSubCategoryElement subCategory = m_P2PMarketMenuCategoryController.SubCategories[i];
			subCategory.UpdateData();
		}
	}
	
	void SetSelected(bool state)
	{
		ErrorEx("State: " + state, ErrorExSeverity.INFO);
		m_Selected = state;
		
		if (state)
		{
			button_element.SetColor(ARGB(40, 255, 255, 255));
			button_element_text.SetColor(ARGB(255, 0, 0, 0));
			category_icon.SetColor(ARGB(255, 0, 0, 0));
			category_items_count.SetColor(ARGB(255, 0, 0, 0));
		}
		else
		{
			button_element.SetColor(ARGB(255, 0, 0, 0));
			button_element_text.SetColor(ARGB(255, 255, 255, 255));
			category_icon.SetColor(ARGB(255, 255, 255, 255));
			category_items_count.SetColor(ARGB(255, 255, 255, 255));
		}
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
	}

	void OnElementButtonClick()
	{
		if (m_P2PMarketMenuCategoryController.SubCategories.Count() > 0)
		{
			if (!m_ShowSubCategories)
			{
				sub_category_content.Show(true);
			}
			else
			{
				sub_category_content.Show(false);
			}
			
			m_ShowSubCategories = !m_ShowSubCategories;
		}
		
		m_P2PMarketMenu.UpdateMenuCategory(m_CategoryData);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == button_element)
			{
				button_element.SetColor(ARGB(40, 255, 255, 255));
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
			if (w == button_element && !m_Selected)
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
	
	ExpansionP2PMarketCategoryListings GetCategoryData()
	{
		return m_CategoryData;
	}
	
	ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement> GetSubCategories()
	{
		return m_P2PMarketMenuCategoryController.SubCategories;
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
	protected ExpansionP2PMarketMenuSubCategory m_SubCategory;
	protected ref ExpansionP2PMarketCategoryListings m_SubCategoryData;
	protected ExpansionP2PMarketMenuCategoryElement m_ParentCategory;
	
	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected Widget tree_panel_3;
	protected ImageWidget category_icon;
	protected TextWidget category_items_count;
	
	protected bool m_Selected;
	
	void ExpansionP2PMarketMenuSubCategoryElement(ExpansionP2PMarketMenu menu, ExpansionP2PMarketMenuSubCategory subCategory, ExpansionP2PMarketMenuCategoryElement parentCategory)
	{
		Class.CastTo(m_P2PMarketMenuSubCategoryController, GetController());

		m_P2PMarketMenu = menu;
		m_SubCategory = subCategory;
		m_ParentCategory = parentCategory;
		
		int traderID = -1;
		if (!m_P2PMarketMenu.IsGlobalTrader() && m_P2PMarketMenu.GetTraderID() > -1)
		{
			traderID = m_P2PMarketMenu.GetTraderID();
		}
		
		m_SubCategoryData = ExpansionP2PMarketModule.GetModuleInstance().GetCategoryListingsData(m_SubCategory.GetCategoryIndex(), m_SubCategory.GetSubCategoryIndex(), m_P2PMarketMenu.GetTraderID(), m_P2PMarketMenu.IsGlobalTrader());
		
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
		if (m_SubCategoryData && m_SubCategoryData.m_Count > 0)
		{
			itemsCountText = "[" + m_SubCategoryData.m_Count + "]";
		}
		
		m_P2PMarketMenuSubCategoryController.SubCategoryItemsCount = itemsCountText;
		m_P2PMarketMenuSubCategoryController.NotifyPropertyChanged("SubCategoryItemsCount");
	}
	
	void ShowTreeElement(bool state)
	{
		tree_panel_3.Show(state);
	}
	
	void SetSelected(bool state)
	{
		ErrorEx("State: " + state, ErrorExSeverity.INFO);
		m_Selected = state;
		
		if (state)
		{
			button_element.SetColor(ARGB(40, 255, 255, 255));
			button_element_text.SetColor(ARGB(255, 0, 0, 0));
			category_icon.SetColor(ARGB(255, 0, 0, 0));
			category_items_count.SetColor(ARGB(255, 0, 0, 0));
		}
		else
		{
			button_element.SetColor(ARGB(255, 0, 0, 0));
			button_element_text.SetColor(ARGB(255, 255, 255, 255));
			category_icon.SetColor(ARGB(255, 255, 255, 255));
			category_items_count.SetColor(ARGB(255, 255, 255, 255));
		}
		
		if (state)
			m_ParentCategory.SetSelected(true);
	}
	
	void UpdateData()
	{
		m_SubCategoryData = ExpansionP2PMarketModule.GetModuleInstance().GetCategoryListingsData(m_SubCategory.GetCategoryIndex(), m_SubCategory.GetSubCategoryIndex(), m_P2PMarketMenu.GetTraderID(), m_P2PMarketMenu.IsGlobalTrader());
		
		string itemsCountText = "[0]";
		if (m_SubCategoryData && m_SubCategoryData.m_Count > 0)
		{
			itemsCountText = "[" + m_SubCategoryData.m_Count + "]";
		}

		m_P2PMarketMenuSubCategoryController.SubCategoryItemsCount = itemsCountText;
		m_P2PMarketMenuSubCategoryController.NotifyPropertyChanged("SubCategoryItemsCount");
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
		m_P2PMarketMenu.UpdateMenuCategory(m_SubCategoryData);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == button_element)
			{
				button_element.SetColor(ARGB(40, 255, 255, 255));
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
			if (w == button_element && !m_Selected)
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
	
	ExpansionP2PMarketCategoryListings GetCategoryData()
	{
		return m_SubCategoryData;
	}
}

class ExpansionP2PMarketMenuSubCategoryElementController: ExpansionViewController
{
	string SubCategoryName;
	string SubCategoryItemsCount;
};
