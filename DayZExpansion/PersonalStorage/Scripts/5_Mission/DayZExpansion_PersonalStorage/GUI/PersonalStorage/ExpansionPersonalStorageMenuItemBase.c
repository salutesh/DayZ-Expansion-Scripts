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

	protected EntityAI m_Object;
	protected ButtonWidget item_button;
	protected TextWidget item_name_text;

#ifdef EXPANSIONMODHARDLINE
	protected int m_RarityColor = -1;
#endif

	void ExpansionPersonalStorageMenuItemBase()
	{
		Class.CastTo(m_PersonalStorageMenuItemBaseController, GetController());
		Class.CastTo(m_PersonalStorageModule, CF_ModuleCoreManager.Get(ExpansionPersonalStorageModule));
	}

	void ~ExpansionPersonalStorageMenuItemBase()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();

		if (m_Object)
			GetGame().ObjectDelete(m_Object);
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
};

class ExpansionPersonalStorageMenuItemBaseController: ExpansionViewController
{
	Object Preview;
	string ItemName;
};