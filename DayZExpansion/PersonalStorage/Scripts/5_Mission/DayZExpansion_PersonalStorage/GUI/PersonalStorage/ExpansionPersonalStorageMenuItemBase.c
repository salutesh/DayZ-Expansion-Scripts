/**
 * ExpansionPersonalStorageMenuItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuItemBase: ExpansionScriptView
{
	protected ref ExpansionPersonalStorageMenuItemBaseController m_PersonalStorageMenuItemBaseController;
	protected ref ExpansionPersonalStorageModule m_PersonalStorageModule;
	protected ref ExpansionPersonalStorageMenu m_PersonalStorageMenu;
	protected ref ExpansionItemTooltip m_ItemTooltip;
	protected ref ExpansionPersonalStorageItemBase m_Item;

	protected EntityAI m_Object;
	protected ButtonWidget item_button;
	protected TextWidget item_name_text;

#ifdef EXPANSIONMODHARDLINE
	protected int m_RarityColor = -1;
#endif

	void ExpansionPersonalStorageMenuItemBase(ExpansionPersonalStorageItemBase item, ExpansionPersonalStorageMenu menu)
	{
		Class.CastTo(m_PersonalStorageMenuItemBaseController, GetController());
		Class.CastTo(m_PersonalStorageModule, CF_ModuleCoreManager.Get(ExpansionPersonalStorageModule));

		m_Item = item;
		m_PersonalStorageMenu = menu;
	}

	void ~ExpansionPersonalStorageMenuItemBase()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();

		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuItemBaseController;
	}

#ifdef EXPANSIONMODHARDLINE
	void SetRarityColor(ExpansionHardlineItemRarity rarity)
	{
		if (rarity > ExpansionHardlineItemRarity.NONE)
		{
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			int color;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, color);
			int a, r, g, b;
			ExpansionStatic.IntToARGB(color, a, r, g, b);
			m_RarityColor = ExpansionStatic.ARGBtoInt(255, r, g, b);
			item_name_text.SetColor(m_RarityColor);
		}
	}
#endif

	void SpawnAttachments(map<string, int> attachments, EntityAI parent, int skinIndex = 0)
	{
		foreach (string attachmentName, int attachmentCount: attachments)
		{
			ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, skinIndex);
		}
	}

	void SpawnAttachments(array<ref ExpansionPersonalStorageContainerItem> attachments, EntityAI parent, int skinIndex = 0)
	{
		foreach (ExpansionPersonalStorageContainerItem attachment: attachments)
		{
			string attachmentName = attachment.GetClassName();
			ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, skinIndex);
		}
	}

	EntityAI GetPreviewObject()
	{
		return m_Object;
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

			Construction.ExpansionBuildFullIfSupported(m_Object);

			UpdateFoodStage();
		}

		m_PersonalStorageMenuItemBaseController.Preview = m_Object;
		m_PersonalStorageMenuItemBaseController.NotifyPropertyChanged("Preview");
	}

	void UpdateFoodStage()
	{
		Edible_Base food_item = Edible_Base.Cast(m_Object);
		if (food_item && food_item.HasFoodStage())
		{
			FoodStage foodStage = food_item.GetFoodStage();
			foodStage.ChangeFoodStage(m_Item.GetFoodStageType());
		}
	}
};

class ExpansionPersonalStorageMenuItemBaseController: ExpansionViewController
{
	Object Preview;
	string ItemName;
};