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
	private int m_CurrentReputation;

	protected Widget Humanity;
	protected TextWidget HumanityVal;
	protected Widget HumanityChange;
	protected TextWidget HumanityChangeVal;

	protected ref WidgetFadeTimer m_HumanityChangeFadeTimer;
	protected ref Timer m_HumanityChangeTimer;
	protected ref Timer m_HumanityChangeFadeOut;
	protected bool m_ViewInit = false;
	protected float m_StartPosX, m_StartPosY;
	protected float m_LastYPos = 0;
	protected bool m_FadedOut = false;

	protected IngameHud m_Hud;

	protected ImageWidget CoveredIndicator;

	void ExpansionHardlineHUD(IngameHud hud)
	{
		m_Hud = hud;

		m_HardlineHUDController = ExpansionHardlineHUDController.Cast(GetController());
		m_HardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
		m_HumanityChangeFadeTimer = new WidgetFadeTimer;
		m_HumanityChangeTimer = new Timer(CALL_CATEGORY_GUI);
		m_HumanityChangeFadeOut = new Timer(CALL_CATEGORY_GUI);
		
		Widget stancePanel = m_Hud.GetStancePanel();
		if (stancePanel)
		{
			stancePanel.ClearFlags(WidgetFlags.CLIPCHILDREN);
			stancePanel.AddChild(CoveredIndicator);
		}

		CoveredIndicator.Show(false);
	}

	void SetView(ExpansionHardlinePlayerData data)
	{
		if (!data)
			return;

		bool viewInit = m_ViewInit;
		if (!m_ViewInit)
		{
			Humanity.Show(true);
			HumanityChange.GetPos(m_StartPosX, m_StartPosY);
			m_ViewInit = true;
		}

		int reputation = data.GetReputation();
		int difference = reputation - m_CurrentReputation;

		if (difference)
		{
			m_CurrentReputation = reputation;

			if (viewInit)
			{
				//! Update (animated)
				OnHumanityChangeReset();
				OnHumanityChange(difference);
			}
			else
			{
				//! Initial (static)
				m_HardlineHUDController.HumanityVal = m_CurrentReputation.ToString();
				m_HardlineHUDController.NotifyPropertyChanged("HumanityVal");
			}
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

			m_HardlineHUDController.HumanityVal = Math.RandomInt(0, m_CurrentReputation).ToString();
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

		m_HardlineHUDController.HumanityVal = m_CurrentReputation.ToString();
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

		bool underRoof = GetExpansionClientSettings().ShowUnderRoofIndicator && MiscGameplayFunctions.IsUnderRoof(GetGame().GetPlayer());
		CoveredIndicator.Show(underRoof);
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