/**
 * ItemManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemManager
{
	protected bool m_Expansion_IsValidLayout;

	void ItemManager(Widget root)
	{
		m_Expansion_IsValidLayout = false;
		ExpansionSettings.SI_Hardline.Insert(Expansion_OnSettingsReceived);
	}
	
	void Expansion_OnSettingsReceived()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this);
		EXTrace.Add(trace, "EnableItemRarity " + GetExpansionSettings().GetHardline().EnableItemRarity);
		EXTrace.Add(trace, "tooltip " + m_TooltipWidget);
#endif

		Expansion_InitHardlineRarityWidget();
	}

	void Expansion_InitHardlineRarityWidget()
	{
		auto settings = GetExpansionSettings().GetHardline(false);

		if (!m_Expansion_IsValidLayout && settings.IsLoaded() && settings.EnableItemRarity && m_TooltipWidget)
		{
			GridSpacerWidget container = GridSpacerWidget.Cast(m_TooltipWidget.FindAnyWidget("GridSpacerWidget0"));
			if (container) //! Make sure the container exists and not any other layout is used that does not have this widget..
			{
				GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_tooltip_entry_rarity.layout", container);
				m_Expansion_IsValidLayout = true;
			}
		}
	}
	
	override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
	{
		super.PrepareTooltip(item, x, y);
		
		Expansion_InitHardlineRarityWidget();

		if (m_Expansion_IsValidLayout)
		{
			InspectMenuNew.Expansion_UpdateItemInfoRarity(m_TooltipWidget, item);
		}
	}
};