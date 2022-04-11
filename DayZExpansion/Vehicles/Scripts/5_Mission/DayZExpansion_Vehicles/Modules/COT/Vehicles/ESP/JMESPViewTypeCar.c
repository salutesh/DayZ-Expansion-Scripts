/**
 * JMESPViewTypeCar.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
modded class JMESPViewTypeCar
{
	void JMESPViewTypeCar()
	{
		Permission = "Car";
		Localisation = "#STR_COT_ESP_MODULE_VIEW_TYPE_Car";

		MetaType = JMESPMetaCar;

		Colour = ARGB( 255, 255, 109, 237 );
	}
};
#endif
