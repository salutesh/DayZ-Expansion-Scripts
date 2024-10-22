/**
 * MissionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBase
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = NULL;

		switch (id)
		{
		case MENU_MAP:
			auto settings = GetExpansionSettings().GetMap();
			if (settings.EnableMap && settings.UseMapOnMapItem)
			{
				PlayerBase pb;
				if (Class.CastTo(pb, GetGame().GetPlayer()))
				{
					ActionBase action = pb.GetActionManager().GetRunningAction();
					if (action && action.Type() == ActionUnfoldMap)
						menu = new ExpansionMapMenu;
				}
			}
			break;
		case MENU_EXPANSION_MAP:
			menu = new ExpansionMapMenu;
			break;
		}

		if (menu)
		{
			menu.SetID(id);
		}
		else
		{
			menu = super.CreateScriptedMenu(id);
		}

		return menu;
	}
};
