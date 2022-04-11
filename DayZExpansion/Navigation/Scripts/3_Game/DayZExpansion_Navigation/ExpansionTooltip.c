/**
 * ExpansionUITooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionUITooltip extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected HtmlWidget			m_Title;
	protected Widget				m_Seperator;
	protected HtmlWidget			m_Text;

	protected string				m_TooltipTitle;
	protected string				m_TooltipText;

	protected ref Timer				m_ToolTipUpdateTimer;
	
	private bool					m_FixPos;
	private int						m_OffsetX;
	private int						m_OffsetY;
	
	// ------------------------------------------------------------
	// ExpansionUITooltip Constructor
	// ------------------------------------------------------------	
	void ExpansionUITooltip(string text, string title = "")
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "ExpansionUITooltip");
#endif

		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_tooltip.layout") );
		m_Title = HtmlWidget.Cast(m_Root.FindAnyWidget("TooltipTitle"));
		m_Seperator = Widget.Cast(m_Root.FindAnyWidget("Separator"));
		m_Text = HtmlWidget.Cast(m_Root.FindAnyWidget("TooltipText"));
		
		m_TooltipTitle = title;
		m_TooltipText = text;

		m_Root.SetHandler( this );

		HideTooltip();
	}
	
	// ------------------------------------------------------------
	// ExpansionUITooltip Deconstructor
	// ------------------------------------------------------------	
	void ~ExpansionUITooltip()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "~ExpansionUITooltip");
#endif

		HideTooltip();
		if ( m_Root )
			delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetToolTip
	// ------------------------------------------------------------	
	void SetToolTip()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "SetToolTip");
#endif

		if (m_TooltipTitle != "")
		{
			m_Title.SetText("<p>" + m_TooltipTitle + "</p>");
		}
		else
		{
			m_Title.Show(false);
			m_Seperator.Show(false);
		}

		m_Text.SetText("<p>" + m_TooltipText + "</p>");
	}

	//! Can be used to offset tooltip. Two special values:
	//! -1 = offset tooltip by its width/height
	//! -2 = negative offset tooltip by its width/height
	void SetOffset( int x, int y )
	{
		m_OffsetX = x;
		m_OffsetY = y;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPos
	// ------------------------------------------------------------	
	void SetPos(int x, int y)
	{
		int offset_x = m_OffsetX, offset_y = m_OffsetY;
		float w, h;

		if ( offset_x < 0 || offset_y < 0 )
			m_Root.GetScreenSize( w, h );

		if ( offset_x == -2 )
			offset_x = -w;
		else if ( offset_x == -1 )
			offset_x = w;

		if ( offset_y == -2 )
			offset_y = -h;
		else if ( offset_y == -1 )
			offset_y = h;

		m_Root.SetPos( x + offset_x, y + offset_y );
	}
	
	// ------------------------------------------------------------
	// Expansion ShowTooltip
	// ------------------------------------------------------------	
	void ShowTooltip()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "ShowTooltip");
#endif

		if (!m_ToolTipUpdateTimer)
		{
			m_ToolTipUpdateTimer = new Timer();
			m_ToolTipUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
		}
		
		m_Root.Show(true);
		
		SetToolTip();
		
		if (!HasFixPosition())
		{
			int x, y;
			GetMousePos(x,y);
			SetPos(x, y);
		}
		
		m_Text.Update();
		m_Root.Update();
	}
	
	// ------------------------------------------------------------
	// Expansion HideTooltip
	// ------------------------------------------------------------	
	void HideTooltip()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "HideTooltip");
#endif

		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		if (m_Root.IsVisible())
			m_Root.Show(false);
		
		if (!HasFixPosition() && m_Root)
			SetPos(0, 0);
		
		m_Title.SetText("");
		m_Text.SetText("");
	}
	
	// ------------------------------------------------------------
	// Expansion SetColor
	// ------------------------------------------------------------		
	void SetColor(int color)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "SetColor");
#endif

		m_Root.SetColor( color );
	}
	
	// ------------------------------------------------------------
	// Expansion SetColor
	// ------------------------------------------------------------		
	void SetTextColor(int color)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "SetTextColor");
#endif

		m_Title.SetColor( color );
		m_Text.SetColor( color );
	}
		
	// ------------------------------------------------------------
	// Expansion Update
	// ------------------------------------------------------------	
	void Update( float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Update");
#endif

		if ( m_Root && m_Root.IsVisible() ) 
			UpdateTooltip();
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateTooltip
	// ------------------------------------------------------------		
	void UpdateTooltip()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "UpdateTooltip");
#endif

		if (m_FixPos)
		{
			int mouse_x;
			int mouse_y;
				
			GetGame().GetMousePos( mouse_x, mouse_y );
			SetPos( mouse_x, mouse_y );
			m_Root.Update();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion IsVisible
	// ------------------------------------------------------------		
	bool IsVisible()
	{
		return m_Root.IsVisible();
	}
	
	void SetParent(Widget parent)
	{
		parent.AddChild(m_Root);
	}
	
	void SetFixPosition(bool state)
	{
		m_FixPos = state;
	}
	
	bool HasFixPosition()
	{
		return m_FixPos;
	}
	
	void SetText(string text)
	{
		m_TooltipText = text;
	}
	
	void SetTextPos(string pos)
	{
		if(pos == "center")
		{
			m_Text.SetFlags(WidgetFlags.CENTER);
		} else if(pos == "right")
		{
			m_Text.SetFlags(WidgetFlags.RALIGN);
		} else if(pos == "left")
		{
			m_Text.ClearFlags(WidgetFlags.RALIGN | WidgetFlags.CENTER);
		}
	}
};
