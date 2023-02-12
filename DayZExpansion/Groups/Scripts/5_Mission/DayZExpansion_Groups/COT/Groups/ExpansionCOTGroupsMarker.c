/**
 * ExpansionCOTGroupsMapMarker.c
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
#ifdef EXPANSIONMODNAVIGATION
class ExpansionCOTGroupsMapMarker: ExpansionMapMarker
{
	private bool m_IsEditting = false;
	private ExpansionCOTGroupsMenu m_COTGroupsMenu;
	
	void SetCOTGroupsMenu(ExpansionCOTGroupsMenu menu)
	{
		m_COTGroupsMenu = menu;
	}
	
	override void Update(float pDt)
	{
		if (!m_Dragging)
		{
			float x, y;
			vector position = m_MapWidget.MapToScreen(m_WorldPosition);
			m_LayoutRoot.GetParent().GetScreenPos(x, y); //! Map widget
			m_LayoutRoot.SetPos(position[0] - x, position[1] - y, true);
		}
	}
	
	override void OnMarkerClick()
	{
		m_COTGroupsMenu.EditMarker(this);
	}
		
	override void SetIcon(ExpansionIcon icon)
	{
		super.SetIcon(icon);

		if (!m_CurrentExpansionIcon)
			return;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			m_LayoutRoot.GetPos(m_DragOffsetX, m_DragOffsetY);
			m_DragOffsetX = x;
			m_DragOffsetY = y;
			m_Dragging = true;
			return true;
		}

		return false;
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY);
			return true;
		}
		return false;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY, true);
			m_Dragging = false;
			return true;
		}
		return false;
	}
	
	override void SetPosition(int x, int y, bool performDropEvent = false)
	{
		if (IsCreating())
		{
			m_CreationPosX = x;
			m_CreationPosY = y;
		}
		
		m_LayoutRoot.SetPos(x, y, true);
		vector mapPos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
		SetPosition(mapPos, performDropEvent);
	}

	override void SetPosition(vector position, bool performDropEvent = false)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 1.0;
		
		m_WorldPosition = position;
		vector mPos = m_MapWidget.MapToScreen(m_WorldPosition);
		
		Print("ExpansionCOTGroupsMapMarker::SetPosition - World position: X: " + m_WorldPosition[0] + " | Y: " + m_WorldPosition[2]);
		Print("ExpansionCOTGroupsMapMarker::SetPosition - Screen position: X: " + mPos[0] + " | Y: " + mPos[1]);
		
		m_LayoutRoot.SetPos(mPos[0], mPos[1], true);
		if (performDropEvent)
		{
			OnDrop();
		}
	}
	
	override void FocusOnMarker(bool shouldEdit = false)
	{
		GetMapWidget().SetMapPos(GetPosition());
	}
	
	override bool IsEditting()
	{
		return m_IsEditting;
	}
	
	void SetIsEditting(bool state)
	{
		m_IsEditting = state;
	}
	
	override bool CanDrag()
	{
		return false;
	}
};
#endif
#endif
