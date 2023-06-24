/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_ISLAND
modded class MainMenu
{
	protected ImageWidget m_DayZLogo;
	protected ImageWidget m_ExLogo;
	protected Widget m_ExSeperatorLine;
	
	override Widget Init()
	{
		layoutRoot = super.Init();

		m_DayZLogo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		float posLogoX, posLogoY;
		m_DayZLogo.GetScreenPos(posLogoX, posLogoY);
		m_DayZLogo.Show(false);
		
		m_ExSeperatorLine = layoutRoot.FindAnyWidget("play_panel_separator");
		m_ExSeperatorLine.SetColor(ARGB(255, 63, 212, 252));
		
		m_ExLogo = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZExpansion/NamalskAdventure/GUI/layouts/logo.layout", layoutRoot));
		m_ExLogo.SetScreenPos(posLogoX, posLogoY, true);
		m_ExLogo.LoadImageFile(0, "DayZExpansion/NamalskAdventure/GUI/logo.edds");

		return layoutRoot;
	}
	
	override void ColorHighlight(Widget w)
	{
		if (!w)
			return;
		
		int color_pnl = ARGB(255, 0, 0, 0);
		int color_lbl = ARGB(255, 63, 212, 252);
		int color_img = ARGB(255, 63, 212, 252);
		
		ButtonSetColor(w, color_pnl);
		ButtonSetTextColor(w, color_lbl);
		ImagenSetColor(w, color_img);
	}
};
#endif
