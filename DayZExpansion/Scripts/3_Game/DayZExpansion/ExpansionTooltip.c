/**
 * ExpansionUITooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// ExpansionUITooltip Constructor
	// ------------------------------------------------------------	
	void ExpansionUITooltip(string text, string title = "")
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::ExpansionUITooltip - Start");
		#endif
		
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_tooltip.layout") );
		m_Title = HtmlWidget.Cast(m_Root.FindAnyWidget("TooltipTitle"));
		m_Seperator = Widget.Cast(m_Root.FindAnyWidget("Separator"));
		m_Text = HtmlWidget.Cast(m_Root.FindAnyWidget("TooltipText"));
		
		m_TooltipTitle = title;
		m_TooltipText = text;

		m_Root.SetHandler( this );

		HideTooltip();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::ExpansionUITooltip - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionUITooltip Deconstructor
	// ------------------------------------------------------------	
	void ~ExpansionUITooltip()
	{
		HideTooltip();
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetToolTip
	// ------------------------------------------------------------	
	void SetToolTip()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetToolTip - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetToolTip - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetPos
	// ------------------------------------------------------------	
	void SetPos(int x, int y)
	{
		m_Root.SetPos(x, y);
	}
	
	// ------------------------------------------------------------
	// Expansion ShowTooltip
	// ------------------------------------------------------------	
	void ShowTooltip()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::ShowTooltip - Start");
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
			m_Root.SetPos(x, y);
		}
		
		m_Text.Update();
		m_Root.Update();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::ShowTooltip - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion HideTooltip
	// ------------------------------------------------------------	
	void HideTooltip()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::HideTooltip - Start");
		#endif
		
		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		if (m_Root.IsVisible())
			m_Root.Show(false);
		
		if (!HasFixPosition())
			m_Root.SetPos(0, 0);
		
		m_Title.SetText("");
		m_Text.SetText("");
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::HideTooltip - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetColor
	// ------------------------------------------------------------		
	void SetColor(int color)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetColor - Start");
		#endif
		
		m_Root.SetColor( color );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetColor - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetColor
	// ------------------------------------------------------------		
	void SetTextColor(int color)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetTextColor - Start");
		#endif
		
		m_Title.SetColor( color );
		m_Text.SetColor( color );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::SetTextColor - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion Update
	// ------------------------------------------------------------	
	void Update( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::Update - Start");
		#endif
		
		if ( m_Root && m_Root.IsVisible() ) 
			UpdateTooltip();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::Update - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateTooltip
	// ------------------------------------------------------------		
	void UpdateTooltip()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::UpdateTooltip - Start");
		#endif
		
		if (m_FixPos)
		{
			int mouse_x;
			int mouse_y;
				
			GetGame().GetMousePos( mouse_x, mouse_y );
			m_Root.SetPos( mouse_x, mouse_y );
			m_Root.Update();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUITooltip::UpdateTooltip - End");
		#endif
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
	
	void SetFixPostion(bool state)
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
}