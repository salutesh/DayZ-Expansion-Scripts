/**
 * ExpansionP2PMarketMenuItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuItem: ExpansionP2PMarketMenuItemBase
{
	protected ref ExpansionP2PMarketMenuItemController m_P2PMarketMenuItemController;
	protected ref ExpansionP2PMarketPlayerItem m_Item;
	protected WrapSpacerWidget cargo_content;
	protected Widget tree_elements;
	protected Widget tree_panel_3;
	protected bool m_IsExcluded;
	protected bool m_CargoDisplayState;

	void ExpansionP2PMarketMenuItem(ExpansionP2PMarketPlayerItem item, ExpansionP2PMarketMenu menu)
	{
		Class.CastTo(m_P2PMarketMenuItemController, GetController());

		m_Item = item;
		m_P2PMarketMenu = menu;
		m_IsExcluded = item.IsExcluded();

		SetView();
	}

	void ~ExpansionP2PMarketMenuItem()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();
	}

	void SetView()
	{
		UpdatePreviewObject();
		UpdateItemName();
		
	#ifdef EXPANSIONMODHARDLINE
		SetRarityColor(m_Item.GetRarity());
	#endif
		
		if (m_Item.GetContainerItems() && m_Item.GetContainerItemsCount() > 0)
		{
			int cargoItemIndex;
			array<ref ExpansionP2PMarketContainerItem> containerItems =  m_Item.GetContainerItems();
			foreach (ExpansionP2PMarketContainerItem item: containerItems)
			{
				EXPrint(ToString() + "::SetView - Container item: " + item.GetItem().GetType());
				ExpansionP2PMarketPlayerItem playerItem = new ExpansionP2PMarketPlayerItem();
				playerItem.SetFromItem(item.GetItem(), m_Item.GetOwnerUID());
				playerItem.SetExcluded(item.IsExcluded());
				ExpansionP2PMarketMenuItem playerItemEntry = new ExpansionP2PMarketMenuItem(playerItem, m_P2PMarketMenu);
				playerItemEntry.ShowTreeElements(true);
				m_P2PMarketMenuItemController.CargoItems.Insert(playerItemEntry);
				cargoItemIndex++;
				
				if (cargoItemIndex < m_Item.GetContainerItemsCount())
					playerItemEntry.ShowTreeElement(true);
			}
		}
				
		if (m_IsExcluded)
		{
			item_name_text.SetColor(ARGB(255, 192, 57, 43));
			item_button.SetColor(ARGB(255, 45, 52, 54));
		}
	}
	
	protected void UpdateItemName()
	{
		string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_Item.GetClassName());
		string displayText = displayName;
		if (m_Item.GetContainerItems().Count() > 0)
		{
			if (!m_CargoDisplayState)
				displayText = "[+] " + displayName;
			else
				displayText = "[-] " + displayName;
		}
		
		if (m_Item.GetQuantityType() == ExpansionItemQuantityType.PC || m_Item.GetQuantityType() == ExpansionItemQuantityType.MAGAZINE)
			displayText = displayText + " - [" + Math.Round(m_Item.GetQuantity()) + "]";
		
		m_P2PMarketMenuItemController.ItemName = displayText;
		m_P2PMarketMenuItemController.NotifyPropertyChanged("ItemName");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_item.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuItemController;
	}

	void UpdatePreviewObject()
	{
		string previewClassName = m_P2PMarketMenu.GetPreviewClassName(m_Item.GetClassName());
		ExpansionMarketMenu.CreatePreviewObject(previewClassName, m_Object);

		if (m_Object)
		{
			if (m_Object.IsInherited(TentBase))
			{
				TentBase tent;
				Class.CastTo(tent, m_Object);
				tent.Pack(false);
			}

			Transport transportEntity;
			if (Class.CastTo(transportEntity, m_Object))
			{
				dBodyActive(m_Object, ActiveState.INACTIVE);
				dBodyDynamic(m_Object, false);
				transportEntity.DisableSimulation(true);
			}

			array<ref ExpansionP2PMarketContainerItem> containerItems = m_Item.GetContainerItems();
			if (containerItems.Count() > 0)
			{
				EXPrint(ToString() + "::UpdatePreviewObject - Attachments count:" + containerItems.Count() + " for item " + previewClassName);
				SpawnAttachments(containerItems, m_Object, m_Item.GetSkinIndex());
			}

			if (m_Object.HasSelection("antiwater"))
				m_Object.HideSelection("antiwater");

			BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(m_Object);
			if (baseBuilding && baseBuilding.CanUseConstruction())
			{
				bool isSupportedBB;
				//! https://feedback.bistudio.com/T173348
				if (baseBuilding.GetType() == "Fence" || baseBuilding.GetType() == "Watchtower" || baseBuilding.GetType() == "TerritoryFlag")
					isSupportedBB = true;
				#ifdef EXPANSIONMODBASEBUILDING
				else if (baseBuilding.IsInherited(ExpansionBaseBuilding))
					isSupportedBB = true;
				#endif
				if (isSupportedBB)
				{
					Construction construction = baseBuilding.GetConstruction();
					construction.Init();
					construction.ExpansionBuildFull();
				}
			}

			Edible_Base food_item = Edible_Base.Cast(m_Object);
			if (food_item && food_item.HasFoodStage())
			{
				FoodStage foodStage = food_item.GetFoodStage();
				foodStage.ChangeFoodStage(m_Item.GetFoodStageType());
			}
		}

		m_P2PMarketMenuItemController.Preview = m_Object;
		m_P2PMarketMenuItemController.NotifyPropertyChanged("Preview");
	}

	void OnItemButtonClick(ButtonCommandArgs args)
	{
		int button = args.GetMouseButton();
		bool buttonState = args.GetButtonState();
		
		if (button == MouseState.LEFT && buttonState)
		{
			if (!m_IsExcluded)
				m_P2PMarketMenu.SetSelectedPlayerItem(this);
		}
		else if (button == MouseState.MIDDLE && m_Object && m_Item)
		{
			int rarity = -1;
		#ifdef EXPANSIONMODHARDLINE
			rarity = m_Item.GetRarity();
		#endif
			MissionGameplay.InspectItem(m_P2PMarketMenu, m_Object, m_Item.GetHealthLevel(), m_Item.GetLiquidType(), m_Item.IsBloodContainer(), m_Item.GetQuantityType(), m_Item.GetQuantity(), m_Object.GetQuantityMax(), m_Item.GetFoodStageType(), m_Item.GetClassName(), rarity);
		}
		else if (button == MouseState.RIGHT)
		{
			if (m_Item.GetContainerItemsCount() > 0)
			{
				if (!m_CargoDisplayState)
				{
					cargo_content.Show(true);
					m_CargoDisplayState = true;
				}
				else
				{
					cargo_content.Show(false);
					m_CargoDisplayState = false;
				}
				
				UpdateItemName();
			}
		}
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
			
			if (!m_IsExcluded)
			{
				item_name_text.SetColor(ARGB(255, 0, 0, 0));
				item_button.SetColor(ARGB(255, 255, 255, 255));
			}
			else
			{
				item_button.SetColor(ARGB(255, 45, 52, 54));
			}
			
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

			if (!m_IsExcluded)
			{
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
				item_button.SetColor(ARGB(255, 0, 0, 0));
			}
			else
			{
				item_name_text.SetColor(ARGB(255, 192, 57, 43));
				item_button.SetColor(ARGB(255, 45, 52, 54));
			}
			
			return true;
		}

		return false;
	}

	ExpansionP2PMarketPlayerItem GetPlayerItem()
	{
		return m_Item;
	}
	
	void ShowTreeElements(bool state)
	{
		tree_elements.Show(state);
	}
	
	void ShowTreeElement(bool state)
	{
		tree_panel_3.Show(state);
	}
	
	ObservableCollection<ref ExpansionP2PMarketMenuItem> GetCargoItemElemens()
	{
		return m_P2PMarketMenuItemController.CargoItems;
	}
};

class ExpansionP2PMarketMenuItemController: ExpansionP2PMarketMenuItemBaseController 
{
	ref ObservableCollection<ref ExpansionP2PMarketMenuItem> CargoItems = new ObservableCollection<ref ExpansionP2PMarketMenuItem>(this);
};