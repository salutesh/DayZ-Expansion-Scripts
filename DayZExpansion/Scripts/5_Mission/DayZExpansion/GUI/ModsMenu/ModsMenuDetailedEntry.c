/**
 * ModsMenuDetailedEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModsMenuDetailedEntry
{
	override void LoadData()
	{
		super.LoadData();

		string picture = m_Data.GetPicture();
		string logo = m_Data.GetLogoSmall();

		if (picture == "" && logo == "")
		{
			m_IconBig.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
			m_IconSmall.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
		}
	}
};