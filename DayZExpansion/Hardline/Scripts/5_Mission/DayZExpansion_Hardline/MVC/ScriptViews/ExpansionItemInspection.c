modded class ExpansionItemInspectionBase
{
	override void UpdateItemStats()
	{
		super.UpdateItemStats();

		if (GetExpansionSettings().GetHardline().EnableItemRarity)
			UpdateItemRarity();
	}

	protected void UpdateItemRarity()
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, m_Item))
			UpdateItemRarity(itemBase.Expansion_GetRarity());
	}

	void UpdateItemRarity(ExpansionHardlineItemRarity rarity)
	{
		if (rarity == ExpansionHardlineItemRarity.NONE)
			return;

		string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
		string text = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
		int color;
		ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
        ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
        m_ItemInspectionController.ItemElements.Insert(element);
	}
};