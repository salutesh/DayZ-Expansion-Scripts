/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	protected int m_Expansion_Rarity = ExpansionHardlineItemRarity.NONE;

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_Expansion_Rarity", EnumTools.GetEnumValue(ExpansionHardlineItemRarity, 0), EnumTools.GetLastEnumValue(ExpansionHardlineItemRarity));

		if (GetGame().IsServer() && GetExpansionSettings().GetHardline().EnableItemRarity)
		{
			m_Expansion_Rarity = GetExpansionSettings().GetHardline().GetItemRarityByType(GetType());

		#ifdef EXPANSIONMODHARDLINEDEBUG
			EXTrace.Print(EXTrace.HARDLINE, this, "- Hardline item rarity: " + typename.EnumToString(ExpansionHardlineItemRarity, m_Expansion_Rarity));
		#endif
		}
	}

	ExpansionHardlineItemRarity Expansion_GetRarity()
	{
		return m_Expansion_Rarity;
	}

	void Expansion_SetRarity(ExpansionHardlineItemRarity rarity)
	{
		m_Expansion_Rarity = rarity;

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

	#ifdef EXPANSIONMODHARDLINEDEBUG
		EXTrace.Print(EXTrace.HARDLINE, this, "- Hardline item rarity: " + typename.EnumToString(ExpansionHardlineItemRarity, m_Expansion_Rarity));
	#endif
	}
};
