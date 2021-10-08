/**
 * ExpansionMarketMenuCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuCategoryColorHandler
{
	protected Widget m_LayooutRoot;
	protected Widget category_header_corners;
	protected Widget category_header_background;
	protected TextWidget category_title;

	void ExpansionMarketMenuCategoryColorHandler(Widget layoutRoot)
	{
		m_LayooutRoot = layoutRoot;
		
		SetColors();
	}
	
	void SetColors()
	{
		//! Category element corners
		category_header_corners = Widget.Cast(m_LayooutRoot.FindAnyWidget("category_header_corners"));
		category_header_corners.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorCategoryCorners));
		
		//! Category element background
		category_header_background = Widget.Cast(m_LayooutRoot.FindAnyWidget("category_header_background"));
		category_header_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorCategoryBackground));
		
		//! Category element text
		category_title = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("category_title"));
		category_title.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
	}
}

class ExpansionMarketMenuCategory: ExpansionScriptView
{
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketCategory m_Category;
	ref ExpansionMarketMenuCategoryController m_CategoryController;
	ref map<string, ref array<ExpansionMarketItem>> m_TempItems;
	ref array<string> m_TempDisplayNames;
	int m_ItemUpdateIdx;
	ref array<ExpansionMarketItem> m_MarketItems;
	ref TIntArray m_ItemIDs;
	
	protected GridSpacerWidget category_items;
	protected  ButtonWidget category_button;
	protected ImageWidget category_expand_icon;
	protected ImageWidget category_collapse_icon;
		
	bool IsExpanded = false;
	static int UpdateCategoryID = -1;
	ref ExpansionMarketMenuCategoryColorHandler m_ColorHandler;
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory Constructor
	// ------------------------------------------------------------	
	void ExpansionMarketMenuCategory(ExpansionMarketMenu menu, ExpansionMarketCategory category, ref map<string, ref array<ExpansionMarketItem>> tempItems)
	{
		m_MarketMenu = menu;
		m_Category = category;

		if (!m_CategoryController)
			m_CategoryController = ExpansionMarketMenuCategoryController.Cast(GetController());
		
		m_CategoryController.CategoryName = m_Category.DisplayName;
		m_CategoryController.NotifyPropertyChanged("CategoryName");

		m_TempItems = tempItems;
		m_MarketItems = new array<ExpansionMarketItem>;
		m_ItemIDs = new TIntArray;

		UpdateMarketItems();

		if (!m_ColorHandler)
			m_ColorHandler = new ExpansionMarketMenuCategoryColorHandler(GetLayoutRoot());
	}

	void UpdateMarketItems()
	{
		//! Sort by display name
		m_TempDisplayNames = m_TempItems.GetKeyArray();
		m_TempDisplayNames.Sort();

		int idx;
		foreach (string tempDisplayName: m_TempDisplayNames)
		{
			foreach (ExpansionMarketItem tempItem: m_TempItems[tempDisplayName])
			{
				tempItem.m_Idx = idx++;
				if (m_MarketItems.Find(tempItem) == -1)
					m_MarketItems.Insert(tempItem);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory Destructor
	// ------------------------------------------------------------		
	void ~ExpansionMarketMenuCategory()
	{
		ClearCategoryItems();
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory GetLayoutFile
	// ------------------------------------------------------------	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_category_element.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory GetControllerType
	// ------------------------------------------------------------	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuCategoryController;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory AddItem
	// ------------------------------------------------------------			
	void AddItem(ExpansionMarketItem item)
	{
		ExpansionMarketMenuItem itemElement = new ExpansionMarketMenuItem(m_MarketMenu, item);

		int count = m_CategoryController.MarketItems.Count();

		if (!count || item.m_Idx > m_CategoryController.MarketItems[count - 1].GetBaseItem().m_Idx)
		{
			m_CategoryController.MarketItems.Insert(itemElement);
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				if (item.m_Idx < m_CategoryController.MarketItems[i].GetBaseItem().m_Idx)
					break;
			}

			m_CategoryController.MarketItems.InsertAt(itemElement, i);
		}

		m_ItemIDs.Insert(item.ItemID);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory GetItems
	// ------------------------------------------------------------		
	ObservableCollection<ref ExpansionMarketMenuItem> GetItems()
	{
		return m_CategoryController.MarketItems;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory ClearCategoryItems
	// ------------------------------------------------------------		
	void ClearCategoryItems()
	{
		//! Need to delete market menu item elements so preview objects get cleared up
		//! TODO: Find out why this is necessary and clearing items is not enough (some other refs anywhere?)
		for (int i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		{
			ExpansionMarketMenuItem itemElement = m_CategoryController.MarketItems[i];
			itemElement.Hide();
			delete itemElement;
		}

		m_CategoryController.MarketItems.Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory GetUpdateTickRate
	// ------------------------------------------------------------		
	override float GetUpdateTickRate()
	{
		return 0.016666;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory Update
	// ------------------------------------------------------------		
	override void Update()
	{
		//! Prevent framerate drop due to too many categories updating at the same time
		if (UpdateCategoryID == -1 || (IsExpanded && IsVisible()))
			UpdateCategoryID = m_Category.CategoryID;
		else if (UpdateCategoryID != m_Category.CategoryID)
			return;

		if (m_ItemUpdateIdx < m_TempItems.Count())
		{
			//! We may still have items to add
			bool isFiltered = m_MarketMenu.IsFiltered();
			string tempDisplayName = m_TempDisplayNames[m_ItemUpdateIdx++];
			foreach (ExpansionMarketItem tempItem: m_TempItems[tempDisplayName])
			{
				if (m_ItemIDs.Find(tempItem.ItemID) > -1)
					continue;  //! Already added
				if (tempItem.m_AttachmentIDs)
					continue;  //! Not yet finalized
				if (tempItem.m_ShowInMenu)
				{
					AddItem(tempItem);
				}
				else if (isFiltered)
				{
					UpdateCategoryID = -1;
				}
			}
		}
		else
		{
			//! All items added?
			if (GetItems().Count() == m_MarketItems.Count())
			{
				m_UpdateTimer.Stop();
				m_TempItems.Clear();
			}
			else
			{
				m_ItemUpdateIdx = 0;
			}

			if (UpdateCategoryID == m_Category.CategoryID)
				UpdateCategoryID = -1;
		}
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory OnCategoryButtonClick
	// ------------------------------------------------------------	
	void OnCategoryButtonClick()
	{
		ToggleCategory(!IsExpanded, true);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory ToggleCategory
	// ------------------------------------------------------------	
	void ToggleCategory(bool toggleState, bool forceUpdate = false)
	{
		if (toggleState)
		{
			//! Force update cycle to prioritize this category
			//! Should only be used when toggling single category
			if (forceUpdate && m_UpdateTimer.IsRunning())
				UpdateCategoryID = m_Category.CategoryID;

			category_collapse_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorCategoryCollapseIcon));
		}
		else
		{
			category_expand_icon.SetColor(ARGB(140, 251, 252, 254));
		}

		category_items.Show(toggleState);
		category_expand_icon.Show(!toggleState);
		category_collapse_icon.Show(toggleState);

		if (!IsExpanded && toggleState)
			m_MarketMenu.CategoriesExpanded++;
		else if (IsExpanded && !toggleState)
			m_MarketMenu.CategoriesExpanded--;

		IsExpanded = toggleState;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case category_button:
			category_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorCategoryButton));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case category_button:
			category_button.SetColor(ARGB(255, 30, 30, 30));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuCategory AllowButtonInputs
	// ------------------------------------------------------------	
	void AllowButtonInputs(bool state)
	{
		if (!state)
		{
			GetLayoutRoot().SetFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			category_button.SetFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
		}
		else
		{
			GetLayoutRoot().ClearFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			category_button.ClearFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
		}
	}
};

class ExpansionMarketMenuCategoryController: ExpansionViewController
{
	string CategoryName;
	ref ObservableCollection<ref ExpansionMarketMenuItem> MarketItems = new ObservableCollection<ref ExpansionMarketMenuItem>(this);
	bool CategoryExpand;
	
	ref ExpansionMarketMenuCategory m_Category;

	// ------------------------------------------------------------
	// ExpansionMarketMenuCategoryController Constructor
	// ------------------------------------------------------------	
	void ExpansionMarketMenuCategoryController()
	{
		if (!m_Category)
			m_Category = ExpansionMarketMenuCategory.Cast(GetParent());
	}
};