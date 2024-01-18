/**
 * ExpansionMarketMenuCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	protected ImageWidget category_icon;
	protected TextWidget category_title;
	protected TextWidget category_info;

	void ExpansionMarketMenuCategoryColorHandler(Widget layoutRoot, string title_color)
	{
		m_LayooutRoot = layoutRoot;
		
		SetColors(title_color);
	}
	
	void SetColors(string title_color)
	{
		//! Category element corners
		category_header_corners = Widget.Cast(m_LayooutRoot.FindAnyWidget("category_header_corners"));
		category_header_corners.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorCategoryCorners"));
		
		//! Category element background
		category_header_background = Widget.Cast(m_LayooutRoot.FindAnyWidget("category_header_background"));
		category_header_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorCategoryBackground"));
		
		//! Category icon
		category_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("category_icon"));
		category_icon.SetColor(ExpansionColor.HexToARGB(title_color));
		
		//! Category element text
		category_title = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("category_title"));
		category_title.SetColor(ExpansionColor.HexToARGB(title_color));

		category_info = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("category_info"));
		category_info.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
	}
}

class ExpansionMarketMenuCategory: ExpansionScriptView
{
	protected ref ExpansionMarketMenu m_MarketMenu;
	protected ref ExpansionMarketCategory m_Category;
	protected ref ExpansionMarketMenuCategoryController m_CategoryController;
	protected ref map<string, ref array<ExpansionMarketItem>> m_TempItems;
	protected ref array<string> m_TempDisplayNames = {};
	protected int m_ItemUpdateIdx;
	protected ref array<ExpansionMarketItem> m_MarketItems;
	protected ref TIntArray m_ItemIDs;
	protected bool m_UpdateItemCount;
	protected float m_UpdateItemCountTime;
	protected string m_CategoryInfo_Loading;
	protected int m_CategoryInfo_Loading_Idx;
	protected string m_CategoryInfo_ItemCount;
	protected bool m_Sorted;
	
	protected GridSpacerWidget category_items;
	protected  ButtonWidget category_button;
	protected ImageWidget category_expand_icon;
	protected ImageWidget category_collapse_icon;
	protected ImageWidget category_icon;
		
	protected bool m_IsExpanded = false;
	static int UpdateCategoryID = -1;
	static int ForceUpdateCategoryID = -1;
	protected ref ExpansionMarketMenuCategoryColorHandler m_ColorHandler;
	
	void ExpansionMarketMenuCategory(ExpansionMarketMenu menu, ExpansionMarketCategory category, ref map<string, ref array<ExpansionMarketItem>> tempItems)
	{
		m_MarketMenu = menu;
		m_Category = category;

		m_CategoryController = ExpansionMarketMenuCategoryController.Cast(GetController());
		m_CategoryController.CategoryName = m_Category.DisplayName;
		
		if (ExpansionIcons.GetPath(m_Category.Icon) != string.Empty)
		{
			m_CategoryController.CategoryIcon = ExpansionIcons.GetPath(m_Category.Icon);
		}
		else
		{
			m_CategoryController.CategoryIcon = m_Category.Icon;
		}
		
		m_CategoryController.NotifyPropertiesChanged({"CategoryName", "CategoryIcon"});

		m_TempItems = tempItems;
		m_MarketItems = new array<ExpansionMarketItem>;
		m_ItemIDs = new TIntArray;

		UpdateMarketItems();

		if (!m_ColorHandler)
			m_ColorHandler = new ExpansionMarketMenuCategoryColorHandler(GetLayoutRoot(), m_Category.Color);
	}

	void UpdateMarketItems()
	{
		m_TempDisplayNames.Clear();

		int idx;
		int show;
		foreach (string tempDisplayName, array<ExpansionMarketItem> tempItems: m_TempItems)
		{
			m_TempDisplayNames.Insert(tempDisplayName);
			foreach (ExpansionMarketItem tempItem: tempItems)
			{
				if (m_MarketItems.Find(tempItem) == -1)
					m_MarketItems.Insert(tempItem);
				if (tempItem.m_ShowInMenu && m_Category.m_Finalized)
					show++;
			}
		}

		if (!IsUpdateTimerRunning() || !m_UpdateItemCount)
			UpdateItemCount(show);
	}

	void ~ExpansionMarketMenuCategory()
	{
		ClearCategoryItems();
	}

	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_category_element.layout";
	}

	override typename GetControllerType() 
	{
		return ExpansionMarketMenuCategoryController;
	}
		
	void AddItem(ExpansionMarketItem item)
	{
		ExpansionMarketMenuItem itemElement = new ExpansionMarketMenuItem(m_MarketMenu, item);

		int sortIdx = GetSortIndex(itemElement);
		m_CategoryController.MarketItems.InsertAt(itemElement, sortIdx);

		if (m_Sorted)
		{
			//! Item sorting has been manually changed while loading, need to update sort order
			for (int i = sortIdx; i < m_CategoryController.MarketItems.Count(); i++)
			{
				m_CategoryController.MarketItems[i].SetSort(i);
			}
		}

		m_ItemIDs.Insert(item.ItemID);
	}
	
	void QuickSortItems(int low, int high)
	{
		if (low < high)
		{
			int partitionIndex = Partition(low, high);
			QuickSortItems(low, partitionIndex - 1);
			QuickSortItems(partitionIndex + 1, high);
		}
	}

	int Partition(int low, int high)
	{
		ExpansionMarketMenuItem pivot = m_CategoryController.MarketItems[high];
		int i = low - 1;

		for (int j = low; j < high; j++)
		{
			if (Cmp(m_CategoryController.MarketItems[j], pivot) < 0)
			{
				i++;
				m_CategoryController.MarketItems.SwapItems(i, j);
			}
		}

		m_CategoryController.MarketItems.SwapItems(i + 1, high);

		return i + 1;
	}

	void SortItems()
	{
		auto trace = EXTrace.Start(EXTrace.MARKET, this);

		int i;
		ExpansionMarketMenuItem menuItem;

		//! QuickSort in EnfScript: ~60 ms for 100 items
	/*
		int high = m_CategoryController.MarketItems.Count() - 1;
		
		QuickSortItems(0, high);
	*/

		//! All the below combined: ~3 ms for 100 items

		TStringArray sortKeys = {};

		ExpansionMarketMenuSortPriority sortPriority = m_MarketMenu.GetSortPriority();

		map<string, ref ExpansionMarketMenuItem> menuItems = new map<string, ref ExpansionMarketMenuItem>;

		for (i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		{
			menuItem = m_CategoryController.MarketItems[i];
			menuItem.SetMenuIdx(i);
			//! @note we append the index so items with same sort key (= same displayname and price) don't overwrite each other
			string sortKey = menuItem.GetSortKey(sortPriority) + "\t" + i;
			sortKeys.Insert(sortKey);
			menuItems[sortKey] = menuItem;
		}

		bool reverse;

		switch (sortPriority)
		{
			case ExpansionMarketMenuSortPriority.NAME:
				reverse = m_MarketMenu.GetNameSortState();
				break;

			case ExpansionMarketMenuSortPriority.PRICE:
				reverse = m_MarketMenu.GetPriceSortState();
				break;
		}

		sortKeys.Sort(reverse);

		foreach (int index, string key: sortKeys)
		{
			menuItem = menuItems[key];
			i = menuItem.GetMenuIdx();
			//i = m_CategoryController.MarketItems.Find(menuItem);
			if (i != index)
			{
				m_CategoryController.MarketItems[index].SetMenuIdx(i);
				//! @note SpacerBaseWidgetController::Swap in current stable DF is broken (does not always swap the widgets correctly),
				//! so we run our own swapping implementation
				//m_CategoryController.MarketItems.SwapItems(index, i);
				SwapMenuItems(index, i);
				menuItem.SetMenuIdx(index);
			}
			menuItem.SetSort(index, false);
		#ifdef DIAG
			PrintFormat("SortItems %1->%2 %3 %4", i, index, menuItem.GetItemSortName(), menuItem.GetBuyPrice());
		#endif
		}

		m_Sorted = true;

	//#ifdef DIAG
		//for (i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		//{
			//menuItem = m_CategoryController.MarketItems[i];
			//PrintFormat("SortItems %1 %2 %3 %4", i, menuItem.GetSort(), menuItem.GetItemSortName(), menuItem.GetBuyPrice());
		//}
	//#endif
	}

	//! @brief Swap menu items at index A and B
	void SwapMenuItems(int indexA, int indexB)
	{
		if (indexA == indexB)
			return;

		m_CategoryController.MarketItems.GetArray().SwapItems(indexA, indexB);

		if (indexA > indexB)
		{
			int temp = indexA;
			indexA = indexB;
			indexB = temp;
		}

		Widget widgetA = m_CategoryController.MarketItems[indexA].GetLayoutRoot();
		Widget widgetB = m_CategoryController.MarketItems[indexB].GetLayoutRoot();
		category_items.RemoveChild(widgetB);
		category_items.AddChildAfter(widgetB, widgetA);
		category_items.RemoveChild(widgetA);
		Widget widgetC = m_CategoryController.MarketItems[indexB - 1].GetLayoutRoot();
		category_items.AddChildAfter(widgetA, widgetC);
	}

	int GetSortIndex(ExpansionMarketMenuItem newMenuItem)
	{
		for (int i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		{
			ExpansionMarketMenuItem menuItem = m_CategoryController.MarketItems[i];
			if (Cmp(newMenuItem, menuItem) < 0)
				break;
		}

		return i;
	}

	int Cmp(ExpansionMarketMenuItem a, ExpansionMarketMenuItem b)
	{
		string nameA = a.GetItemSortName();
		string nameB = b.GetItemSortName();

		int priceA = a.GetBuyPrice();
		int priceB = b.GetBuyPrice();

		switch (m_MarketMenu.GetSortPriority())
		{
			case ExpansionMarketMenuSortPriority.NAME:
				if (nameA == nameB)
					return CmpPrice(priceA, priceB);
				else
					return CmpName(nameA, nameB);

				break;

			case ExpansionMarketMenuSortPriority.PRICE:
				if (priceA == priceB)
					return CmpName(nameA, nameB);
				else
					return CmpPrice(priceA, priceB);

				break;
		}

		return 0;
	}

	int CmpName(string nameA, string nameB)
	{
		if (m_MarketMenu.GetNameSortState())
			return ExpansionString.StrCmp(nameB, nameA);  //! Descending
		else
			return ExpansionString.StrCmp(nameA, nameB);  //! Ascending
	}

	int CmpPrice(int priceA, int priceB)
	{
		if (m_MarketMenu.GetPriceSortState())
		{
			//! Descending
			if (priceA < priceB)
				return 1;
			else if (priceA > priceB)
				return -1;
		}
		else
		{
			//! Ascending
			if (priceA < priceB)
				return -1;
			else if (priceA > priceB)
				return 1;
		}

		return 0;
	}

	ObservableCollection<ref ExpansionMarketMenuItem> GetItems()
	{
		return m_CategoryController.MarketItems;
	}

	int GetShowItemsCount()
	{
		int show;
		foreach (ExpansionMarketItem item: m_MarketItems)
		{
			if (item.m_ShowInMenu)
				show++;
		}
		return show;
	}

	int GetShownItemsCount()
	{
		int shown;
		for (int i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		{
			if (m_CategoryController.MarketItems[i].IsVisible())
				shown++;
		}
		return shown;
	}

	void ClearCategoryItems()
	{
		for (int i = 0; i < m_CategoryController.MarketItems.Count(); i++)
		{
			ExpansionMarketMenuItem itemElement = m_CategoryController.MarketItems[i];
			itemElement.Destroy();
		}

		m_CategoryController.MarketItems.Clear();
	}

	override float GetUpdateTickRate()
	{
		return 0.016666;
	}

	override void Expansion_Update()
	{
		//! Prevent framerate drop due to too many categories updating at the same time
		if (UpdateCategoryID == -1 || (m_IsExpanded && IsVisible()))
			UpdateCategoryID = m_Category.CategoryID;
		else if (UpdateCategoryID != m_Category.CategoryID)
		{
			if (m_UpdateItemCount)
				UpdateItemCount(-1);
			return;
		}

		bool addItems;
		if (m_ItemUpdateIdx < m_TempItems.Count())
			addItems = true;

		if (addItems)
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
				else if (isFiltered && UpdateCategoryID != ForceUpdateCategoryID)
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
			{
				UpdateCategoryID = -1;
				ForceUpdateCategoryID = -1;
			}
		}

		if (m_UpdateItemCount)
		{
			if (!addItems)
			{
				int show = GetShowItemsCount();
				if (show > 0 && GetShownItemsCount() == show)
				{
					m_UpdateItemCount = false;
					UpdateItemCount(show);
					return;
				}
			}

			UpdateItemCount(-1);
		}
	}

	void UpdateItemCount(int itemCount)
	{
		if (itemCount < 0)
		{
			//! Show a little loading animation using middots (U+00B7) and a bullet point (U+2022)
			m_UpdateItemCountTime += GetUpdateTickRate();
			if (m_UpdateItemCountTime > 0.125)
			{
				m_UpdateItemCountTime = 0;
				switch (m_CategoryInfo_Loading_Idx)
				{
					case 0:
						m_CategoryInfo_Loading = "•··";
						break;
					case 1:
					case 3:
						m_CategoryInfo_Loading = "·•·";
						break;
					case 2:
						m_CategoryInfo_Loading = "··•";
						break;
				}
				if (m_CategoryInfo_Loading_Idx < 3)
					m_CategoryInfo_Loading_Idx++;
				else
					m_CategoryInfo_Loading_Idx = 0;
			}
		}
		else if (itemCount > 0)
		{
			m_CategoryInfo_Loading = "";
			m_CategoryInfo_ItemCount = itemCount.ToString();
		}
		else
		{
			m_CategoryInfo_Loading = "";
			m_CategoryInfo_ItemCount = "···";
		}

		m_CategoryController.CategoryInfo = m_CategoryInfo_Loading + " " + m_CategoryInfo_ItemCount + " ITEMS";
		m_CategoryController.NotifyPropertyChanged("CategoryInfo");
	}

	bool IsUpdateTimerRunning()
	{
		return m_UpdateTimer.IsRunning();
	}

	void OnCategoryButtonClick()
	{
		ToggleCategory(!m_IsExpanded, true);
	}

	void ToggleCategory(bool toggleState, bool forceUpdate = false)
	{
		if (toggleState)
		{
			//! Force update cycle to prioritize this category
			//! Should only be used when toggling single category
			if (forceUpdate && m_UpdateTimer.IsRunning())
			{
				UpdateCategoryID = m_Category.CategoryID;
				ForceUpdateCategoryID = UpdateCategoryID;
			}

			if (GetShownItemsCount() < GetShowItemsCount())
				m_UpdateItemCount = true;

			category_collapse_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorCategoryCollapseIcon"));
		}
		else
		{
			category_expand_icon.SetColor(ARGB(140, 251, 252, 254));
		}

		category_items.Show(toggleState);
		category_expand_icon.Show(!toggleState);
		category_collapse_icon.Show(toggleState);

		if (!m_IsExpanded && toggleState)
			m_MarketMenu.CategoriesExpanded++;
		else if (m_IsExpanded && !toggleState)
			m_MarketMenu.CategoriesExpanded--;

		m_IsExpanded = toggleState;

		if (m_IsExpanded)
			SortItems();
	}

	bool IsExpanded()
	{
		return m_IsExpanded;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case category_button:
			category_button.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorCategoryButton"));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}

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
	
	ExpansionMarketCategory GetCategory()
	{
		return m_Category;
	}
	
	map<string, ref array<ExpansionMarketItem>> GetTempItems()
	{
		return m_TempItems;
	}
	
	array<ExpansionMarketItem> GetMarketItems()
	{
		return m_MarketItems;
	}
	
	ExpansionMarketMenuCategoryController GetCategoryController()
	{
		return m_CategoryController;
	}
	
	bool GetUpdateItemCount()
	{
		return m_UpdateItemCount;
	}
};

class ExpansionMarketMenuCategoryController: ExpansionViewController
{
	string CategoryName;
	string CategoryIcon;
	string CategoryInfo;
	ref ObservableCollection<ref ExpansionMarketMenuItem> MarketItems = new ObservableCollection<ref ExpansionMarketMenuItem>(this);
	bool CategoryExpand;
	
	ref ExpansionMarketMenuCategory m_Category;

	void ExpansionMarketMenuCategoryController()
	{
		if (!m_Category)
			m_Category = ExpansionMarketMenuCategory.Cast(GetParent());
	}
};