/**
 * ModsMenuSimpleEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModsMenuSimpleEntry
{
	override void LoadData()
	{
		super.LoadData();

		string logo = m_Data.GetLogo();
		string logo_over = m_Data.GetLogoOver();

		if (logo == "")
			m_Icon.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");

		if (logo_over == "")
			m_Icon.LoadImageFile(1, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
	}
};