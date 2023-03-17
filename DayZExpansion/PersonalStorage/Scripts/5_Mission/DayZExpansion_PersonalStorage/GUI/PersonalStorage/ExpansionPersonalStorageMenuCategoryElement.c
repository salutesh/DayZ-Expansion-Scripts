/**
 * ExpansionPersonalStorageMenuCategoryElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuCategoryElement: ExpansionScriptView
{
	protected ref ExpansionPersonalStorageMenuCategoryElementController m_PersonalStorageMenuCategoryController;
	protected ref ExpansionPersonalStorageMenu m_PersonalStorageMenu;
	protected ref ExpansionPersonalStorageMenuCategory m_Category;
	protected bool m_ShowSubCategories;
	protected int m_SubCategoriesCount;

	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected WrapSpacerWidget sub_category_content;
	protected ImageWidget element_icon;
	protected ImageWidget category_icon;
	protected TextWidget category_items_count;

	void ExpansionPersonalStorageMenuCategoryElement(ExpansionPersonalStorageMenu menu, ExpansionPersonalStorageMenuCategory category)
	{
		Class.CastTo(m_PersonalStorageMenuCategoryController, GetController());

		m_PersonalStorageMenu = menu;
		m_Category = category;

		SetView();
	}

	void SetView()
	{
		m_PersonalStorageMenuCategoryController.CategoryName = m_Category.GetDisplayName();
		m_PersonalStorageMenuCategoryController.NotifyPropertyChanged("CategoryName");

		string iconPath = m_Category.GetIconPath();
		category_icon.LoadImageFile(0, iconPath);
		category_icon.SetImage(0);

		if (m_Category.GetSubCategories() && m_Category.GetSubCategories().Count() > 0)
		{
			element_icon.Show(true);

			int subCategoryIndex;
			array<ref ExpansionPersonalStorageMenuSubCategory> subCategories = m_Category.GetSubCategories();

			foreach (ExpansionPersonalStorageMenuSubCategory subCategory: subCategories)
			{
				subCategoryIndex++;
				ExpansionPersonalStorageMenuSubCategoryElement subCategoryElement = new ExpansionPersonalStorageMenuSubCategoryElement(m_PersonalStorageMenu, subCategory);
				m_PersonalStorageMenuCategoryController.SubCategories.Insert(subCategoryElement);

				if (m_Category.GetSubCategories().Count() > 1 && subCategoryIndex < m_Category.GetSubCategories().Count())
					subCategoryElement.ShowTreeElement(true);

			}
		}

		string itemsCountText = "[0]";
		int itemsCount = m_PersonalStorageMenu.GetCategoryItemsCount(m_Category);
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";

		m_PersonalStorageMenuCategoryController.CategoryItemsCount = itemsCountText;
		m_PersonalStorageMenuCategoryController.NotifyPropertyChanged("CategoryItemsCount");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_category.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuCategoryElementController;
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

		if (m_PersonalStorageMenuCategoryController.SubCategories.Count() > 0)
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

		m_PersonalStorageMenu.UpdateMenuCategory(m_Category);
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

class ExpansionPersonalStorageMenuCategoryElementController: ExpansionViewController
{
	string CategoryName;
	string CategoryItemsCount;
	ref ObservableCollection<ref ExpansionPersonalStorageMenuSubCategoryElement> SubCategories = new ObservableCollection<ref ExpansionPersonalStorageMenuSubCategoryElement>(this);
};

class ExpansionPersonalStorageMenuSubCategoryElement: ExpansionScriptView
{
	protected ref ExpansionPersonalStorageMenuSubCategoryElementController m_PersonalStorageMenuSubCategoryController;
	protected ref ExpansionPersonalStorageMenu m_PersonalStorageMenu;
	protected ref ExpansionPersonalStorageMenuSubCategory m_SubCategory;

	protected ButtonWidget button_element;
	protected TextWidget button_element_text;
	protected Widget tree_panel_3;
	protected ImageWidget category_icon;

	void ExpansionPersonalStorageMenuSubCategoryElement(ExpansionPersonalStorageMenu menu, ExpansionPersonalStorageMenuSubCategory subCategory)
	{
		Class.CastTo(m_PersonalStorageMenuSubCategoryController, GetController());

		m_PersonalStorageMenu = menu;
		m_SubCategory = subCategory;

		SetView();
	}

	void SetView()
	{
		m_PersonalStorageMenuSubCategoryController.SubCategoryName = m_SubCategory.GetDisplayName();
		m_PersonalStorageMenuSubCategoryController.NotifyPropertyChanged("SubCategoryName");

		string iconPath = m_SubCategory.GetIconPath();
		category_icon.LoadImageFile(0, iconPath);
		category_icon.SetImage(0);

		string itemsCountText = "[0]";
		int itemsCount = m_PersonalStorageMenu.GetCategoryItemsCount(m_SubCategory);
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";

		m_PersonalStorageMenuSubCategoryController.SubCategoryItemsCount = itemsCountText;
		m_PersonalStorageMenuSubCategoryController.NotifyPropertyChanged("SubCategoryItemsCount");
	}

	void ShowTreeElement(bool state)
	{
		tree_panel_3.Show(state);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_subcategory.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuSubCategoryElementController;
	}

	void OnElementButtonClick()
	{
		m_PersonalStorageMenu.UpdateMenuCategory(m_SubCategory);
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

class ExpansionPersonalStorageMenuSubCategoryElementController: ExpansionViewController
{
	string SubCategoryName;
	string SubCategoryItemsCount;
};
