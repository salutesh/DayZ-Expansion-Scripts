/**
 * MainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class MainMenu
{
	protected DayZIntroSceneExpansion m_Expansion_Scene;

	override Widget Init()
	{
		layoutRoot = super.Init();

		m_Expansion_Scene = m_Mission.GetIntroSceneExpansion();

		ImageWidget dayZLogo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		dayZLogo.LoadImageFile(0, "set:expansion_iconset image:logo_expansion_white");
		dayZLogo.SetFlags(dayZLogo.GetFlags() | WidgetFlags.STRETCH);

		ImageWidget tutorialButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("tutorial_button_image"));
		tutorialButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
		tutorialButtonImage.SetFlags(tutorialButtonImage.GetFlags() | WidgetFlags.STRETCH);

		ImageWidget creditsButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("message_button_image"));
		creditsButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\menu_64x64.edds");
		creditsButtonImage.SetFlags(creditsButtonImage.GetFlags() | WidgetFlags.STRETCH);

		ImageWidget settingsButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("settings_button_image"));
		settingsButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\toothwheels_64x64.edds");
		settingsButtonImage.SetFlags(settingsButtonImage.GetFlags() | WidgetFlags.STRETCH);

		ImageWidget exitButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("exit_button_image"));
		exitButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\switch_64x64.edds");
		exitButtonImage.SetFlags(exitButtonImage.GetFlags() | WidgetFlags.STRETCH);

		Expansion_AdjustButtonsPanel();

		return layoutRoot;
	}

	protected void Expansion_AdjustButtonsPanel()
	{
		float x1, y1;
		WrapSpacerWidget buttonsPanel = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("play_panel_root"));
		buttonsPanel.GetPos(x1, y1);
		float yNew1 = y1 + 32.0;
		buttonsPanel.SetPos(x1, yNew1);

		float x2, y2;
		m_LastPlayedTooltip.GetPos(x2, y2);
		float yNew2 = y2 + 32.0;
		m_LastPlayedTooltip.SetPos(x2, yNew2);
	}

	override void Refresh()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Refresh");
	#endif

		if (m_Expansion_Scene || m_ScenePC)
			OnChangeCharacter();

		GetDayZGame().Expansion_SetGameVersionText(m_Version);
		Expansion_AdjustButtonsPanel();
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w == m_CharacterRotationFrame)
		{
			if (m_Expansion_Scene)
				m_Expansion_Scene.CharacterRotationStart();

			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();

			return true;
		}

		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (m_Expansion_Scene)
			m_Expansion_Scene.CharacterRotationStop();

		if (m_ScenePC)
			m_ScenePC.CharacterRotationStop();

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		int charID;
		if (w == m_Play && !GetExpansionClientSettings().StreamerMode)
		{
			string ip = "";
			string name = "";
			int port = 0;

			if (m_Expansion_Scene && !m_Expansion_Scene.GetIntroCharacter().IsDefaultCharacter())
			{
				charID = m_Expansion_Scene.GetIntroCharacter().GetCharacterID();
				m_Expansion_Scene.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
				m_LastPlayedTooltipName.SetText("#server_details_name " + name);
				m_LastPlayedTooltipIP.SetText("#main_menu_IP " + ip);
				m_LastPlayedTooltipPort.SetText("#main_menu_port " + port);
				m_LastPlayedTooltipTimer.FadeIn(m_LastPlayedTooltip, 0.3, true);
			}

			if (m_ScenePC && !m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
			{
				charID = m_ScenePC.GetIntroCharacter().GetCharacterID();
				m_ScenePC.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
				m_LastPlayedTooltipName.SetText("#server_details_name " + name);
				m_LastPlayedTooltipIP.SetText( "#main_menu_IP " + ip );
				m_LastPlayedTooltipPort.SetText("#main_menu_port " + port);
				m_LastPlayedTooltipTimer.FadeIn(m_LastPlayedTooltip, 0.3, true);
			}
		}

		if (IsFocusable(w))
		{
			ColorHighlight(w);
			return true;
		}

		return false;
	}

	override bool CanSaveDefaultCharacter()
	{
		//! TODO - check if default character exists
		if (m_Expansion_Scene && m_Expansion_Scene.GetIntroCharacter() && m_Expansion_Scene.GetIntroCharacter().GetCharacterID() == -1)
			return true;

		return super.CanSaveDefaultCharacter();
	}

	override void OpenMenuServerBrowser()
	{
		//! Saves demounit for further use
		if (m_Expansion_Scene && m_Expansion_Scene.GetIntroCharacter() && m_Expansion_Scene.GetIntroCharacter().GetCharacterID() == -1 )
		{
			PlayerBase player = m_Expansion_Scene.GetIntroCharacter().GetCharacterObj();
			if (player && player.GetInventory() && player.GetInventory().FindAttachment(InventorySlots.BODY))
			{
				//! TODO - save default char here if none exists
				//m_Expansion_Scene.GetIntroCharacter().SaveCharacterSetup();
			}
		}

		super.OpenMenuServerBrowser();
	}

	override void NextCharacter()
	{
		if (m_Expansion_Scene && m_Expansion_Scene.GetIntroCharacter())
		{
			int charID = m_Expansion_Scene.GetIntroCharacter().GetNextCharacterID();
			if (charID != m_Expansion_Scene.GetIntroCharacter().GetCharacterID())
			{
				m_Expansion_Scene.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}

	override void PreviousCharacter()
	{
		if (m_Expansion_Scene && m_Expansion_Scene.GetIntroCharacter())
		{
			int charID = m_Expansion_Scene.GetIntroCharacter().GetPrevCharacterID();
			if (charID != m_Expansion_Scene.GetIntroCharacter().GetCharacterID())
			{
				m_Expansion_Scene.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}

	override void OnChangeCharacter(bool create_character = true)
	{
		if (m_Expansion_Scene && m_Expansion_Scene.GetIntroCharacter())
		{
			int charID = m_Expansion_Scene.GetIntroCharacter().GetCharacterID();
			if (create_character)
				m_Expansion_Scene.GetIntroCharacter().CreateNewCharacterById(charID);

			m_PlayerName.SetText(m_Expansion_Scene.GetIntroCharacter().GetCharacterNameById(charID));
			Widget w = m_CustomizeCharacter.FindAnyWidget(m_CustomizeCharacter.GetName() + "_label");

			if (w)
			{
				TextWidget text = TextWidget.Cast(w);
				if (m_Expansion_Scene.GetIntroCharacter().IsDefaultCharacter())
				{
					text.SetText("#layout_main_menu_customize_char");
				}
				else
				{
					text.SetText("#layout_main_menu_rename");
				}
			}

			if (m_Expansion_Scene.GetIntroCharacter().GetCharacterObj())
			{
				if (m_Expansion_Scene.GetIntroCharacter().GetCharacterObj().IsMale())
				{
					m_Expansion_Scene.GetIntroCharacter().SetCharacterGender(ECharGender.Male);
				}
				else
				{
					m_Expansion_Scene.GetIntroCharacter().SetCharacterGender(ECharGender.Female);
				}
			}

			//! Update character stats
			m_Stats.UpdateStats();
		}

		super.OnChangeCharacter(create_character);

		if (m_Expansion_Scene)
		{
			m_Expansion_Scene.SetAnim();
			m_Expansion_Scene.CustomHandItem();
		}
	}

	override void ConnectLastSession()
	{
		string ip = "";
		string name = "";
		int port = 0;
		int charID;

		if (m_Expansion_Scene && !m_Expansion_Scene.GetIntroCharacter().IsDefaultCharacter())
		{
			charID = m_Expansion_Scene.GetIntroCharacter().GetCharacterID();
			m_Expansion_Scene.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
		}

		if (m_ScenePC && !m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
		{
			charID = m_ScenePC.GetIntroCharacter().GetCharacterID();
			m_ScenePC.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
		}

		if (ip.Length() > 0)
		{
			g_Game.ConnectFromServerBrowser(ip, port, "");
		}
		else
		{
			OpenMenuServerBrowser();
		}
	}
};
#endif