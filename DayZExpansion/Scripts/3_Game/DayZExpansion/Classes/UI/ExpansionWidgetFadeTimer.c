
/**
 * ExpansionWidgetFadeTimer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionWidgetFadeTimer
 * @brief		
 **/
class ExpansionWidgetFadeTimer extends TimerBase
{
	//! Expansion reference to widget
	private Widget m_Widget;

	//! Expansion fade in switch
	bool m_FadeIn;

	//! Expansion fade in alpha
	float m_Alpha;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionWidgetFadeTimer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::ExpansionWidgetFadeTimer - Start");
		#endif
		
		OnInit( CALL_CATEGORY_GUI );

		m_FadeIn = true;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::ExpansionWidgetFadeTimer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionWidgetFadeTimer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::~ExpansionWidgetFadeTimer - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::~ExpansionWidgetFadeTimer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion FadeIn
	// ------------------------------------------------------------
	void FadeIn(Widget w, float time, bool continue_ = false)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::FadeIn - Start");
		#endif
		
		float alpha = w.GetAlpha();
		
		if (continue_ && alpha > 0.95)
		{
			w.SetAlpha(0.1);
			w.Show(true);
			return;
		}	
		
		m_Widget = w;
		m_FadeIn = true;
	
		OnStart(time, false);
	
		if (m_Widget)
		{
			alpha = m_Widget.GetAlpha();
			m_Widget.SetAlpha(0);
			m_Widget.Show(true);
		}
	
		if (continue_)
		{
			m_time = alpha * time;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::FadeIn - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion FadeOut
	// ------------------------------------------------------------
	void FadeOut(Widget w, float time, bool continue_ = false)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::FadeOut - Start");
		#endif
		
		m_Alpha = w.GetAlpha();
		
		if (continue_ && m_Alpha < 0.05)
		{
			w.SetAlpha(0);
			w.Show(false);
			return;
		}	
		
		m_Widget = w;
		m_FadeIn = false;
	
		OnStart(time, false);
		
		if (m_Widget && !continue_)
		{
			m_Alpha = 1;
			m_Widget.SetAlpha(m_Alpha);
			m_Widget.Show(true);
		}	
	
		if (continue_)
		{
			m_time = (1 - m_Alpha) * time;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::FadeOut - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnTimer
	// ------------------------------------------------------------
	override private void OnTimer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::OnTimer - Start");
		#endif
		
		if (m_Widget)
		{
			if (m_FadeIn)
			{
				m_Widget.SetAlpha(1);
			}
			else
			{
				m_Widget.SetAlpha(0);
				m_Widget.Show(false);
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::OnTimer - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override private void OnUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::OnUpdate - Start");
		#endif
		
		float m_TimeDiff = m_time / m_duration;

		if ( m_Widget )
		{
			if ( m_FadeIn )
			{
				m_Widget.SetAlpha( m_TimeDiff );
			}
			else
			{
				m_Widget.SetAlpha( m_Alpha - m_TimeDiff );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionWidgetFadeTimer::OnUpdate - End");
		#endif
	}
}