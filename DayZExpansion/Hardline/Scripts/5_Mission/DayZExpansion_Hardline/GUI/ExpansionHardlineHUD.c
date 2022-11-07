/**
 * ExpansionIngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHardlineHUD: ExpansionScriptView
{
	private ref ExpansionHardlineHUDController m_HardlineHUDController;
	private ExpansionHardlineModule m_HardlineModule;
	private int m_CurrentHumanity;
	
	private Widget Humanity;
	private TextWidget HumanityVal;
	private Widget HumanityChange;
	private TextWidget HumanityChangeVal;
	
	private ref WidgetFadeTimer	m_HumanityChangeFadeTimer;
	private ref Timer m_HumanityChangeTimer;
	private ref Timer m_HumanityChangeFadeOut;
	private bool m_ViewInit = false;
	private float m_StartPosX, m_StartPosY;
	private float m_LastYPos = 0;
	private bool m_FadedOut = false;
	
	void ExpansionHardlineHUD()
	{
		m_HardlineHUDController = ExpansionHardlineHUDController.Cast(GetController());
		m_HardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
		m_HumanityChangeFadeTimer = new WidgetFadeTimer;
		m_HumanityChangeTimer = new Timer(CALL_CATEGORY_GUI);
		m_HumanityChangeFadeOut = new Timer(CALL_CATEGORY_GUI);
		Hide();  //! Initial hidden so it doesn't show on loading screen
	}

	void SetView(ExpansionHardlinePlayerData data)
	{
		if (!data)
			return;
		
		if (!m_ViewInit)
		{		
			Humanity.Show(true);
			HumanityChange.GetPos(m_StartPosX, m_StartPosY);
			m_ViewInit = true;
		}
		
		int humanity = data.GetReputation();		
		int difference;
		
		if (humanity != m_CurrentHumanity )
		{
			difference = humanity - m_CurrentHumanity;
			m_CurrentHumanity = humanity;
		
			OnHumanityChangeReset();
			OnHumanityChange(difference);
			
			m_HardlineHUDController.HumanityVal = Math.RandomInt(0, humanity).ToString();
			m_HardlineHUDController.NotifyPropertyChanged("HumanityVal");
		}
		else
		{
			m_HardlineHUDController.HumanityVal = humanity.ToString();
			m_HardlineHUDController.NotifyPropertyChanged("HumanityVal");
			
			m_HardlineHUDController.HumanityIcon = ExpansionIcons.GetPath("Persona");
			m_HardlineHUDController.NotifyPropertyChanged("HumanityIcon");
		}
	}

	void OnHumanityChange(int difference)
	{	
		string text;
		if (difference > 0)
		{
			text = "+" + difference.ToString();
			HumanityChangeVal.SetColor(ARGB(200, 46, 204, 113));
		}
		else
		{
			text = difference.ToString();
			HumanityChangeVal.SetColor(ARGB(200, 231, 76, 60));
		}
		
		m_HardlineHUDController.HumanityChangeValue = text;
		m_HardlineHUDController.NotifyPropertyChanged("HumanityChangeValue");
		m_HumanityChangeFadeTimer.FadeIn(HumanityChangeVal, 3.0, true);
		m_HumanityChangeTimer.Run(0.01, this, "UpdateHumanityChange", NULL, true);
	}
		
	void UpdateHumanityChange()
	{
		float finalXPos, finalYPos; 
		HumanityVal.GetPos(finalXPos, finalYPos);
		
		float x, y;
		HumanityChange.GetPos(x, y);		
		m_LastYPos = y;
		
		if (m_LastYPos > finalYPos)
		{
			float newY = m_LastYPos - 1.0;	
			HumanityChange.SetPos(x, newY, true);
			
			m_HardlineHUDController.HumanityVal = Math.RandomInt(0, m_CurrentHumanity).ToString();
			m_HardlineHUDController.NotifyPropertyChanged("HumanityVal");
		}
		else if (m_LastYPos <= finalYPos)
		{
			HumanityVal.Show(false);
			m_HumanityChangeTimer.Stop();
			m_HumanityChangeTimer.Run(1.0, this, "OnHumanityChangeFadeOut");
		}
	}
	
	void OnHumanityChangeFadeOut()
	{
		m_HumanityChangeFadeTimer.FadeOut(HumanityChangeVal, 3.0, true);
		m_HumanityChangeFadeOut.Run(3.0, this, "OnHumanityChangeReset");
	}
	
	void OnHumanityChangeReset()
	{
		m_HumanityChangeTimer.Stop();
		m_HumanityChangeFadeOut.Stop();
		
		HumanityChangeVal.SetAlpha(0);
		HumanityChange.SetPos(m_StartPosX, m_StartPosY);

		HumanityVal.Show(true);
		
		m_HardlineHUDController.HumanityVal = m_CurrentHumanity.ToString();
		m_HardlineHUDController.NotifyPropertyChanged("HumanityVal");
	}
	
	override typename GetControllerType()
	{
		return ExpansionHardlineHUDController;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Hardline/GUI/layouts/expansion_hardline_hud.layout";
	}

	void ShowHud(bool state)
	{
		if (state)
		{
			Show();
		}
		else
		{
			Hide();
		}
	}
	
	override float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	override void Update()
	{
		if (m_HardlineModule && GetExpansionSettings().GetHardline(false).IsLoaded() && GetExpansionSettings().GetHardline().UseReputation && GetExpansionSettings().GetHardline().ShowHardlineHUD)
		{
			if (!m_HardlineModule.GetHardlineClientData())
				return;
			
			SetView(m_HardlineModule.GetHardlineClientData());
		}
	}
};

class ExpansionHardlineHUDController: ExpansionViewController
{
	string HumanityVal;
	string HumanityIcon;
	string SanityVal;
	string SanityIcon;	
	string HumanityChangeValue;
};