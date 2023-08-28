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

	protected EntityAI m_Object;
	protected ButtonWidget item_button;
	protected TextWidget item_name_text;

#ifdef EXPANSIONMODHARDLINE
	protected int m_RarityColor = -1;
#endif

	void ExpansionP2PMarketMenuItemBase()
	{
		Class.CastTo(m_P2PMarketMenuItemBaseController, GetController());
		Class.CastTo(m_P2PMarketModule, CF_ModuleCoreManager.Get(ExpansionP2PMarketModule));
	}

	void ~ExpansionP2PMarketMenuItemBase()
	{
		if (m_ItemTooltip)
			m_ItemTooltip.Destroy();

		if (m_Object)
			GetGame().ObjectDelete(m_Object);
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
};

class ExpansionP2PMarketMenuItemBaseController: ExpansionViewController
{
	Object Preview;
	string ItemName;
};