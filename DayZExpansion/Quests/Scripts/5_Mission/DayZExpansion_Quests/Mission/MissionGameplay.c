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
	protected bool m_HideHUD = false;

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

	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if (GetExpansionSettings().GetQuest(false).IsLoaded() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			//! Checking for keyboard focus
			bool inputIsFocused = false;
			//! Reference to focused windget
			Widget focusedWidget = GetFocus();
			if  (focusedWidget)
			{
				if (focusedWidget.ClassName().Contains("EditBoxWidget"))
				{
					inputIsFocused = true;
				}
				else if (focusedWidget.ClassName().Contains("MultilineEditBoxWidget"))
				{
					inputIsFocused = true;
				}
			}

			Man man = GetGame().GetPlayer(); //! Refernce to man
			Input input = GetGame().GetInput(); //! Reference to input
			UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Expansion reference to menu
			PlayerBase playerPB = PlayerBase.Cast(man); //! Expansion reference to player
			ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
			ExpansionQuestMenu questMenu = ExpansionQuestMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		#ifdef JM_COT
			bool isCOTWindowOpen = GetCommunityOnlineTools().IsOpen();
		#endif

			if (playerPB && playerPB.GetHumanInventory())
			{
				if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
				{
				#ifdef JM_COT
					if (viewMenu || topMenu || m_HideHUD || isCOTWindowOpen)
				#else
					if (viewMenu || topMenu || m_HideHUD)
				#endif
					{
						m_ExpansionQuestHUD.ShowHud(false);
					}
					else
					{
						m_ExpansionQuestHUD.ShowHud(true);
					}

					if ((m_ExpansionQuestHUD || !topMenu) && !inputIsFocused)
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

					if ((questMenu || !topMenu) && !inputIsFocused)
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
				else
				{
					m_ExpansionQuestHUD.ShowHud(false);
				}
			}
		}
	}

	void ToggleQuestHUD()
	{
		m_HideHUD = !m_HideHUD;
	}

	bool QuestHudState()
	{
		return m_HideHUD;
	}

	ExpansionQuestHUD GetQuestHUD()
	{
		return m_ExpansionQuestHUD;
	}

	protected void OnQuestTogglePressed()
	{
		ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();	//! Reference to expansion ui manager
		ScriptView menu = uiManager.GetMenu();	//! Reference to current opened script view menu

		ExpansionQuestMenu questMenu = ExpansionQuestMenu.Cast(menu);
		if (!questMenu)
		{
			uiManager.CreateSVMenu("ExpansionQuestMenu");
			questMenu = ExpansionQuestMenu.Cast(uiManager.GetMenu());
			if (questMenu)
				questMenu.SetLogMode(true);
		}
		else if (questMenu && questMenu.IsVisible())
		{
			uiManager.CloseMenu();
		}
	}
};
#endif