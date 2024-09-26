/**
 * ExpansionScriptViewFadeTimer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionScriptViewFadeTimer: TimerBase
{
	private ExpansionScriptViewBase m_ScriptView;
	bool m_FadeIn;
	bool m_Hide;
	float m_Alpha;

	void ExpansionScriptViewFadeTimer()
	{
		OnInit(CALL_CATEGORY_GUI);
		m_FadeIn = true;
	}

	/**
	\brief "fade in" effect
	@param scriptView which will be faded
	@param time duration of effect
	@param continue - if True continue from current alpha value, otherwise always begin from 0.0 alpha
	*/
	void FadeIn(ExpansionScriptViewBase scriptView, float time, bool continue_ = false)
	{
		m_Alpha = scriptView.GetAlpha();
		
		if (continue_ && m_Alpha > 0.99)
		{
			scriptView.SetAlpha(1.0);
			scriptView.Show(true);
			return;
		}	
		
		m_ScriptView = scriptView;
		m_FadeIn = true;
	
		OnStart(time, false);
	
		if (m_ScriptView && !continue_)
		{
			m_Alpha = 0.0;
			m_ScriptView.SetAlpha(m_Alpha);
			m_ScriptView.Show(true);
		}
	
		if (continue_)
		{
			m_time = m_Alpha * time;
		}
	}

	/**
	\brief "fade out" effect
	@param scriptView which will be faded
	@param time duration of effect
	@param continue - if True continue from current alpha value, otherwise always begin from 1.0 alpha
	*/
	void FadeOut(ExpansionScriptViewBase scriptView, float time, bool continue_ = false, bool hide = true)
	{
		m_Alpha = scriptView.GetAlpha();
		
		if (continue_ && m_Alpha < 0.01)
		{
			scriptView.SetAlpha(0);
			if (hide)
				scriptView.Show(false);
			return;
		}	
		
		m_ScriptView = scriptView;
		m_FadeIn = false;
		m_Hide = hide;
	
		OnStart(time, false);
		
		if (m_ScriptView && !continue_)
		{
			m_Alpha = 1.0;
			m_ScriptView.SetAlpha(m_Alpha);
			m_ScriptView.Show(true);
		}	
	
		if (continue_)
		{
			m_time = (1.0 - m_Alpha) * time;
		}
	}

	override private void OnTimer()
	{
		if (m_ScriptView)
		{
			if (m_FadeIn)
			{
				m_ScriptView.SetAlpha(1);
			}
			else
			{
				m_ScriptView.SetAlpha(0);
				if (m_Hide)
					m_ScriptView.Show(false);
			}
		}
	}

	override private void OnUpdate()
	{
		float timeDiff = m_time / m_duration;
	
		if (m_ScriptView)
		{
			if (m_FadeIn)
			{
				m_ScriptView.SetAlpha(timeDiff);
			}
			else
			{
				m_ScriptView.SetAlpha(m_Alpha - timeDiff);
			}
		}
	}
};
#endif
