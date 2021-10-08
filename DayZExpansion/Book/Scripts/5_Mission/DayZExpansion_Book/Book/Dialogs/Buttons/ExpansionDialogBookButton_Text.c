/**
 * ExpansionDialogButton_Text.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDialogBookButton_Text: ExpansionDialogButton_Text
{
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/dialogs/expansion_dialog_button_book.layout";
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case dialog_button:
			SetTextColor(ARGB(255,255,255,255));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case dialog_button:
			SetTextColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}