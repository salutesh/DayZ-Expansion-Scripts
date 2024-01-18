/**
 * ExpansionP2PMarketMenuItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuItemBase: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuItemBaseController m_P2PMarketMenuItemBaseController;
	protected ref ExpansionP2PMarketModule m_P2PMarketModule;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected ref ExpansionItemTooltip m_ItemTooltip;
	protected ref ExpansionItemInspection m_ItemInspection;
	protected ref ExpansionP2PMarketListingBase m_Item;

	protected EntityAI m_Object;
	protected ButtonWidget item_button;
	protected TextWidget item_name_text;

#ifdef EXPANSIONMODHARDLINE
	protected int m_RarityColor = -1;
#endif

	void ExpansionP2PMarketMenuItemBase(ExpansionP2PMarketListingBase item, ExpansionP2PMarketMenu menu)
	{
		Class.CastTo(m_P2PMarketMenuItemBaseController, GetController());
		Class.CastTo(m_P2PMarketModule, CF_ModuleCoreManager.Get(ExpansionP2PMarketModule));

		m_P2PMarketMenu = menu;
		m_Item = item;
	}

	void ~ExpansionP2PMarketMenuItemBase()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();

		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuItemBaseController;
	}

#ifdef EXPANSIONMODHARDLINE
	void SetRarityColor(ExpansionHardlineItemRarity rarity)
	{
		if (rarity != ExpansionHardlineItemRarity.NONE)
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

	void SpawnAttachments(array<ref ExpansionP2PMarketContainerItem> attachments, EntityAI parent, int skinIndex = 0)
	{
		foreach (ExpansionP2PMarketContainerItem attachment: attachments)
		{
			string attachmentName = attachment.GetClassName();
			ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, skinIndex);
		}
	}

	EntityAI GetPreviewObject()
	{
		return m_Object;
	}
	
	ExpansionP2PMarketListingBase GetItem()
	{
		return m_Item;
	}

	void UpdatePreviewObject()
	{
		if (!m_Item)
			return;

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

			Construction.ExpansionBuildFullIfSupported(m_Object);

			UpdateFoodStage();
		}

		m_P2PMarketMenuItemBaseController.Preview = m_Object;
		m_P2PMarketMenuItemBaseController.NotifyPropertyChanged("Preview");
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

class ExpansionP2PMarketMenuItemBaseController: ExpansionViewController
{
	Object Preview;
	string ItemName;
};