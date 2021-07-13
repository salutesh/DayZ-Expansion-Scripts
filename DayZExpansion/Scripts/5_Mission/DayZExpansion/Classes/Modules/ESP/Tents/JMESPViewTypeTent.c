/**
 * JMESPViewType.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMESPViewTypeTent
{
	void JMESPViewTypeTent()
	{
		Permission = "Item.Tent";
		Localisation = "#STR_COT_ESP_MODULE_VIEW_TYPE_Tents";

		MetaType = JMESPMetaTents;

		Colour = ARGB( 255, 80, 112, 255 );
	}
};