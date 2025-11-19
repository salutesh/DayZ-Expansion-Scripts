/**
 * ExpansionMapMarkerPlayerArrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerPlayerArrow : ExpansionMapWidgetBase
{
	void ExpansionMapMarkerPlayerArrow(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		SetIgnorePointer(true);
		SetIcon(EXPANSION_ICON_POSITION);
		SetPlayerArrowColor();
		//! Do NOT show by default, otherwise it'll pop up on the load screen in the top left corner at the end of the loading progress
		Hide();

		GetExpansionClientSettings().SI_UpdateSetting.Insert(SetPlayerArrowColor);
	}

	void ~ExpansionMapMarkerPlayerArrow()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove(SetPlayerArrowColor);
	}

	void SetPlayerArrowColor()
	{
		switch (GetExpansionClientSettings().PlayerArrowColor)
		{
			case ExpansionClientUIPlayerArrowColor.GREY:
				SetPrimaryColor(-8421505);
				break;
			case ExpansionClientUIPlayerArrowColor.BLACK:
				SetPrimaryColor(-14803426);
				break;

			case ExpansionClientUIPlayerArrowColor.RED:
				SetPrimaryColor(-3657166);
				break;
			case ExpansionClientUIPlayerArrowColor.GREEN:
				SetPrimaryColor(-10178716);
				break;
			case ExpansionClientUIPlayerArrowColor.BLUE:
				SetPrimaryColor(-13487416);
				break;

			case ExpansionClientUIPlayerArrowColor.CYAN:
				SetPrimaryColor(-16715536);
				break;
			case ExpansionClientUIPlayerArrowColor.MAGENTA:
				SetPrimaryColor(-1048336);
				break;
			case ExpansionClientUIPlayerArrowColor.YELLOW:
				SetPrimaryColor(-987136);
				break;

			case ExpansionClientUIPlayerArrowColor.WHITE:
			default:
				SetPrimaryColor(-1644826);
				break;
		}
	}

	override void Update(float pDt)
	{
		float x, y;
		vector mapPos = GetMapWidget().MapToScreen(g_Game.GetCurrentCameraPosition());
		GetLayoutRoot().GetParent().GetScreenPos(x, y);
		GetLayoutRoot().SetPos(mapPos[0] - x, mapPos[1] - y, true);
		GetDragWidget().SetRotation(0, 0, GetMapDirection(), true);
	}

	private int GetMapDirection()
	{
		return Math.Round(Math.NormalizeAngle(g_Game.GetCurrentCameraDirection().VectorToAngles()[0]));
	}

	override bool IsEditButton( Widget w )
	{
		return false;
	}

	override bool CanDrag()
	{
		return false;
	}
};