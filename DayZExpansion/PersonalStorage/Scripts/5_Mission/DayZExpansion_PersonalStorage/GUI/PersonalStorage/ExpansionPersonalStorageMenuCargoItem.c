/**
 * ExpansionPersonalStorageMenuItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuCargoItem: ExpansionPersonalStorageMenuItemBase
{
	void ExpansionPersonalStorageMenuCargoItem(ExpansionPersonalStorageItemBase item, ExpansionPersonalStorageMenu menu)
	{
		Print(ToString() + "::ExpansionPersonalStorageMenuCargoItem - Container item: " + item.ToString());
		Print(ToString() + "::ExpansionPersonalStorageMenuCargoItem - Container item class name: " + item.GetClassName());

		SetView();
	}

	void SetView()
	{
		UpdatePreviewObject();

		string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_Item.GetClassName());
		string displayText = displayName;
		if (m_Item.GetContainerItems().Count() > 0)
			displayText = "[+] " + displayName;
		
		if (m_Item.GetQuantityType() == ExpansionItemQuantityType.PC || m_Item.GetQuantityType() == ExpansionItemQuantityType.MAGAZINE)
			displayText = displayText + " - [" + Math.Round(m_Item.GetQuantity()) + "]";
		
		m_PersonalStorageMenuItemBaseController.ItemName = displayText;
		m_PersonalStorageMenuItemBaseController.NotifyPropertyChanged("ItemName");

	#ifdef EXPANSIONMODHARDLINE
		SetRarityColor(m_Item.GetRarity());
	#endif
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_container_item.layout";
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && w == item_button)
		{
			if (button == MouseState.MIDDLE && m_Object && m_Item)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_Item.GetRarity();
#endif
				MissionGameplay.InspectItem(m_PersonalStorageMenu, m_Object, m_Item.GetHealthLevel(), m_Item.GetLiquidType(), m_Item.IsBloodContainer(), m_Item.GetQuantityType(), m_Item.GetQuantity(), m_Object.GetQuantityMax(), m_Item.GetFoodStageType(), m_Item.GetClassName(), rarity);
				return true;
			}
		}
		
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && w == item_button)
		{		
			if (!m_ItemTooltip && m_Object && m_Item)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_Item.GetRarity();
#endif
				m_ItemTooltip = MissionGameplay.SetItemTooltip(m_Object, m_Item.GetHealthLevel(), m_Item.GetLiquidType(), m_Item.IsBloodContainer(), m_Item.GetQuantityType(), m_Item.GetQuantity(), m_Object.GetQuantityMax(), m_Item.GetFoodStageType(), m_Item.GetClassName(), rarity);
			}

			item_name_text.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL && w == item_button)
		{
			if (m_ItemTooltip)
			{
				MissionGameplay.Expansion_DestroyItemTooltip();
				m_ItemTooltip = null;
			}

		#ifdef EXPANSIONMODHARDLINE
			if (m_RarityColor != -1)
			{
				item_name_text.SetColor(m_RarityColor);
			}
			else
			{
				item_name_text.SetColor(ARGB(255, 255, 255, 255));
			}
		#else
			item_name_text.SetColor(ARGB(255, 255, 255, 255));
		#endif

			return true;
		}

		return false;
	}
};
