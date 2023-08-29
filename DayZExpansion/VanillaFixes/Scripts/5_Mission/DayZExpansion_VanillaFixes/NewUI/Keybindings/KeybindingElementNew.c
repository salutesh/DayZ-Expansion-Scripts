/**
 * KeybindingElementNew.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Fix controller buttons not being removable or reassignable with DayZ 1.18
//! Remove this and the accompanying layout file again when this gets fixed in vanilla DayZ
modded class KeybindingElementNew
{
	override string GetLayoutName()
	{
		return "DayZExpansion/VanillaFixes/GUI/layouts/new_ui/options/keybindings_selectors/keybinding_option.layout";
	}
}
