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
	private ExpansionCOTGroupsMenu m_COTGroupsMenu;
	
	void ExpansionCOTGroupsMapMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		
	}
	
	void SetCOTGroupsMenu(ExpansionCOTGroupsMenu menu)
	{
		m_COTGroupsMenu = menu;
	}
	
	override void Update(float pDt)
	{
		if (!IsDragging())
		{
			float x, y;
			vector mapPos = GetMapWidget().MapToScreen(GetPosition());
			GetLayoutRoot().GetParent().GetScreenPos(x, y);
			GetLayoutRoot().SetPos(mapPos[0] - x, mapPos[1] - y, true);
		}
	}
	
	override void OnMarkerClick()
	{
		m_COTGroupsMenu.EditMarker(this);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			m_LayoutRoot.GetPos(m_DragOffsetX, m_DragOffsetY);
			
			Print("ExpansionCOTGroupsMapMarker::OnDrag - Layout position: X: " + m_DragOffsetX + " | Y: " + m_DragOffsetY);
			Print("ExpansionCOTGroupsMapMarker::OnDrag - Drag position: X: " + x + " | Y: " + y);
			
			m_DragOffsetX = x - m_DragOffsetX;
			m_DragOffsetY = y - m_DragOffsetY;
			
			Print("ExpansionCOTGroupsMapMarker::OnDrag - Drag offset: X: " + m_DragOffsetX + " | Y: " + m_DragOffsetY);
			
			m_Dragging = true;
			return true;
		}
		return false;
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			Print("ExpansionCOTGroupsMapMarker::OnDragging - Position: X: " + (x - m_DragOffsetX) + " | Y: " + (y - m_DragOffsetY));
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY);
			return true;
		}
		return false;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			Print("ExpansionCOTGroupsMapMarker::OnDrop - Position: X: " + (x - m_DragOffsetX) + " | Y: " + (y - m_DragOffsetY));
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY, true);
			m_Dragging = false;
			return true;
		}
		return false;
	}
	
	override void OnDrop()
	{
		super.OnDrop();

		
	}
	
	override void SetPosition(int x, int y, bool performDropEvent = false)
	{
		if (IsCreating())
		{
			m_CreationPosX = x;
			m_CreationPosY = y;
		}
		
		//m_LayoutRoot.SetPos(x, y, true);
		vector newPos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
		SetPosition(newPos, performDropEvent);
	}

	override void SetPosition(vector position, bool performDropEvent = false)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 1.0;
		
		m_WorldPosition = position;
		vector mPos = m_MapWidget.MapToScreen(m_WorldPosition);
		
		Print("ExpansionCOTGroupsMapMarker::SetPosition - World position: X: " + m_WorldPosition[0] + " | Y: " + m_WorldPosition[2]);
		Print("ExpansionCOTGroupsMapMarker::SetPosition - Map position: X: " + mPos[0] + " | Y: " + mPos[2]);
		
		m_LayoutRoot.SetPos(mPos[0], mPos[1], true);
		if (performDropEvent)
		{
			OnDrop();
		}
	}
	
	override bool CanDrag()
	{
		return false;
	}
};
#endif
#endif
