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
	protected ref ExpansionPersonalStorageMenuCargoItemController m_PersoanlStorageMenuCargoItemController;
	protected ref ExpansionPersonalStorageContainerItem m_ContainerItem;

	void ExpansionPersonalStorageMenuCargoItem(ExpansionPersonalStorageMenu menu, ExpansionPersonalStorageContainerItem containerItem)
	{
		Class.CastTo(m_PersoanlStorageMenuCargoItemController, GetController());

		Print(ToString() + "::ExpansionPersonalStorageMenuCargoItem - Container item: " + containerItem.ToString());
		Print(ToString() + "::ExpansionPersonalStorageMenuCargoItem - Container item class name: " + containerItem.GetClassName());

		m_PersonalStorageMenu = menu;
		m_ContainerItem = containerItem;

		SetView();
	}

	void SetView()
	{
		UpdatePreviewObject();

		string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_ContainerItem.GetClassName());
		string displayText = displayName;
		if (m_ContainerItem.GetContainerItems().Count() > 0)
			displayText = "[+] " + displayName;
		
		if (m_ContainerItem.GetQuantityType() == ExpansionItemQuantityType.PC || m_ContainerItem.GetQuantityType() == ExpansionItemQuantityType.MAGAZINE)
			displayText = displayText + " - [" + Math.Round(m_ContainerItem.GetQuantity()) + "]";
		
		m_PersoanlStorageMenuCargoItemController.ItemName = displayText;
		m_PersoanlStorageMenuCargoItemController.NotifyPropertyChanged("ItemName");

	#ifdef EXPANSIONMODHARDLINE
		SetRarityColor(m_ContainerItem.GetRarity());
	#endif
	}

	void UpdatePreviewObject()
	{
		if (!m_ContainerItem)
			return;

		Print(ToString() + "::UpdatePreviewObject - Container item: " + m_ContainerItem.ToString());
		Print(ToString() + "::UpdatePreviewObject - Container item class name: " + m_ContainerItem.GetClassName());

		string previewClassName = m_PersonalStorageMenu.GetPreviewClassName(m_ContainerItem.GetClassName());
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

			if (m_ContainerItem.GetContainerItems().Count() > 0)
			{
				Print(ToString() + "::UpdatePreviewObject - Attachments count:" + m_ContainerItem.GetContainerItems().Count() + " for item " + previewClassName);
				SpawnAttachments(m_ContainerItem.GetContainerItems(), m_Object, m_ContainerItem.GetSkinIndex());
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
				foodStage.ChangeFoodStage(m_ContainerItem.GetFoodStageType());
			}
		}

		m_PersoanlStorageMenuCargoItemController.Preview = m_Object;
		m_PersoanlStorageMenuCargoItemController.NotifyPropertyChanged("Preview");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_container_item.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuCargoItemController;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && w == item_button)
		{
			if (button == MouseState.MIDDLE && m_Object && m_ContainerItem)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_ContainerItem.GetRarity();
#endif
				MissionGameplay.InspectItem(m_PersonalStorageMenu, m_Object, m_ContainerItem.GetHealthLevel(), m_ContainerItem.GetLiquidType(), m_ContainerItem.IsBloodContainer(), m_ContainerItem.GetQuantityType(), m_ContainerItem.GetQuantity(), m_Object.GetQuantityMax(), m_ContainerItem.GetFoodStageType(), m_ContainerItem.GetClassName(), rarity);
				return true;
			}
		}
		
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && w == item_button)
		{		
			if (!m_ItemTooltip && m_Object && m_ContainerItem)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_ContainerItem.GetRarity();
#endif
				m_ItemTooltip = MissionGameplay.SetItemTooltip(m_Object, m_ContainerItem.GetHealthLevel(), m_ContainerItem.GetLiquidType(), m_ContainerItem.IsBloodContainer(), m_ContainerItem.GetQuantityType(), m_ContainerItem.GetQuantity(), m_Object.GetQuantityMax(), m_ContainerItem.GetFoodStageType(), m_ContainerItem.GetClassName(), rarity);
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

class ExpansionPersonalStorageMenuCargoItemController: ExpansionPersonalStorageMenuItemBaseController {};