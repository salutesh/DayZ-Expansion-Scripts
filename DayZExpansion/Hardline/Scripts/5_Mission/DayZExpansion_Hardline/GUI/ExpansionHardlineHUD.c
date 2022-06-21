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
	
	private Widget HumanityAddVal;
	private Widget HumanityRemoveVal;

	void ExpansionHardlineHUD()
	{
		if (!m_HardlineHUDController)
			m_HardlineHUDController = ExpansionHardlineHUDController.Cast(GetController());

		if (!m_HardlineModule)
			m_HardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
	}

	void SetView(ExpansionHardlinePlayerData data)
	{
		if (!data)
			return;
		
		int humanity = data.GetHumanity();
		//! If humanity has changed show indicator
		if (humanity != m_CurrentHumanity)
		{
			int difference;
			if (m_CurrentHumanity < 0)
			{
				if (humanity < 0)
				{
					difference = m_CurrentHumanity + humanity;
					m_CurrentHumanity =- humanity;
				}
				else if (humanity > 0)
				{
					difference = m_CurrentHumanity - humanity;
					m_CurrentHumanity += humanity;
				}
			}
			else if (m_CurrentHumanity > 0)
			{
				if (humanity < 0)
				{
					difference = m_CurrentHumanity - humanity;
				}
				else if (humanity > 0)
				{
					difference = m_CurrentHumanity + humanity;
				}
			}
			
			if (difference < 0)
			{
				RemoveHumanity(difference);
			}
			else if (difference > 0)
			{
				AddHumanity(difference);
			}
		}
		
		string path = GetHumanityIcon(humanity);
		
		m_HardlineHUDController.HumanityVal = humanity.ToString();
		m_HardlineHUDController.HumanityIcon = path;

		m_HardlineHUDController.NotifyPropertiesChanged({"HumanityVal", "HumanityIcon"});
	}

	string GetHumanityIcon(int humanity)
	{
		string path = ExpansionIcons.GetPath("Persona");
		//! If player is hero
		if (humanity > 1000)
		{
			if (humanity > 1000)
			{
				path = "set:expansion_notification_iconset image:icon_hero_1";
			}
			
			if (humanity > 2000)
			{
				path = "set:expansion_notification_iconset image:icon_hero_2";
			}
			
			if (humanity > 3000)
			{
				path = "set:expansion_notification_iconset image:icon_hero_3";
			}
			
			if (humanity > 4000)
			{
				path = "set:expansion_notification_iconset image:icon_hero_4";
			}
			
			if (humanity > 5000)
			{
				path = "set:expansion_notification_iconset image:icon_hero_5";
			}
		}
		//! If player is badit
		else if (humanity < -1000)
		{
			if (humanity < -1000)
			{
				path = "set:expansion_notification_iconset image:icon_bandit_1";
			}
			
			if (humanity < -2000)
			{
				path = "set:expansion_notification_iconset image:icon_bandit_2";
			}
			
			if (humanity < -3000)
			{
				path = "set:expansion_notification_iconset image:icon_bandit_3";
			}
			
			if (humanity < -4000)
			{
				path = "set:expansion_notification_iconset image:icon_bandit_4";
			}
			
			if (humanity < -5000)
			{
				path = "set:expansion_notification_iconset image:icon_bandit_5";
			}
		}
		
		return path;
	}
	
	void AddHumanity(int difference)
	{
		
	}
	
	void RemoveHumanity(int difference)
	{
		
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
		return 2.0;
	}
	
	override void Update()
	{
		if (m_HardlineModule && GetExpansionSettings().GetHardline().UseHumanity)
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
	
	string HumanityAddVal;
	string HumanityRemoveVal;
};