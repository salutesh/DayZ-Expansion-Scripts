/**
 * ExpansionCOTTerritoriesMapMarker.c
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
class ExpansionCOTTerritoriesMapMarker : ExpansionMapWidgetBase
{
	private JMFormBase m_Menu;
	private ref ExpansionTerritory m_Territory;

	private GridSpacerWidget m_MemberEntries;
	private ButtonWidget m_CancelButton;
	private ButtonWidget m_EditTerritoryButton;

	void ExpansionCOTTerritoriesMapMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
	}

	void SetCOTMenu(JMFormBase menu)
	{
		m_Menu = menu;
	}

	void SetTerritory(ExpansionTerritory territory)
	{
		m_Territory = territory;
	}

	override void Update(float pDt)
	{
		vector mapPos = GetMapWidget().MapToScreen(m_Territory.GetPosition());

		float x;
		float y;

		GetLayoutRoot().GetParent().GetScreenPos(x, y);
		GetLayoutRoot().SetPos(mapPos[0] - x, mapPos[1] - y, true);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w)
		{
		case m_Frame:
			ExpansionCOTTerritoriesMenu menu = ExpansionCOTTerritoriesMenu.Cast(m_Menu);
			if (menu)
			{
				menu.SetTerritoryInfo(m_Territory);
			}
			break;
		}

		return false;
	}
	
	override bool CanDrag()
	{
		return false;
	}
};
#endif
