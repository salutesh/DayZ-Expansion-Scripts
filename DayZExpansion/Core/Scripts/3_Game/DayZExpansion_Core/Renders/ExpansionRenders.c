/**
 * ExpansionRenders.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCircle
{
    float Radius;
    float X;
    float Y;
    float Width;
    int Color;
	
	void ExpansionCircle(float rad, float x, float y, float width, float color)
	{
		Radius = rad;
    	X = x;
  	  	Y = y;
   		Width = width;
    	Color = color;
	}
};


class ExpansionCircleRender
{
	//Widget m_LayoutCanvas;
	CanvasWidget m_CanvasWidget;
 	const int SEGMENTS = 180;
    ref array<ref ExpansionCircle> m_Circles = new array<ref ExpansionCircle>();
	Widget m_Parent;
	MapWidget m_MapWidget;	
	
	void ExpansionCircleRender(Widget parent)
	{
		m_Parent = parent;
	}
	
	void ~ExpansionCircleRender()
	{
		m_CanvasWidget.Clear();
		m_Circles.Clear();
	}
	
	void AddCircle(ExpansionCircle cirlce)
	{
		m_Circles.Insert(cirlce);
	}
	
    void Render(MapWidget mapWidget)
    {	
		m_MapWidget = mapWidget;
		Widget layoutCanvas = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Core/GUI/layouts/map/expansion_map_canvaslayout.layout", m_Parent);
		m_CanvasWidget = CanvasWidget.Cast( layoutCanvas.FindAnyWidget( "CanvasWidget" ) );
		
        for (int i = 0; i < m_Circles.Count(); i++)
        {		
            RenderCircle(m_Circles[i]);
        }
    }

    bool CircleOverlap(ExpansionCircle a, ExpansionCircle b)
    {
        float xx = (a.X - b.X);
        float yy = (a.Y - b.Y);
        float rr = a.Radius + b.Radius;
        return ((xx * xx) + (yy * yy)) < (rr * rr);
    }

    bool IsPointInCircle(array<ExpansionCircle> overlaps, float x, float y)
    {
        for (int i = 0; i < overlaps.Count(); i++)
        {
            float xx = (overlaps[i].X - x);
            float yy = (overlaps[i].Y - y);
            float rr = overlaps[i].Radius;
            if (((xx * xx) + (yy * yy)) < (rr * rr)) return true;
        }
        return false;
    }

    void RenderCircle(ExpansionCircle circle)
    {		
		m_CanvasWidget.Clear();
		
		array<ExpansionCircle> overlaps = new array<ExpansionCircle>();
        for (int i = 0; i < m_Circles.Count(); i++)
        {
            if (m_Circles[i] == circle) continue;

            if (CircleOverlap(circle, m_Circles[i]))
                overlaps.Insert(m_Circles[i]);
        }
	
        for (int a = 0; a < SEGMENTS; a++)
        {
            float stride = 360.0 / SEGMENTS;
			
		    float a1 = Math.DEG2RAD * a * stride;
		    float a2 = Math.DEG2RAD * (a + SEGMENTS) * stride;
		
		    float x1 = circle.X + Math.Sin(a1) * circle.Radius;
		    float y1 = circle.Y + Math.Cos(a1) * circle.Radius;
		
		    float x2 = circle.X + Math.Sin(a2) * circle.Radius;
		    float y2 = circle.Y + Math.Cos(a2) * circle.Radius;

            //if (IsPointInCircle(overlaps, x1, y1)) continue;

			vector map_pos1 = m_MapWidget.ScreenToMap(Vector(x1, y1, 0));
			vector map_pos2 = m_MapWidget.ScreenToMap(Vector(x2, y2, 0));
			
            m_CanvasWidget.DrawLine(map_pos1[0], map_pos1[2], map_pos2[0], map_pos2[2], circle.Width, circle.Color);
        }
    }
};