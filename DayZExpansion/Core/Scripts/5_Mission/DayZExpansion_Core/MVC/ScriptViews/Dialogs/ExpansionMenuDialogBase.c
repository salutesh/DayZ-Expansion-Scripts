/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialogBase: ExpansionScriptView
{
	WrapSpacerWidget dialog_base;
	WrapSpacerWidget dialog_base_spacer;
	Widget dialog_base_header;
	TextWidget dialog_base_title;
	Widget dialog_base_footer;
	ButtonWidget dialog_close_button;
	Widget dialog_info_content;
	ButtonWidget dialog_info_button;
	GridSpacerWidget dialog_buttons_grid;
	
	ref ExpansionScriptViewMenu m_ParentMenu;
	ref ExpansionMenuDialogBaseController m_DialogBaseController;
		
	void ExpansionMenuDialogBase(ExpansionScriptViewMenu parentMenu)
	{
		m_ParentMenu = parentMenu;
		
		if (!m_DialogBaseController)
			m_DialogBaseController = ExpansionMenuDialogBaseController.Cast(GetController());
		
		SetBaseDialogView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_base.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogBaseController;
	}
	
	string GetDialogTitle()
	{
		return "";
	}
	
	bool HasHeader()
	{
		return false;
	}
	
	bool HasFooter()
	{
		return false;
	}
	
	bool HasCloseButton()
	{
		return false;
	}
	
	bool HasInfoButton()
	{
		return false;
	}
	
	void SetBaseDialogView()
	{
		if (!HasHeader())
			dialog_base_header.Show(false);
		
		if (!HasCloseButton())
			dialog_close_button.Show(false);
		
		if (!HasInfoButton())
			dialog_close_button.Show(false);
		
		if (GetDialogTitle() == "")
		{
			dialog_base_title.Show(false);
		} 
		else
		{
			m_DialogBaseController.DialogTitle = GetDialogTitle();
			m_DialogBaseController.NotifyPropertyChanged("DialogTitle");
		}
		
		if (!HasFooter())
			dialog_base_footer.Show(false);
	}
	
	void AddContent(ExpansionMenuDialogContentBase content)
	{
		m_DialogBaseController.DialogContents.Insert(content);
	}
	
	void AddButton(ExpansionMenuDialogButtonBase button)
	{
		m_DialogBaseController.DialogButtons.Insert(button);
	}
	
	ExpansionScriptViewMenu GetParentMenu()
	{
		return m_ParentMenu;
	}

	void CenterVertically()
	{
		int screen_w, screen_h;
		GetScreenSize(screen_w, screen_h);
		//EXPrint("Screen " + screen_w + " x " + screen_h);
		
		float w, h;
		GetActualSize(dialog_base_spacer, w, h);

		//float x = (screen_w - w) * 0.5 / screen_w;
		//float y = (screen_h - h) * 0.5 / screen_h;
		//EXPrint("Dialog " + x + " " + y + " " + w + " x " + h);

		float x, y;
		dialog_base.GetPos(x, y);
		dialog_base.SetPos(x, -h * 0.5 / screen_h, true);
	}

	//! @note not accurate, just adds heights of all spacer widgets it finds (so only works for vertical layouts),
	//! doesn't (can't) take into account padding at right and bottom
	static void GetActualSize(Widget parent, out float w, out float h)
	{
		float x, y;
		parent.GetScreenPos(x, y);
		parent.GetScreenSize(w, h);
		//EXPrint("GetActualSize " + parent.GetName() + " " + x + " " + y + " " + w + " x " + h);

		float child_x, child_y;
		float child_w, child_h;
		Widget child = parent.GetChildren();
		while (child)
		{
			if (child.IsVisible() && child.IsInherited(SpacerWidget))
			{
				//child.GetScreenPos(child_x, child_y);
				GetActualSize(child, child_w, child_h);
				//if (child_x + child_w > x + w)
					//w = child_x + child_w - x;
				//if (child_y + child_h > y + h)
					//h = child_y + child_h - y;
				h += child_h;
			}

			child = child.GetSibling();
		}
	}
}

class ExpansionMenuDialogBaseController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionMenuDialogContentBase> DialogContents = new ObservableCollection<ref ExpansionMenuDialogContentBase>(this);
	ref ObservableCollection<ref ExpansionMenuDialogButtonBase> DialogButtons = new ObservableCollection<ref ExpansionMenuDialogButtonBase>(this);
	string DialogTitle;
}

class ExpansionMenuDialogButtonBase: ExpansionScriptView 
{
	ref ExpansionMenuDialogBase m_Dialog;
	
	void ExpansionMenuDialogButtonBase(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
	}
	
	ExpansionMenuDialogBase GetDialog()
	{
		return m_Dialog;
	}
};

class ExpansionMenuDialogContentBase: ExpansionScriptView 
{
	ref ExpansionMenuDialogBase m_Dialog;
	
	void ExpansionMenuDialogContentBase(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
	}
	
	ExpansionMenuDialogBase GetDialog()
	{
		return m_Dialog;
	}
};