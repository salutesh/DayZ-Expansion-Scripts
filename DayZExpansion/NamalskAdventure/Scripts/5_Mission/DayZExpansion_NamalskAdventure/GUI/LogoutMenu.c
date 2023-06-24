/**
 * LogoutMenu.c
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
modded class LogoutMenu
{
	protected Widget m_SeperatorLine;

	override Widget Init()
	{
		layoutRoot = super.Init();

		m_SeperatorLine = layoutRoot.FindAnyWidget("SeparatorPanel");
		m_SeperatorLine.SetColor(ARGB(255, 63, 212, 252));

		m_bCancel.SetColor(ARGB(255, 63, 212, 252));
		m_bLogoutNow.SetColor(ARGB(255, 63, 212, 252));

		return layoutRoot;
	}
};
#endif
