/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
modded class MissionGameplay
{
	protected bool m_Expansion_QuestHUDTogglePressed;
	protected ref ExpansionQuestHUD m_ExpansionQuestHUD;
	protected bool m_Expansion_HideQuestHUD = false;

	//! Quest menu check
	protected bool m_Expansion_QuestMenuTogglePressed;

	override void OnInit()
	{
		//! Expansion Quest Hud
		InitExpansionQuestHud();

		super.OnInit();
	}

	void InitExpansionQuestHud()
	{
		if (!m_ExpansionQuestHUD)
		{
			m_ExpansionQuestHUD = new ExpansionQuestHUD();
			m_ExpansionQuestHUD.Show();
		}
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		if (m_ExpansionQuestHUD)
		{
			bool isCOTOpen;
		#ifdef JM_COT
			isCOTOpen = GetCommunityOnlineTools().IsOpen();
		#endif

			bool show = isAliveConscious && !menu && !viewMenu && !m_Expansion_HideQuestHUD && !isCOTOpen;
			if (m_ExpansionQuestHUD.IsVisible() != show)
				m_ExpansionQuestHUD.ShowHud(show);
		}

		if (isAliveConscious && !inputIsFocused)
		{
			if (m_ExpansionQuestHUD && !menu)
			{
				//! Toggle Quest HUD
				if (input.LocalPress("UAExpansionQuestToggle", false) && !m_Expansion_QuestHUDTogglePressed)
				{
					m_Expansion_QuestHUDTogglePressed = true;
					ToggleQuestHUD();
				}
				else if (input.LocalRelease("UAExpansionQuestToggle", false) || input.LocalValue("UAExpansionQuestToggle", false) == 0)
				{
					m_Expansion_QuestHUDTogglePressed = false;
				}
			}

			ExpansionQuestMenu questMenu = ExpansionQuestMenu.Cast(viewMenu);
			if (questMenu || !menu)
			{
				//! Toggle quest menu
				if (input.LocalPress("UAExpansionQuestLogToggle", false) && !m_Expansion_QuestMenuTogglePressed)
				{
					m_Expansion_QuestMenuTogglePressed = true;
					OnQuestTogglePressed();
				}
				else if (input.LocalRelease("UAExpansionQuestLogToggle", false) || input.LocalValue("UAExpansionQuestLogToggle", false) == 0)
				{
					m_Expansion_QuestMenuTogglePressed = false;
				}
			}
		}
	}

	void ToggleQuestHUD()
	{
		m_Expansion_HideQuestHUD = !m_Expansion_HideQuestHUD;
	}

	bool QuestHudState()
	{
		return m_Expansion_HideQuestHUD;
	}

	ExpansionQuestHUD GetQuestHUD()
	{
		return m_ExpansionQuestHUD;
	}

	protected void OnQuestTogglePressed()
	{
		if (!GetExpansionSettings().GetQuest(false).IsLoaded() || !GetExpansionSettings().GetQuest().EnableQuests)
			return;

		ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();	//! Reference to expansion ui manager
		ScriptView menu = uiManager.GetMenu();	//! Reference to current opened script view menu

		ExpansionQuestMenu questMenu = ExpansionQuestMenu.Cast(menu);
		if (!questMenu)
		{
			uiManager.CreateSVMenu("ExpansionQuestMenu");
			questMenu = ExpansionQuestMenu.Cast(uiManager.GetMenu());
			if (questMenu)
			{
				questMenu.SetLogMode(true);
				
				if (m_ExpansionQuestHUD)
					m_ExpansionQuestHUD.UpdateView();
			}
		}
		else if (questMenu && questMenu.IsVisible())
		{
			uiManager.CloseMenu();
		}
	}
};
#endif