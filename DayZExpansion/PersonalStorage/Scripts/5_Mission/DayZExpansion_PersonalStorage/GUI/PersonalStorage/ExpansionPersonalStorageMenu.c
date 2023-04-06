/**
 * ExpansionPersonalStorageMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionPersonalStorageMenuViewState
{
	NONE = 0,
	ViewPlayerItems = 1,
	ViewDepositedItems = 2,
	DetailViewPlayerItem = 3,
	DetailViewDepositedItem = 4,
};

class ExpansionPersonalStorageMenu: ExpansionScriptViewMenu
{
	protected ref ExpansionPersonalStorageMenuController m_PersonalStorageMenuController;
	protected ref array<ref ExpansionPersonalStorageItem> m_PlayerItems;
	protected ref array<ref ExpansionPersonalStorageMenuItem> m_DepositedItems;

	protected ExpansionPersonalStorageMenuViewState m_ViewState;
	protected ExpansionPersonalStorageMenuViewState m_PreviousViewState;

	protected ExpansionPersonalStorageMenuItem m_SelectedPlayerItem;
	Object m_SelectedPreviewObject;
	protected ref array<ref ExpansionPersonalStorageContainerItem> m_SelectedContainerItems;

	protected ref ExpansionPersonalStorageMenuDetailsView m_ItemDetailsView;
	protected ref ExpansionPersonalStorageMenuBrowseHeader m_BrowseHeader;

	protected ref TStringArray m_BloodContainers = {"BloodContainerBase"};

	protected Widget categories_panel;
	protected Widget inventory_panel;
	protected Widget details_panel;
	protected ScrollWidget inventory_scroller;
	protected WrapSpacerWidget inventory_header;
	protected WrapSpacerWidget inventory_content;
	protected ScrollWidget player_items_scroller;
	protected WrapSpacerWidget player_items_content;

	protected ButtonWidget browse_category_all_button;
	protected TextWidget browse_category_all_text;
	protected ImageWidget browse_category_all_icon;
	protected TextWidget browse_category_all_items_count;

	protected GridSpacerWidget tabs_panel;
	protected ButtonWidget tabs_button_browse;
	protected TextWidget tabs_button_browse_text;
	protected ButtonWidget tabs_button_deposit;
	protected TextWidget tabs_button_deposit_text;

	protected ButtonWidget exit_button;
	protected TextWidget exit_button_text;
	
	protected ButtonWidget toggle_categories_button;
	protected TextWidget toggle_categories_text;

	protected Widget market_filter_panel;
	protected EditBoxWidget market_filter_box;

	protected WrapSpacerWidget browse_categories_content;
	protected WrapSpacerWidget player_categories_content;
	protected Widget loading;
	
	protected GridSpacerWidget toggle_categories_panel;

	protected string m_PlayerUID;
	protected int m_StorageID = -1;
	protected bool m_RequestsLocked;

	protected ref ExpansionPersonalStorageSettings m_PersonalStorageSettings;

	protected bool m_CategoriesToggleState;

	void ExpansionPersonalStorageMenu()
	{
		Class.CastTo(m_PersonalStorageMenuController, GetController());

		ExpansionPersonalStorageModule.GetModuleInstance().GetPersonalStorageMenuCallbackSI().Insert(OnModuleCallback);
		ExpansionPersonalStorageModule.GetModuleInstance().GetPersonalStorageMenuSI().Insert(SetDepositedItems);

		m_PersonalStorageSettings = GetExpansionSettings().GetPersonalStorage();

		m_ItemDetailsView = new ExpansionPersonalStorageMenuDetailsView(this);
		m_ItemDetailsView.Hide();

		m_BrowseHeader = new ExpansionPersonalStorageMenuBrowseHeader(this);

		m_PlayerUID = GetGame().GetPlayer().GetIdentity().GetId();
	}

	void ~ExpansionPersonalStorageMenu()
	{
		if (m_BrowseHeader)
			m_BrowseHeader.Destroy();

		if (m_ItemDetailsView)
			m_ItemDetailsView.Destroy();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuController;
	}

	void SetDepositedItems(int storageID, array<ref ExpansionPersonalStorageItem> depositedItems = null, string displayName = string.Empty, string displayIcon = string.Empty)
	{
		PrintDebug(ToString() + "::SetDepositedItems - Start");
		PrintDebug(ToString() + "::SetDepositedItems - Storage ID: " + storageID);
		PrintDebug(ToString() + "::SetDepositedItems - Display Name: " + displayName);
		PrintDebug(ToString() + "::SetDepositedItems - Display Icon: " + displayIcon);
		PrintDebug(ToString() + "::SetDepositedItems - Start");
		
		int itemsCount;
		
		if (displayName != string.Empty)
		{
			m_PersonalStorageMenuController.PersonalStorageName = displayName;
			m_PersonalStorageMenuController.NotifyPropertyChanged("PersonalStorageName");
		}
		
		if (displayIcon != string.Empty)
		{
			string iconPath = ExpansionIcons.GetPath(displayIcon);
			if (iconPath == string.Empty)
				iconPath = displayIcon;
			
			m_PersonalStorageMenuController.PersonalStorageIcon = iconPath;
			m_PersonalStorageMenuController.NotifyPropertyChanged("PersonalStorageIcon");
		}
		
		m_StorageID = storageID;

		if (!m_DepositedItems)
			m_DepositedItems = new array<ref ExpansionPersonalStorageMenuItem>;
		else
			m_DepositedItems.Clear();

		inventory_header.AddChild(m_BrowseHeader.GetLayoutRoot());
		m_BrowseHeader.SetSort(0, false);

		if (depositedItems && depositedItems.Count() > 0)
		{
			itemsCount = depositedItems.Count();
			foreach (ExpansionPersonalStorageItem item: depositedItems)
			{
				ExpansionPersonalStorageMenuItem storedItem = new ExpansionPersonalStorageMenuItem(item, this);
				m_DepositedItems.Insert(storedItem);
				inventory_content.AddChild(storedItem.GetLayoutRoot());
			}

			SortListingsByName();
		}

		for (int i = m_PersonalStorageMenuController.BrowseCategories.Count() - 1; i >= 0; i--)
		{
			m_PersonalStorageMenuController.BrowseCategories.RemoveOrdered(i);
		}

		array<ref ExpansionPersonalStorageMenuCategory> menuCategories = m_PersonalStorageSettings.MenuCategories;
		foreach (ExpansionPersonalStorageMenuCategory category: menuCategories)
		{
			ExpansionPersonalStorageMenuCategoryElement categoryElement = new ExpansionPersonalStorageMenuCategoryElement(this, category);
			m_PersonalStorageMenuController.BrowseCategories.Insert(categoryElement);
		}

		//! Get and set all deposited items count
		string itemsCountText = "[0]";
		if (itemsCount > 0)
			itemsCountText = "[" + itemsCount + "]";

		m_PersonalStorageMenuController.AllItemsCount = itemsCount.ToString();
		m_PersonalStorageMenuController.NotifyPropertyChanged("AllItemsCount");

		int maxItemsPerStorage = m_PersonalStorageSettings.MaxItemsPerStorage;
	#ifdef EXPANSIONMODHARDLINE
		ExpansionPersonalStorageConfig personalStorageConfig = ExpansionPersonalStorageModule.GetModuleInstance().GetPersonalStorageClientConfig();
		if (personalStorageConfig)
		{
			int reputationToUnlock = personalStorageConfig.GetReputation();
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player)
			{
				int reputation = player.Expansion_GetReputation();
				int limit = ExpansionPersonalStorageModule.GetModuleInstance().GetStorageLimitByReputation(reputation, reputationToUnlock);
				maxItemsPerStorage = limit;
			}
		}
	#endif
		
		m_PersonalStorageMenuController.StoredItemsCount = itemsCount.ToString() + "/" + maxItemsPerStorage.ToString();
		m_PersonalStorageMenuController.NotifyPropertyChanged("StoredItemsCount");
		
		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionPersonalStorageMenuViewState.ViewDepositedItems;
		
		loading.Show(false);
		
		PrintDebug(ToString() + "::SetDepositedItems - End");
	}

	void UpdatePlayerItems()
	{
		PrintDebug(ToString() + "::UpdatePlayerItems - Start");

		if (!m_PlayerItems)
			m_PlayerItems = new array <ref ExpansionPersonalStorageItem>;
		else
			m_PlayerItems.Clear();

		array<EntityAI> items = ExpansionPersonalStorageModule.GetModuleInstance().LocalGetEntityInventory();
		if (items && items.Count() > 0)
		{
			foreach (EntityAI item: items)
			{
				if (item.GetHierarchyParent() && item.GetHierarchyParent().IsInherited(SurvivorBase))
				{
					//! Hardcoded excluded type names where the item should never get added and shown in the menu.
					if (ExpansionPersonalStorageModule.m_HardcodedExcludes.Find(item.GetType()) > -1)
						continue;
					
					ExpansionPersonalStorageItem newPlayerItem = new ExpansionPersonalStorageItem();
					newPlayerItem.SetFromItem(item, item.GetHierarchyRootPlayer().GetIdentity().GetId());
					m_PlayerItems.Insert(newPlayerItem);
				}
			}

			m_PersonalStorageMenuController.PlayerItems.Clear();

			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			array<EntityAI> slotItems = GetSlotItems(player);
			array<string> slotNames = new array<string>;
			slotNames.Insert("All");

			foreach (ExpansionPersonalStorageItem playerItem: m_PlayerItems)
			{
				if (!playerItem.GetItem())
					continue;

				for (int i = 0; i < slotItems.Count(); i++)
				{
					EntityAI slotItem = slotItems[i];
					if (!slotItem)
						continue;

					int playerItemSlotID;
					string playerItemSlotName;
					if (playerItem.GetItem() == slotItem)
					{
						slotItem.GetInventory().GetCurrentAttachmentSlotInfo(playerItemSlotID, playerItemSlotName);
						slotNames.Insert(playerItemSlotName);
						playerItem.SetSlotName(playerItemSlotName);
						continue;
					}
					else if (playerItem.GetItem().GetHierarchyParent() == slotItem)
					{
						playerItem.SetSlotName(playerItemSlotName);
						continue;
					}
				}

				ExpansionPersonalStorageMenuItem playerItemEntry = new ExpansionPersonalStorageMenuItem(playerItem, this);
				m_PersonalStorageMenuController.PlayerItems.Insert(playerItemEntry);
			}

			m_PersonalStorageMenuController.PlayerCategories.Clear();
			foreach (string slotName: slotNames)
			{
				ExpansionPersonalStorageMenuInventoryCategoryElement inventoryCategoryElement = new ExpansionPersonalStorageMenuInventoryCategoryElement(this, slotName);
				m_PersonalStorageMenuController.PlayerCategories.Insert(inventoryCategoryElement);
			}
		}
		
		loading.Show(false);
		
		PrintDebug(ToString() + "::UpdatePlayerItems - End");
	}

	void HideItemDetailElements()
	{
		PrintDebug(ToString() + "::HideItemDetailElements - Start");

		GetDetailsView().ShowQuantityColor(false);
		GetDetailsView().ShowItemQuantity(false);
		GetDetailsView().ShowItemLiquid(false);
		GetDetailsView().ShowItemCargoCount(false);
		GetDetailsView().ShowItemAttachmentsCount(false);
		GetDetailsView().ShowItemFoodState(false);
	#ifdef EXPANSIONMODHARDLINE
		GetDetailsView().ShowItemRarity(false);
	#endif
		
		PrintDebug(ToString() + "::HideItemDetailElements - End");
	}

	void SetSelectedMenuItem(ExpansionPersonalStorageMenuItem item)
	{
		PrintDebug(ToString() + "::SetSelectedMenuItem - Start");

		m_SelectedPlayerItem = item;
		
		PrintDebug(ToString() + "::SetSelectedMenuItem - End");
	}
	
	void SetSelectedPlayerItem(ExpansionPersonalStorageMenuItem item)
	{
		PrintDebug(ToString() + "::SetSelectedPlayerItem - Start");

		if (!item || !item.GetPlayerItem())
			return;

		GetDetailsView().ResetPreview();
		m_SelectedPlayerItem = item;
		m_SelectedPreviewObject = item.GetPreviewObject();
		m_SelectedContainerItems = item.GetPlayerItem().GetContainerItems();

		GetDetailsView().SetViewDeposit();

		GetDetailsView().GetDetailsViewController().SelectedName = item.GetPreviewObject().GetDisplayName();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("SelectedName");

		market_filter_panel.Show(false);
		categories_panel.Show(false);
		player_items_scroller.Show(false);
		inventory_panel.Show(false);
		tabs_panel.Show(false);

		GetDetailsView().ShowConfirmButton(true);

		string buttonText;
		if (item.GetPlayerItem().IsStored())
			buttonText = "Retrieve Item";
		else
			buttonText = "Deposit Item";

		GetDetailsView().GetDetailsViewController().ConfirmButtonText = buttonText;
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ConfirmButtonText");

		details_panel.AddChild(GetDetailsView().GetLayoutRoot());
		details_panel.Show(true);

		HideItemDetailElements();

		UpdateItemInfoDamage(item.GetPlayerItem().GetHealthLevel());

		item.GetPlayerItem().UpdateContainerItems();

		if (item.GetPlayerItem().GetLiquidType() != -1)
		{
			bool isBloodContainer = ExpansionStatic.IsAnyOf(item.GetPlayerItem().GetClassName(), m_BloodContainers);
			int liquidColor;
			GetDetailsView().ShowItemLiquid(true);
			GetDetailsView().GetDetailsViewController().LiquidType = GetLiquidTypeString(item.GetPlayerItem().GetLiquidType(), isBloodContainer, liquidColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("LiquidType");
			GetDetailsView().SetLiquidColor(liquidColor);
		}

		if (item.GetPlayerItem().GetFoodStageType() != -1)
		{
			int foodStageColor;
			GetDetailsView().ShowItemFoodState(true);
			GetDetailsView().GetDetailsViewController().FoodState = GetFoodStageString(item.GetPlayerItem().GetFoodStageType(), foodStageColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("FoodState");
			GetDetailsView().SetFoodStateColor(foodStageColor);
		}

		if (item.GetPlayerItem().GetQuantity() > 0)
		{
			int quantityColor;
			if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.POWER || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.MILLILITER || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.PERCENTAGE || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.GRAM)
			{
				GetDetailsView().GetQuantityBarWidget().SetCurrent(item.GetPlayerItem().GetQuantity());
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(item.GetPlayerItem().GetQuantityType(), item.GetPlayerItem().GetQuantity(), quantityColor);
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityBar(true);
			}
			else if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.HEATISOLATION)
			{
				int quantity = item.GetPlayerItem().GetQuantity();
				string isolationText;
				int isolationColor;
				if (quantity <= GameConstants.HEATISO_THRESHOLD_BAD)
				{
					isolationText = "#inv_inspect_iso_bad";
					isolationColor = GetTemperatureColor(10);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_BAD) && (quantity <= GameConstants.HEATISO_THRESHOLD_LOW))
				{
					isolationText = "#inv_inspect_iso_low";
					isolationColor = GetTemperatureColor(20);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_LOW) && (quantity <= GameConstants.HEATISO_THRESHOLD_MEDIUM))
				{
					isolationText = "#inv_inspect_iso_medium";
					isolationColor = GetTemperatureColor(30);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_MEDIUM) && (quantity <= GameConstants.HEATISO_THRESHOLD_HIGH))
				{
					isolationText = "#inv_inspect_iso_high";
					isolationColor = GetTemperatureColor(50);
				}
				else
				{
					isolationText = "#inv_inspect_iso_excel";
					isolationColor = GetTemperatureColor(70);
				}
				
				GetDetailsView().GetDetailsViewController().QuantityText = "Heat-Isolation:";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = isolationText;
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityColor(true, isolationColor);
			}
			else if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.PC)
			{
				GetDetailsView().GetDetailsViewController().QuantityText = "Quantity:";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(item.GetPlayerItem().GetQuantityType(), item.GetPlayerItem().GetQuantity(), quantityColor);
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowItemQuantity(true);
			}
		}

	#ifdef EXPANSIONMODHARDLINE
		if (item.GetPlayerItem().GetRarity() != ExpansionHardlineItemRarity.NONE)
		{
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, item.GetPlayerItem().GetRarity());
			string rarityText = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
			int rarityColor;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, rarityColor);
			int a, r, g, b;
			ExpansionStatic.IntToARGB(rarityColor, a, r, g, b);

			GetDetailsView().GetDetailsViewController().Rarity = rarityText;
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("Rarity");
			GetDetailsView().SetRarityColor(rarityColor);

			if (rarityText.Contains("Common"))
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 0, 0, 0));
			}
			else
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 255, 255, 255));
			}

			GetDetailsView().ShowItemRarity(true);
		}
	#endif

		GetDetailsView().ClearAttachments();
		GetDetailsView().ClearCargo();

		if (item.GetPlayerItem().GetContainerItemsCount() > 0)
		{
			int cargoItemsCount = 0;
			int attachmentsCount = 0;
			array<ref ExpansionPersonalStorageContainerItem> containerItems = item.GetPlayerItem().GetContainerItems();
			if (containerItems && containerItems.Count() > 0)
			{
				ExpansionPersonalStorageMenuCargoItem containerElement;
				string rarityType;
				for (int i = 0; i < containerItems.Count(); i++)
				{
					ExpansionPersonalStorageContainerItem containerItem = containerItems[i];
					PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item: " + containerItem.ToString());
					PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item class name: " + containerItem.GetClassName());
				#ifdef EXPANSIONMODHARDLINE
					rarityType = typename.EnumToString(ExpansionHardlineItemRarity, containerItem.GetRarity());
					PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item rarity: " + rarityType);
				#endif
					PrintDebug(ToString() + "::SetSelectedPlayerItem - Container is excluded: " + containerItem.IsExcluded().ToString());
					
					containerElement = new ExpansionPersonalStorageMenuCargoItem(this, containerItem);
					if (!containerElement)
						continue;

					if (!containerItem.IsAttached())
					{
						GetDetailsView().AddCargoEntry(containerElement);
						cargoItemsCount++;
					}
					else
					{
						GetDetailsView().AddAttachmentEntry(containerElement);
						attachmentsCount++;
					}

					array<ref ExpansionPersonalStorageContainerItem> containerItemsOfContainerItem = containerItem.GetContainerItems();
					if (containerItemsOfContainerItem && containerItemsOfContainerItem.Count() > 0)
					{
						foreach (ExpansionPersonalStorageContainerItem containerItemOfContainerItem: containerItemsOfContainerItem)
						{
							PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item of container item: " + containerItemOfContainerItem.ToString());
							PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item of container class name: " + containerItemOfContainerItem.GetClassName());
						#ifdef EXPANSIONMODHARDLINE
							rarityType = typename.EnumToString(ExpansionHardlineItemRarity, containerItemOfContainerItem.GetRarity());
							PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item of container rarity: " + rarityType);
						#endif
							PrintDebug(ToString() + "::SetSelectedPlayerItem - Container item of container is excluded: " + containerItemOfContainerItem.IsExcluded().ToString());

							containerElement = new ExpansionPersonalStorageMenuCargoItem(this, containerItemOfContainerItem);
							
							if (!containerItemOfContainerItem.IsAttached())
							{
								GetDetailsView().AddCargoEntry(containerElement);
								cargoItemsCount++;
							}
							else
							{
								GetDetailsView().AddAttachmentEntry(containerElement);
								attachmentsCount++;
							}
						}
					}
				}
			}
			
			if (cargoItemsCount > 0)
			{
				GetDetailsView().ShowItemCargoCount(true);
				GetDetailsView().GetDetailsViewController().CargoItemsCount = cargoItemsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("CargoItemsCount");
			}
			
			if (attachmentsCount > 0)
			{
				GetDetailsView().ShowItemAttachmentsCount(true);
				GetDetailsView().GetDetailsViewController().AttachmentItemsCount = attachmentsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("AttachmentItemsCount");
			}
		}

		GetDetailsView().Show();
		GetDetailsView().UpdatePreview();

		m_PreviousViewState = m_ViewState;

		if (item.GetPlayerItem().IsStored())
		{
			m_ViewState = ExpansionPersonalStorageMenuViewState.DetailViewDepositedItem;
		}
		else
		{
			m_ViewState = ExpansionPersonalStorageMenuViewState.DetailViewPlayerItem;
		}

		PlayObjectSound();
		
		PrintDebug(ToString() + "::SetSelectedPlayerItem - End");
	}

	protected void PlayObjectSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (GetGame().GetPlayer())
			{
				string soundConfig;
				string typeName = m_SelectedPlayerItem.GetPreviewObject().GetType();
				string path = "cfgVehicles " + typeName + " " + "AnimEvents ";
				int events_sources_count = GetGame().ConfigGetChildrenCount(path);
				string soundName = "";
				EffectSound sound;

				for (int i = 0; i < events_sources_count; i++)
				{
					string soundWeapons;
					GetGame().ConfigGetChildName(path, i, soundWeapons);

					if (GetGame().ConfigIsExisting(path + soundWeapons + " pickUpItem" + " soundSet"))
						GetGame().ConfigGetText(path + soundWeapons + " pickUpItem" + " soundSet", soundName);
					else if (GetGame().ConfigIsExisting(path + soundWeapons + " pickUpItem_Light" + " soundSet"))
						GetGame().ConfigGetText(path + soundWeapons + " pickUpItem_Light" + " soundSet", soundName);
				}

				if (soundName != "")
					sound = SEffectManager.PlaySound(soundName, GetGame().GetPlayer().GetPosition());
				else
					sound = SEffectManager.PlaySound("pickUpBackPack_Metal_SoundSet", GetGame().GetPlayer().GetPosition());

				sound.SetSoundAutodestroy(true);
			}
		}
	}

	protected string GetQuantityString(int quantityType, float quantity, inout int color)
	{
		string quantity_str;
		if (quantityType == ExpansionItemQuantityType.PC)
		{
			if (quantity == 1)
			{
				quantity_str =  quantity.ToString() + " " + "#inv_inspect_piece";
				color = Colors.COLOR_DEFAULT;
			}
			else
			{
				quantity_str =  quantity.ToString() + " " + "#inv_inspect_pieces";
				color = Colors.COLOR_DEFAULT;
			}
		}
		else if (quantityType == ExpansionItemQuantityType.PERCENTAGE)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.GRAM)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.MILLILITER)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.POWER)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.MAGAZINE)
		{
			if (quantity == 1)
			{
				quantity_str = quantity.ToString() + " " + "#inv_inspect_piece";
				color = Colors.COLOR_DEFAULT;
			}
			else
			{
				quantity_str = quantity.ToString() + " " + "#inv_inspect_pieces";
				color = Colors.COLOR_DEFAULT;
			}
		}
		else
		{
			quantity_str = "";
			color = 0;
		}

		return quantity_str;
	}

	protected string GetFoodStageString(int foodStage, inout int color)
	{
		if (foodStage == -1)
		{
			color = Colors.COLOR_ROTTEN;
			return "ERROR";
		}

		string text;
		switch (foodStage)
		{
			case FoodStageType.RAW:
			{
				text = "#inv_inspect_raw";
				color = Colors.COLOR_RAW;
				break;
			}
			case FoodStageType.BAKED:
			{
				text = "#inv_inspect_baked";
				color = Colors.COLOR_BAKED;
				break;
			}
			case FoodStageType.BOILED:
			{
				text = "#inv_inspect_boiled";
				color = Colors.COLOR_BOILED;
				break;
			}
			case FoodStageType.DRIED:
			{
				text = "#inv_inspect_dried";
				color = Colors.COLOR_DRIED;
				break;
			}
			case FoodStageType.BURNED:
			{
				text = "#inv_inspect_burned";
				color = Colors.COLOR_BURNED;
				break;
			}
			case FoodStageType.ROTTEN:
			{
				text = "#inv_inspect_rotten";
				color = Colors.COLOR_ROTTEN;
				break;
			}
		}

		return text;
	}

	protected string GetLiquidTypeString(int liquid_type, bool isBloodContainer, inout int color)
	{
		string text;
		if (isBloodContainer)
		{
			if (liquid_type > -1)
			{
				string type;
				bool positive;
				string blood_type_name = BloodTypes.GetBloodTypeName(liquid_type, type, positive);
				text = "#inv_inspect_blood: " + blood_type_name;
				color = Colors.COLOR_LIQUID;
			}
			else if (liquid_type == -1)
			{
				text = "#inv_inspect_blood";
				color = Colors.COLOR_LIQUID;
			}
		}
		else if (liquid_type > -1)
		{
			switch (liquid_type)
			{
				case LIQUID_WATER:
				{
					text = "#inv_inspect_water";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_RIVERWATER:
				{
					text = "#inv_inspect_river_water";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_VODKA:
				{
					text = "#inv_inspect_vodka";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_BEER:
				{
					text = "#inv_inspect_beer";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case EXPANSION_LIQUID_MILK:
				{
					text = "MILK";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_GASOLINE:
				{
					text = "#inv_inspect_gasoline";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_DIESEL:
				{
					text = "#inv_inspect_diesel";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_DISINFECTANT:
				{
					text = "#inv_inspect_disinfectant";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_SALINE:
				{
					text = "#inv_inspect_saline";
					color = Colors.COLOR_LIQUID;
					break;
				}

				default:
				{
					text = "ERROR";
					color = Colors.COLOR_LIQUID;
					break;
				}
			}
		}

		return text;
	}

	protected void UpdateItemInfoDamage(int damageLevel)
	{
		string text;
		int color;

		switch (damageLevel)
		{
			case GameConstants.STATE_RUINED:
			{
				text = "#inv_inspect_ruined";
				color = Colors.COLOR_RUINED;
				break;
			}
			case GameConstants.STATE_BADLY_DAMAGED:
			{
				text = "#inv_inspect_badly";
				color = Colors.COLOR_BADLY_DAMAGED;
				break;
			}

			case GameConstants.STATE_DAMAGED:
			{
				text = "#inv_inspect_damaged";
				color = Colors.COLOR_DAMAGED;
				break;
			}

			case GameConstants.STATE_WORN:
			{
				text = "#inv_inspect_worn";
				color = Colors.COLOR_WORN;
				break;
			}

			case GameConstants.STATE_PRISTINE:
			{
				text = "#inv_inspect_pristine";
				color = Colors.COLOR_PRISTINE;
				break;
			}

			default:
			{
				text = "ERROR";
				color = Colors.COLOR_PRISTINE;
				break;
			}
		}

		GetDetailsView().GetDetailsViewController().ItemHealth = text;
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemHealth");
		GetDetailsView().GetHealthImageWidget().SetColor(color | 0x7F000000);
	}

	protected bool ItemCheck(EntityAI item)
	{
		if (!ItemCheckEx(item))
			return false;

		//! Don't add the optic attachment on AUG's.
		if (item.GetType() == "AugOptic")
			return false;
		
		//! Don`t add items that have excuded items in cargo
		array<EntityAI> cargoItems = new array<EntityAI>;
		item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, cargoItems);
		foreach (EntityAI cargoItem: cargoItems)
		{
			if (!ItemCheckEx(cargoItem))
				return false;
		}

		return true;
	}

	protected bool ItemCheckEx(EntityAI item)
	{
		if (ExpansionStatic.IsAnyOf(item, GetExpansionSettings().GetPersonalStorage().ExcludedClassNames))
			return false;

		if (item.IsRuined())
			return false;

		//! Don`t add rotten food items
		Edible_Base foodItem;
		if (Class.CastTo(foodItem, item) && foodItem.HasFoodStage())
		{
			FoodStage foodStage = foodItem.GetFoodStage();
			FoodStageType foodStageType = foodStage.GetFoodStageType();
			if (foodStageType == FoodStageType.ROTTEN || foodStageType == FoodStageType.BURNED)
				return false;
		}

	#ifdef WRDG_DOGTAGS
		//! Don`t add players own dogtag
		if (item.IsInherited(Dogtag_Base))
		{
			if (item.GetHierarchyRootPlayer())
				return false;
		}
	#endif

	#ifdef EXPANSIONMODQUESTS
		//! Don`t add quest items
		ItemBase itemIB;
		if (Class.CastTo(itemIB, item))
		{
			if (itemIB.IsQuestItem() || itemIB.IsQuestGiver())
				return false;
		}
	#endif

		//! Don`t add any active items.
		if (item.HasEnergyManager())
		{
			if (item.GetCompEM().IsWorking())
				return false;
		}

		return true;
	}

	string GetPreviewClassName(string className, bool ignoreBaseBuildingKits = false)
	{
		if (GetGame().ConfigIsExisting("CfgVehicles " + className + "_ExpansionMarketPreview"))
		{
			return className + "_ExpansionMarketPreview";
		}
		else if (!ignoreBaseBuildingKits && className.IndexOf("kit") == className.Length() - 3)
		{
			//! Special handling for Expansion
			if (GetGame().IsKindOf(className, "ExpansionKitLarge"))
			{
				string path = "CfgVehicles " + className + " placingTypes";
				if (GetGame().ConfigIsExisting(path))
				{
					TStringArray placingTypes = new TStringArray;
					GetGame().ConfigGetTextArray(path, placingTypes);
					foreach (string placingType : placingTypes)
					{
						path = "CfgVehicles " + placingType + " deployType";
						if (GetGame().ConfigIsExisting(path))
						{
							return GetGame().ConfigGetTextOut(path);
						}
					}
				}
			}

			if (className == "fencekit" || className == "watchtowerkit" || className == "territoryflagkit")
			{
				//! Item name is kit name without "kit" at the end
				string previewClassName = className.Substring(0, className.Length() - 3);
				if (GetGame().ConfigIsExisting("CfgVehicles " + previewClassName))
					return previewClassName;
			}
		}
		return className;
	}

	array<EntityAI> GetSlotItems(PlayerBase player)
	{
		array<EntityAI> slotItems = new array<EntityAI>;
		for (int i = 0; i < player.GetInventory().GetAttachmentSlotsCount(); i++)
		{
			int slot = player.GetInventory().GetAttachmentSlotId(i);
			EntityAI slotItem = player.GetInventory().FindAttachment(slot);
			if (slotItem)
				slotItems.Insert(slotItem);
		}

		return slotItems;
	}

	void UpdateInventorySlotFilter(string slotName)
	{
		for (int i = 0; i < m_PersonalStorageMenuController.PlayerItems.Count(); i++)
		{
			ExpansionPersonalStorageMenuItem playerItem = m_PersonalStorageMenuController.PlayerItems[i];
			if (playerItem.GetPlayerItem().GetSlotName() != slotName && slotName != "All")
			{
				playerItem.Hide();
			}
			else
			{
				playerItem.Show();
			}
		}
	}

	int GetInventoryCategoryItemsCount(string slotName)
	{
		int count;
		if (slotName == "All")
		{
			return m_PersonalStorageMenuController.PlayerItems.Count();
		}
		else
		{
			for (int i = 0; i < m_PersonalStorageMenuController.PlayerItems.Count(); i++)
			{
				ExpansionPersonalStorageMenuItem playerItem = m_PersonalStorageMenuController.PlayerItems[i];
				if (playerItem.GetPlayerItem().GetSlotName() == slotName)
					count++;
			}
		}

		return count;
	}

	void OnDepositButtonClick()
	{
		PrintDebug(ToString() + "::OnDepositButtonClick - Start");

		if (m_ViewState == ExpansionPersonalStorageMenuViewState.ViewPlayerItems)
		{
			m_BrowseHeader.Hide();
			tabs_panel.Show(false);
		}
		else if (m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewPlayerItem || m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewDepositedItem)
		{
			ClearSelected();
			GetDetailsView().Hide();

			categories_panel.Show(true);
			inventory_panel.Show(true);
			player_items_scroller.Show(true);
			details_panel.Show(false);
		}
		
		ShowDepositView();
		
		PrintDebug(ToString() + "::OnDepositButtonClick - End");
	}

	void OnBackClick()
	{
		PrintDebug(ToString() + "::OnBackClick - Start");

		ClearSelected();

		if (m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewPlayerItem)
		{
			OnDepositButtonClick();
		}
		else if (m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewDepositedItem)
		{
			OnBrowseButtonClick();
		}
		
		PrintDebug(ToString() + "::OnBackClick - End");
	}

	void OnBrowseButtonClick()
	{
		PrintDebug(ToString() + "::OnBrowseButtonClick - Start");

		if (m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewPlayerItem || m_ViewState == ExpansionPersonalStorageMenuViewState.DetailViewDepositedItem)
		{
			ClearSelected();
			GetDetailsView().Hide();

			categories_panel.Show(true);
			inventory_panel.Show(true);
			player_items_scroller.Show(false);
			details_panel.Show(false);
		}

		ShowBrowseView();
		
		PrintDebug(ToString() + "::OnBrowseButtonClick - End");
	}

	protected void ShowDepositView()
	{
		PrintDebug(ToString() + "::ShowDepositView - Start");

		tabs_panel.Show(true);
		market_filter_panel.Show(true);
		categories_panel.Show(true);
		player_items_scroller.Show(true);
		player_categories_content.Show(true);

		inventory_scroller.Show(false);
		toggle_categories_panel.Show(false);
		browse_categories_content.Show(false);
		toggle_categories_panel.Show(false);
		
		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionPersonalStorageMenuViewState.ViewPlayerItems;
		
		PrintDebug(ToString() + "::ShowDepositView - End");
	}

	protected void ShowBrowseView()
	{
		PrintDebug(ToString() + "::ShowBrowseView - Start");

		m_BrowseHeader.Show();
		tabs_panel.Show(true);
		market_filter_panel.Show(true);
		categories_panel.Show(true);
		inventory_scroller.Show(true);
		browse_categories_content.Show(true);
		toggle_categories_panel.Show(true);
		
		player_items_scroller.Show(false);
		player_categories_content.Show(false);
		toggle_categories_panel.Show(true);
		
		ExpansionPersonalStorageModule.GetModuleInstance().EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
		UpdatePlayerItems();
		
		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionPersonalStorageMenuViewState.ViewDepositedItems;
		
		PrintDebug(ToString() + "::ShowBrowseView - End");
	}
	
	protected void SetCategory_All()
	{
		foreach (ExpansionPersonalStorageMenuItem entry: m_DepositedItems)
		{
			if (!entry || !entry.GetPlayerItem())
				continue;

			entry.Show();
		}
	}
	
	void OnClickCategory_All()
	{
		SetCategory_All();
	}

	void OnConfirmButtonClick()
	{
		PrintDebug(ToString() + "::OnConfirmButtonClick - Start");

		if (m_RequestsLocked)
			return;

		if (!m_SelectedPlayerItem.GetPlayerItem().IsStored())
		{
			OnConfirmDepositClick();
		}
		else
		{
			OnConfirmRetrieveClick();
		}
		
		PrintDebug(ToString() + "::OnConfirmButtonClick - End");
	}

	void OnConfirmDepositClick()
	{
		PrintDebug(ToString() + "::OnConfirmDepositClick - Start");

		if (!m_SelectedPlayerItem || !m_SelectedPlayerItem.GetPlayerItem() || m_RequestsLocked)
			return;

		EntityAI playerItem = m_SelectedPlayerItem.GetPlayerItem().GetItem();
		if (!playerItem)
			return;

		loading.Show(true);
		m_RequestsLocked = true;
		ExpansionPersonalStorageModule.GetModuleInstance().RequestDepositItem(m_StorageID, playerItem);
		
		GetDetailsView().UpdatePreview();
		
		PrintDebug(ToString() + "::OnConfirmDepositClick - End");
	}

	void OnConfirmRetrieveClick()
	{
		PrintDebug(ToString() + "::OnConfirmRetrieveClick - Start");

		if (!m_SelectedPlayerItem || !m_SelectedPlayerItem.GetPlayerItem() || m_RequestsLocked)
			return;

		loading.Show(true);
		m_RequestsLocked = true;
		ExpansionPersonalStorageModule.GetModuleInstance().RequestRetrieveItem(m_SelectedPlayerItem.GetPlayerItem(), m_StorageID);
		
		GetDetailsView().UpdatePreview();
		
		PrintDebug(ToString() + "::OnConfirmRetrieveClick - End");
	}

	protected void ClearSelected()
	{
		PrintDebug(ToString() + "::ClearSelected - Start");

		m_SelectedPlayerItem = null;
		m_SelectedPreviewObject = null;
		m_SelectedContainerItems = null;

		GetDetailsView().ShowConfirmButton(false);

		if (m_PreviousViewState == ExpansionPersonalStorageMenuViewState.DetailViewPlayerItem || m_PreviousViewState == ExpansionPersonalStorageMenuViewState.DetailViewDepositedItem)
		{
			GetDetailsView().GetDetailsViewController().SelectedName = "";
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("SelectedName");
		}
		
		PrintDebug(ToString() + "::ClearSelected - End");
	}

	protected void OnSearchFilterChange()
	{
		PrintDebug(ToString() + "::OnSearchFilterChange - Start");

		if (m_ViewState == ExpansionPersonalStorageMenuViewState.ViewPlayerItems)
		{
			SearchInPlayerItems();
		}
		else if (m_ViewState == ExpansionPersonalStorageMenuViewState.ViewDepositedItems)
		{
			SearchInDepositedItems();
		}

		PrintDebug(ToString() + "::OnSearchFilterChange - End");
	}
	
	protected void SearchInDepositedItems()
	{
		PrintDebug(ToString() + "::SearchInDepositedItems - Start");
		
		if (!m_DepositedItems || m_DepositedItems.Count() == 0)
			return;

		bool hasChildWithName;
		string displayName;
		string searchText = market_filter_box.GetText();
		searchText.ToLower();

		for (int i = 0; i < m_DepositedItems.Count(); i++)
		{
			ExpansionPersonalStorageMenuItem entry = m_DepositedItems[i];
			if (entry && entry.GetPreviewObject())
			{
				if (!entry.IsVisible() || searchText == string.Empty)
					entry.Show();
				
				hasChildWithName = false;					
				ObservableCollection<ref ExpansionPersonalStorageMenuItem> cargoEntries = entry.GetCargoItemElemens();
				if (cargoEntries && cargoEntries.Count() > 0)
				{
					for (int k = 0; k < cargoEntries.Count(); k++)
					{
						ExpansionPersonalStorageMenuItem cargoEntry = cargoEntries[k];
						if (!cargoEntry.IsVisible() || searchText == string.Empty)
							cargoEntry.Show();

						displayName = cargoEntry.GetPreviewObject().GetDisplayName();
						displayName.ToLower();
						
						if (displayName.IndexOf(searchText) == -1)
						{
							cargoEntry.Hide();
						}
						else
						{
							hasChildWithName = true;
						}
					}
				}
				
				displayName = entry.GetPreviewObject().GetDisplayName();
				displayName.ToLower();
				
				if (displayName.IndexOf(searchText) == -1 && !hasChildWithName)
					entry.Hide();
			}
		}
		
		PrintDebug(ToString() + "::SearchInDepositedItems - End");
	}

	protected void SearchInPlayerItems()
	{
		PrintDebug(ToString() + "::SearchInPlayerItems - Start");
		
		if (!m_PersonalStorageMenuController.PlayerItems || m_PersonalStorageMenuController.PlayerItems.Count() == 0)
			return;

		bool hasChildWithName;
		string displayName;
		string searchText = market_filter_box.GetText();
		searchText.ToLower();

		for (int i = 0; i < m_PersonalStorageMenuController.PlayerItems.Count(); i++)
		{
			ExpansionPersonalStorageMenuItem entry = m_PersonalStorageMenuController.PlayerItems[i];
			if (entry && entry.GetPreviewObject())
			{
				if (!entry.IsVisible() || searchText == string.Empty)
					entry.Show();
				
				hasChildWithName = false;					
				ObservableCollection<ref ExpansionPersonalStorageMenuItem> cargoEntries = entry.GetCargoItemElemens();
				if (cargoEntries && cargoEntries.Count() > 0)
				{
					for (int k = 0; k < cargoEntries.Count(); k++)
					{
						ExpansionPersonalStorageMenuItem cargoEntry = cargoEntries[k];
						if (!cargoEntry.IsVisible() || searchText == string.Empty)
							cargoEntry.Show();

						displayName = cargoEntry.GetPreviewObject().GetDisplayName();
						displayName.ToLower();
						
						if (displayName.IndexOf(searchText) == -1)
						{
							cargoEntry.Hide();
						}
						else
						{
							hasChildWithName = true;
						}
					}
				}
				
				displayName = entry.GetPreviewObject().GetDisplayName();
				displayName.ToLower();
				
				if (displayName.IndexOf(searchText) == -1 && !hasChildWithName)
					entry.Hide();
			}
		}
		
		PrintDebug(ToString() + "::SearchInPlayerItems - End");
	}

	void OnModuleCallback(ExpansionPersonalStorageModuleCallback callback)
	{
		PrintDebug(ToString() + "::OnModuleCallback - Start");
		string callbackName = typename.EnumToString(ExpansionPersonalStorageModuleCallback, callback);
		PrintDebug(ToString() + "::OnModuleCallback - Callback: " + callbackName);

		switch (callback)
		{
			case ExpansionPersonalStorageModuleCallback.ItemStored:
			{
				ShowDepositView();
				m_RequestsLocked = false;
			}
			break;
			case ExpansionPersonalStorageModuleCallback.ItemRetrieved:
			{
				ExpansionPersonalStorageModule.GetModuleInstance().EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
				UpdatePlayerItems();
				m_RequestsLocked = false;
			}
			break;
			case ExpansionPersonalStorageModuleCallback.Error:
			{
				ExpansionPersonalStorageModule.GetModuleInstance().EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
				UpdatePlayerItems();
				OnBackClick();
				m_RequestsLocked = false;
			}
			break;
		}
		
		PrintDebug(ToString() + "::OnModuleCallback - End");
	}

	override bool CanClose()
	{
		bool canClose = !m_RequestsLocked;
		return canClose;
	}
	
	void OnExitClick()
	{
		CloseMenu();
	}

	void OnToggleCategoriesClick()
	{
		m_CategoriesToggleState = !m_CategoriesToggleState;
		for (int i = 0; i < m_PersonalStorageMenuController.BrowseCategories.Count(); i++)
		{
			ExpansionPersonalStorageMenuCategoryElement categoryElement = m_PersonalStorageMenuController.BrowseCategories[i];
			if (!categoryElement)
				continue;

			categoryElement.ShowSubCategories(m_CategoriesToggleState);
		}
	}

	override void OnShow()
	{
		super.OnShow();
		
		SetFocus(GetLayoutRoot());
		loading.Show(true);
		ShowBrowseView();
	}

	override void OnHide()
	{
		if (!GetGame())
			return;

		if (m_ItemDetailsView)
			m_ItemDetailsView.Destroy();

		if (m_BrowseHeader)
			m_BrowseHeader.Destroy();

		super.OnHide();
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == market_filter_box)
		{
			OnSearchFilterChange();
			return true;
		}

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == exit_button)
			{
				exit_button.SetColor(ARGB(255, 255, 255, 255));
				exit_button_text.SetColor(ARGB(255, 0, 0, 0));
			}
			else if (w == browse_category_all_button)
			{
				browse_category_all_button.SetColor(ARGB(255, 255, 255, 255));
				browse_category_all_text.SetColor(ARGB(255, 0, 0, 0));
				browse_category_all_icon.SetColor(ARGB(255, 0, 0, 0));
				browse_category_all_items_count.SetColor(ARGB(255, 0, 0, 0));
			}
			else if (w == toggle_categories_button)
			{
				toggle_categories_button.SetColor(ARGB(255, 255, 255, 255));
				toggle_categories_text.SetColor(ARGB(255, 0, 0, 0));
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == exit_button)
			{
				exit_button.SetColor(ARGB(255, 0, 0, 0));
				exit_button_text.SetColor(ARGB(255, 255, 255, 255));
			}
			else if (w == browse_category_all_button)
			{
				browse_category_all_button.SetColor(ARGB(255, 0, 0, 0));
				browse_category_all_text.SetColor(ARGB(255, 255, 255, 255));
				browse_category_all_icon.SetColor(ARGB(255, 255, 255, 255));
				browse_category_all_items_count.SetColor(ARGB(255, 255, 255, 255));
			}
			else if (w == toggle_categories_button)
			{
				toggle_categories_button.SetColor(ARGB(255, 0, 0, 0));
				toggle_categories_text.SetColor(ARGB(255, 255, 255, 255));
			}
		}

		return false;
	}

	void SortListingsByName(bool reverse = false)
	{
		TStringArray displayName = new TStringArray;
		foreach (ExpansionPersonalStorageMenuItem entry: m_DepositedItems)
		{
			if (!entry || !entry.GetPlayerItem())
				continue;

			displayName.Insert(entry.GetPreviewObject().GetDisplayName());
		}

		displayName.Sort(reverse);

		foreach (ExpansionPersonalStorageMenuItem currentEntry: m_DepositedItems)
		{
			if (!currentEntry || !currentEntry.GetPlayerItem())
				continue;

			string name = currentEntry.GetPreviewObject().GetDisplayName();
			int index = displayName.Find(name) + 1;
			currentEntry.SetSort(index, false);
		}
	}

	void Browse_Filter_ClassNameAZ()
	{
		SortListingsByName();
	}

	void Browse_Filter_ClassNameZA()
	{
		SortListingsByName(true);
	}

	void UpdateMenuCategory(ExpansionPersonalStorageMenuCategoryBase category)
	{
		foreach (ExpansionPersonalStorageMenuItem entry: m_DepositedItems)
		{
			if (!entry || !entry.GetPlayerItem())
				continue;

			TStringArray included = category.GetIncluded();
			TStringArray excluded = category.GetExcluded();
			if (ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), included) && !ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), excluded))
			{
				entry.Show();
			}
			else
			{
				entry.Hide();
			}
		}
	}

	int GetCategoryItemsCount(ExpansionPersonalStorageMenuCategoryBase category)
	{
		int count;
		foreach (ExpansionPersonalStorageMenuItem entry: m_DepositedItems)
		{
			if (!entry || !entry.GetPlayerItem())
				continue;

			TStringArray included = category.GetIncluded();
			TStringArray excluded = category.GetExcluded();
			if (ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), included) && !ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), excluded))
				count++;
		}

		return count;
	}

	ExpansionPersonalStorageMenuDetailsView GetDetailsView()
	{
		return m_ItemDetailsView;
	}

	Object GetSelectedPreviewObject()
	{
		return m_SelectedPreviewObject;
	}

	array<ref ExpansionPersonalStorageContainerItem> GetSelectedContainerItems()
	{
		return m_SelectedContainerItems;
	}
	
	static void CreatePreviewObject(string className, inout EntityAI preview)
	{
		if (preview)
		{
			if (CF_String.EqualsIgnoreCase(preview.GetType(), className))
			{
				//! Same classname, remove all attachments
				for (int i = preview.GetInventory().AttachmentCount() - 1; i >= 0; i--)
				{
					EntityAI attachment = preview.GetInventory().GetAttachmentFromIndex(i);
					if (attachment)
						GetGame().ObjectDelete(attachment);
				}

				return;
			}
			else
			{
				//! Different classname, delete old preview object
				GetGame().ObjectDelete(preview);
			}
		}

		if (!GetGame().IsKindOf(className, "DZ_LightAI"))
		{
			preview = EntityAI.Cast(GetGame().CreateObjectEx(className, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
		#ifdef EXPANSIONMODHARDLINE
			ItemBase item;
			if (GetExpansionSettings().GetHardline().EnableItemRarity && Class.CastTo(item, preview))
				item.Expansion_SetRarity(GetExpansionSettings().GetHardline().GetItemRarityByType(item.GetType()));
		#endif
		}
	}

	void PrintDebug(string text)
	{
		Print(text);
	}
};

class ExpansionPersonalStorageMenuController: ExpansionViewController
{
	string AllItemsCount;
	string StoredItemsCount;
	string PersonalStorageName;
	string PersonalStorageIcon;
	ref ObservableCollection<ref ExpansionPersonalStorageMenuItem> PlayerItems = new ObservableCollection<ref ExpansionPersonalStorageMenuItem>(this);
	ref ObservableCollection<ref ExpansionPersonalStorageMenuCategoryElement> BrowseCategories = new ObservableCollection<ref ExpansionPersonalStorageMenuCategoryElement>(this);
	ref ObservableCollection<ref ExpansionPersonalStorageMenuInventoryCategoryElement> PlayerCategories = new ObservableCollection<ref ExpansionPersonalStorageMenuInventoryCategoryElement>(this);
};