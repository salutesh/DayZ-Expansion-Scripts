/**
 * ExpansionTooltipServerSettingEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTooltipServerSettingEntry: ExpansionScriptView
{
	string m_Title;
	string m_Text;
	
	RichTextWidget TooltipText;
	
	void ExpansionTooltipServerSettingEntry(string title, string text)
	{
		m_Title = title;
		m_Text = text;
		
		int x, y;
		GetGame().GetMousePos(x, y);
		GetLayoutRoot().SetPos(x, y, true);
		
		SetView();
	}

	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_tooltip_setting.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionTooltipServerSettingEntryController;
	}
	
	void SetView()
	{
		ExpansionTooltipServerSettingEntryController controller = ExpansionTooltipServerSettingEntryController.Cast(GetController());
		if (controller)
		{
			controller.TooltipTitle = m_Title;
			controller.NotifyPropertyChanged("TooltipTitle");
			
			controller.TooltipText = m_Text;
			controller.NotifyPropertyChanged("TooltipText");
		}
		
		TooltipText.Update();
	}
}

class ExpansionTooltipServerSettingEntryController: ExpansionViewController
{
	string TooltipTitle;
	string TooltipText;
}