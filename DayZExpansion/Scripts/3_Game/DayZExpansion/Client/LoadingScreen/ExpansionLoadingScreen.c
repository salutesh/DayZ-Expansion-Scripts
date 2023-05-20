/**
 * ExpansionLoadingScreen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoadingScreen
{
	void LoadingScreen(DayZGame game)
	{
		if (m_ImageLogoMid)
			m_ImageLogoMid.LoadImageFile(0, "set:expansion_iconset image:logo_expansion_white");
		if (m_ImageLogoCorner)
			m_ImageLogoCorner.LoadImageFile(0, "set:expansion_iconset image:logo_expansion_white");
	}
}
