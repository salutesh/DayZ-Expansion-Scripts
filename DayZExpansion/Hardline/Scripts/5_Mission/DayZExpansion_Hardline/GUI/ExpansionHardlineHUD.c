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
	private int m_CurrentReputation = -1;
	private int m_CurrentReputationChange;
	private int m_DeltaPerTick;

	protected Widget Reputation;
	protected TextWidget ReputationVal;
	protected Widget ReputationChange;
	protected TextWidget ReputationChangeVal;

	protected ref WidgetFadeTimer m_ReputationChangeFadeTimer;
	protected ref Timer m_ReputationChangeTimer;
	protected ref Timer m_ReputationChangeFadeOut;
	protected bool m_ViewInit = false;
	protected float m_StartPosX, m_StartPosY;
	protected float m_DeltaY = 0;
	protected bool m_FadedOut = false;

	protected IngameHud m_Hud;

	protected ImageWidget CoveredIndicator;

	void ExpansionHardlineHUD(IngameHud hud)
	{
		m_Hud = hud;

		m_HardlineHUDController = ExpansionHardlineHUDController.Cast(GetController());
		m_ReputationChangeFadeTimer = new WidgetFadeTimer;
		m_ReputationChangeTimer = new Timer(CALL_CATEGORY_GUI);
		m_ReputationChangeFadeOut = new Timer(CALL_CATEGORY_GUI);
		
		Widget stancePanel = m_Hud.GetStancePanel();
		if (stancePanel)
		{
			stancePanel.ClearFlags(WidgetFlags.CLIPCHILDREN);
			stancePanel.AddChild(CoveredIndicator);
		}

		CoveredIndicator.Show(false);
	}

	void SetView()
	{
		PlayerBase player;
		if (!Class.CastTo(player, GetGame().GetPlayer()))
			return;

		bool viewInit = m_ViewInit;
		if (!m_ViewInit)
		{
			Reputation.Show(true);
			ReputationChange.GetPos(m_StartPosX, m_StartPosY);
			m_ViewInit = true;
		}

		int reputation = player.Expansion_GetReputation();
		if (reputation == -1)  //! Not yet initialized
			return;

		if (m_CurrentReputation != reputation)
		{
			if (viewInit && m_CurrentReputation != -1)
			{
				//! Update (animated)
				OnReputationChangeReset();
				OnReputationChange(reputation);
			}
			else
			{
				//! Initial (static)
				m_HardlineHUDController.ReputationVal = reputation.ToString();
				m_HardlineHUDController.NotifyPropertyChanged("ReputationVal");
				m_CurrentReputationChange = reputation;
			}

			m_CurrentReputation = reputation;
		}
	}

	void OnReputationChange(int reputation)
	{
		int difference = reputation - m_CurrentReputation;
		string text;
		if (difference > 0)
		{
			text = "+" + difference.ToString();
			ReputationChangeVal.SetColor(ARGB(0, 46, 204, 113));
		}
		else
		{
			text = difference.ToString();
			ReputationChangeVal.SetColor(ARGB(0, 231, 76, 60));
		}

		m_HardlineHUDController.ReputationChangeValue = text;
		m_HardlineHUDController.NotifyPropertyChanged("ReputationChangeValue");

		m_ReputationChangeFadeTimer.FadeIn(ReputationChangeVal, 0.25, true);

		int absDiff = Math.AbsInt(reputation - m_CurrentReputationChange);
		if (absDiff > 200)
			m_DeltaPerTick = absDiff / 100;
		else
			m_DeltaPerTick = 1;
		EXTrace.Print(EXTrace.HARDLINE, this, "OnReputationChange " + difference + " " + m_DeltaPerTick);
		m_ReputationChangeTimer.Run(1 / 60.0, this, "UpdateReputationChange", NULL, true);
	}

	void UpdateReputationChange()
	{
		if (m_DeltaY < 28.0)
		{
			m_DeltaY += 1.0;
			ReputationChange.SetPos(m_StartPosX, m_StartPosY - m_DeltaY, true);
		}
		
		if (m_CurrentReputationChange != m_CurrentReputation)
		{
			if (m_CurrentReputationChange < m_CurrentReputation)
			{
				m_CurrentReputationChange += m_DeltaPerTick;
				if (m_CurrentReputationChange > m_CurrentReputation)
					m_CurrentReputationChange = m_CurrentReputation;
			}
			else
			{
				m_CurrentReputationChange -= m_DeltaPerTick;
				if (m_CurrentReputationChange < m_CurrentReputation)
					m_CurrentReputationChange = m_CurrentReputation;
			}

			m_HardlineHUDController.ReputationVal = m_CurrentReputationChange.ToString();
			m_HardlineHUDController.NotifyPropertyChanged("ReputationVal");
		}
		else if (ReputationChangeVal.GetAlpha() > 0.95)
		{
			m_ReputationChangeFadeTimer.Stop();
			m_ReputationChangeFadeTimer.FadeOut(ReputationChangeVal, 1.0, true);
			m_ReputationChangeTimer.Stop();
			m_ReputationChangeTimer.Run(1.0, this, "OnReputationChangeReset");
		}
	}

	void OnReputationChangeReset()
	{
		m_ReputationChangeTimer.Stop();
		m_ReputationChangeFadeOut.Stop();

		ReputationChangeVal.SetAlpha(0);
		ReputationChange.SetPos(m_StartPosX, m_StartPosY);
		m_DeltaY = 0.0;
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
		if (IsVisible() == state)
			return;

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

	override void Expansion_Update()
	{
		if (GetExpansionSettings().GetHardline(false).IsLoaded() && GetExpansionSettings().GetHardline().UseReputation && GetExpansionSettings().GetHardline().ShowHardlineHUD)
		{
			SetView();
		}

		if (!GetGame().GetPlayer())
			return;

		bool underRoof = GetExpansionClientSettings().ShowUnderRoofIndicator && MiscGameplayFunctions.IsUnderRoof(GetGame().GetPlayer());
		CoveredIndicator.Show(underRoof);
	}
};

class ExpansionHardlineHUDController: ExpansionViewController
{
	string ReputationVal;
	string ReputationIcon;
	string ReputationChangeValue;
};