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
	private ref ExpansionQuestHUD m_ExpansionQuestHUD;
	private bool m_HideHUD = false;

	// ------------------------------------------------------------
	// OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		//! Expansion Quest Hud
		InitExpansionQuestHud();

		super.OnInit();
	}

	// ------------------------------------------------------------
	// InitExpansionQuestHud
	// ------------------------------------------------------------
	void InitExpansionQuestHud()
	{
		if (!m_ExpansionQuestHUD)
		{
			m_ExpansionQuestHUD = new ExpansionQuestHUD();
			m_ExpansionQuestHUD.Show();
		}
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if ( !m_bLoaded )
		{
			return;
		}

		//! Checking for keyboard focus
		bool inputIsFocused = false;

		//! Reference to focused windget
		Widget focusedWidget = GetFocus();

		if ( focusedWidget )
		{
			if ( focusedWidget.ClassName().Contains( "EditBoxWidget" ) )
			{
				inputIsFocused = true;
			}
			else if ( focusedWidget.ClassName().Contains( "MultilineEditBoxWidget" ) )
			{
				inputIsFocused = true;
			}
		}

		Man man = GetGame().GetPlayer(); //! Refernce to man
		Input input = GetGame().GetInput(); //! Reference to input
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Expansion reference to menu
		PlayerBase playerPB = PlayerBase.Cast( man ); //! Expansion reference to player
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());

		if (playerPB && playerPB.GetHumanInventory())
		{
			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
			{
				if (!m_Hud.GetHudState() && m_HideHUD)
				{
					m_ExpansionQuestHUD.ShowHud(false);
				}
				else if (m_Hud.GetHudState()  && !m_HideHUD)
				{
					m_ExpansionQuestHUD.ShowHud(true);
				}

				if (viewMenu || topMenu || m_HideHUD)
				{
					m_ExpansionQuestHUD.ShowHud(false);
				}
				else
				{
					m_ExpansionQuestHUD.ShowHud(true);
				}
			}
			else
			{
				m_ExpansionQuestHUD.ShowHud(false);
			}
		}
	}

	void ToggleQuestHUD()
	{
		Print("MissionGameplay::ToggleQuestHUD - Start");

		m_HideHUD = !m_HideHUD;

		Print("MissionGameplay::ToggleQuestHUD - End");
	}

	bool QuestHudState()
	{
		return m_HideHUD;
	}

	ExpansionQuestHUD GetQuestHUD()
	{
		return m_ExpansionQuestHUD;
	}
};
#endif