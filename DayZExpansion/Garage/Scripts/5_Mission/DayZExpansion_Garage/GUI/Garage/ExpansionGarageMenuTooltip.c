/**
 * ExpansionGarageMenuTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenuTooltip: ExpansionScriptView
{
	ref ExpansionGarageMenuTooltipController m_TooltipController;

	protected ImageWidget tooltip_icon;
	protected Widget tooltip_header;
	protected Widget tooltip_content;
	protected HtmlWidget tooltip_title;
	protected HtmlWidget tooltip_text;
	protected WrapSpacerWidget spacer_content;
	protected float m_ContentOffsetX;
	protected float m_ContentOffsetY;

	void ExpansionGarageMenuTooltip()
	{
		if (!m_TooltipController)
			m_TooltipController = ExpansionGarageMenuTooltipController.Cast(GetController());
	}

	void ~ExpansionGarageMenuTooltip()
	{
		m_TooltipController.TooltipEntries.Clear();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Garage/GUI/layouts/expansion_garage_menu_tooltip.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionGarageMenuTooltipController;
	}

	void SetTitle(string title)
	{
		m_TooltipController.TooltipTitle = title;
		m_TooltipController.NotifyPropertyChanged("TooltipTitle");
	}

	void SetText(string text)
	{
		m_TooltipController.TooltipText = text;
		m_TooltipController.NotifyPropertyChanged("TooltipText");
	}

	void ClearEntries()
	{
		m_TooltipController.TooltipEntries.Clear();
	}

	void AddEntry(string text, int color)
	{
		ExpansionGarageMenuTooltipEntry entry = new ExpansionGarageMenuTooltipEntry(text, color);
		m_TooltipController.TooltipEntries.Insert(entry);
	}

	void SetContentOffset(float x, float y)
	{
		m_ContentOffsetX = x;
		m_ContentOffsetY = y;
	}

	override void OnHide()
	{
		tooltip_content.Show(false);
		spacer_content.Show(false);

		for (int i = 0; i < m_TooltipController.TooltipEntries.Count(); i++)
		{
			ExpansionGarageMenuTooltipEntry entry = m_TooltipController.TooltipEntries[i];
			entry.Hide();
		}
	}

	override void OnShow()
	{
		super.OnShow();

		if (m_TooltipController.TooltipText != "")
		{
			tooltip_content.Show(true);
			tooltip_text.Show(true);
		}

		if (m_TooltipController.TooltipEntries.Count() > 0)
		{
			tooltip_content.Show(true);
			spacer_content.Show(true);

			for (int i = 0; i < m_TooltipController.TooltipEntries.Count(); i++)
			{
				ExpansionGarageMenuTooltipEntry entry = m_TooltipController.TooltipEntries[i];
				entry.Show();
			}
		}

		int x, y;
		GetGame().GetMousePos(x, y);
		int w, h;
		GetScreenSize(w, h);
		GetLayoutRoot().SetPos(x + w * m_ContentOffsetX, y + h * m_ContentOffsetY, true);

		GetLayoutRoot().Update();
	}
};

class ExpansionGarageMenuTooltipController: ExpansionViewController
{
	string TooltipTitle;
	string TooltipText;
	ref ObservableCollection<ref ExpansionGarageMenuTooltipEntry> TooltipEntries = new ObservableCollection<ref ExpansionGarageMenuTooltipEntry>(this);
};