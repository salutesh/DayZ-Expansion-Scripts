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

class ExpansionPersonalStorageMenuItem: ExpansionPersonalStorageMenuItemBase
{
	protected ref ExpansionPersonalStorageMenuItemController m_PersonalStorageMenuItemController;
	protected ref ExpansionPersonalStorageItem m_Item;
	protected WrapSpacerWidget cargo_content;
	protected Widget tree_elements;
	protected Widget tree_panel_3;
	protected bool m_IsExcluded;
	protected bool m_IsStored;
	protected bool m_CargoDisplayState;

	void ExpansionPersonalStorageMenuItem(ExpansionPersonalStorageItem item, ExpansionPersonalStorageMenu menu)
	{
		Class.CastTo(m_PersonalStorageMenuItemController, GetController());

		m_Item = item;
		m_PersonalStorageMenu = menu;
		m_IsExcluded = item.IsExcluded();
		m_IsStored = item.IsStored();

		SetView();
	}

	void ~ExpansionPersonalStorageMenuItem()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();
	}

	void SetView()
	{
		if (!m_Item)
		{
			Error(ToString() + "::SetView - Could not get personal storage item!");
			return;
		}

		UpdatePreviewObject();
		UpdateItemName();

	#ifdef EXPANSIONMODHARDLINE
		SetRarityColor(m_Item.GetRarity());
	#endif

		if (m_Item.GetContainerItems() && m_Item.GetContainerItemsCount() > 0)
		{
			int cargoItemIndex = 0;
			array<ref ExpansionPersonalStorageContainerItem> containerItems = m_Item.GetContainerItems();
			foreach (ExpansionPersonalStorageContainerItem item: containerItems)
			{
				ExpansionPersonalStorageItem playerItem = new ExpansionPersonalStorageItem();
				if (!m_Item.IsStored() && m_Item.GetItem())
				{
					playerItem.SetFromItem(item.GetItem(), m_Item.GetOwnerUID());
				}
				else
				{
					playerItem.SetFromContainerItem(item, m_Item.GetStorageID(), m_Item.GetGlobalID(), m_Item.GetOwnerUID(), m_Item.IsStored());
				}

				playerItem.SetExcluded(item.IsExcluded());

				ExpansionPersonalStorageMenuItem playerItemEntry = new ExpansionPersonalStorageMenuItem(playerItem, m_PersonalStorageMenu);
				playerItemEntry.ShowTreeElements(true);
				m_PersonalStorageMenuItemController.CargoItems.Insert(playerItemEntry);
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
		
		if (m_IsStored)
		{
			m_PersonalStorageMenuItemController.ButtonText = "Retrieve";
		}
		else
		{
			m_PersonalStorageMenuItemController.ButtonText = "Deposit";
		}

		m_PersonalStorageMenuItemController.NotifyPropertyChanged("ButtonText");
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

		m_PersonalStorageMenuItemController.ItemName = displayText;
		m_PersonalStorageMenuItemController.NotifyPropertyChanged("ItemName");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_item.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuItemController;
	}

	void UpdatePreviewObject()
	{
		string previewClassName = m_PersonalStorageMenu.GetPreviewClassName(m_Item.GetClassName());
		ExpansionPersonalStorageMenu.CreatePreviewObject(previewClassName, m_Object);

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

			array<ref ExpansionPersonalStorageContainerItem> containerItems = m_Item.GetContainerItems();
			if (containerItems.Count() > 0)
			{
				Print(ToString() + "::UpdatePreviewObject - Attachments count:" + containerItems.Count() + " for item " + previewClassName);
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
				FoodStage foodStage;
				if (!m_IsStored)
				{
					foodStage = food_item.GetFoodStage();
					foodStage.ChangeFoodStage(m_Item.GetFoodStageType());
				}
				else
				{
					foodStage = food_item.GetFoodStage();
					foodStage.ChangeFoodStage(m_Item.GetFoodStageType());
					
					int storeTime = m_Item.GetStoreTime();
					int currentTime = CF_Date.Now(true).GetTimestamp();
					int elapsed = (currentTime - storeTime);
					
					Print(ToString() + "::UpdatePreviewObject - Time elapsed: " + GetTimeString(elapsed));
					FoodStageType processedFoodStage = food_item.Expansion_GetProcessedFoodStageDecay(elapsed, false);
					Print(ToString() + "::UpdatePreviewObject - Processed food stage: " + typename.EnumToString(FoodStageType, processedFoodStage));
					foodStage = food_item.GetFoodStage();
					foodStage.ChangeFoodStage(processedFoodStage);
					
					m_Item.SetFoodStageType(processedFoodStage);
				}
			}
		}

		m_PersonalStorageMenuItemController.Preview = m_Object;
		m_PersonalStorageMenuItemController.NotifyPropertyChanged("Preview");
	}
	
	string GetTimeString( float total_time )
	{
		string time_string;
		if ( total_time < 0 )
		{
			time_string =  "0 H";
			return time_string;
		}

		int time_seconds = total_time;	//! Convert total time to int
		int days = time_seconds / 86400;
		if ( days > 0 )
		{
			time_string += ExpansionStatic.GetValueString( days ) + " D";	//! Days
		}

		time_string += " ";	//! Separator

		int hours = ( time_seconds % 86400 ) / 3600;
		if ( hours > 0 )
		{
			time_string += ExpansionStatic.GetValueString( hours ) + " H";	//! Hours
		}

		time_string += " ";	//! Separator

		int minutes = ( time_seconds % 3600 ) / 60;
		time_string += ExpansionStatic.GetValueString( minutes ) + " M";	//! Minutes

		return time_string;
	}
	
	void OnItemButtonClick(ButtonCommandArgs args)
	{
		int button = args.GetMouseButton();
		bool buttonState = args.GetButtonState();
		
		if (button == MouseState.LEFT && buttonState)
		{
			if (!m_IsExcluded)
				m_PersonalStorageMenu.SetSelectedPlayerItem(this);
		}
		else if (button == MouseState.MIDDLE && m_Object && m_Item)
		{
			int rarity = -1;
		#ifdef EXPANSIONMODHARDLINE
			rarity = m_Item.GetRarity();
		#endif
			MissionGameplay.InspectItem(m_PersonalStorageMenu, m_Object, m_Item.GetHealthLevel(), m_Item.GetLiquidType(), m_Item.IsBloodContainer(), m_Item.GetQuantityType(), m_Item.GetQuantity(), m_Object.GetQuantityMax(), m_Item.GetFoodStageType(), m_Item.GetClassName(), rarity);
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

	void OnStateButtonClick(ButtonCommandArgs args)
	{
		int button = args.GetMouseButton();
		bool buttonState = args.GetButtonState();
		
		if (button == MouseState.LEFT && buttonState)
		{
			if (!m_IsExcluded)
			{
				m_PersonalStorageMenu.SetSelectedMenuItem(this);
				if (m_IsStored)
				{
					m_PersonalStorageMenu.OnConfirmRetrieveClick();
				}
				else
				{
					m_PersonalStorageMenu.OnConfirmDepositClick();
				}
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
		}

		return false;
	}

	ExpansionPersonalStorageItem GetPlayerItem()
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
	
	void SetExcluded(bool state)
	{
		m_IsExcluded = state;
	}

	ObservableCollection<ref ExpansionPersonalStorageMenuItem> GetCargoItemElemens()
	{
		return m_PersonalStorageMenuItemController.CargoItems;
	}
};

class ExpansionPersonalStorageMenuItemController: ExpansionPersonalStorageMenuItemBaseController
{
	string ButtonText;
	ref ObservableCollection<ref ExpansionPersonalStorageMenuItem> CargoItems = new ObservableCollection<ref ExpansionPersonalStorageMenuItem>(this);
};