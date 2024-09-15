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

/**@class		MissionGameplay
 * @brief
 **/
modded class MissionGameplay
{
	//! PlayerList check
	protected bool m_Expansion_PlayerListTogglePressed;
	protected float m_Expansion_NVUpdateTick;
	//! Modules
	protected ref ExpansionAutorunModule m_AutoRunModule;

	void MissionGameplay()
	{
		CF_Modules<ExpansionAutorunModule>.Get(m_AutoRunModule);
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		auto generalSettings = GetExpansionSettings().GetGeneral(false);

		if (isAliveConscious)
		{
			if (!menu && !inputIsFocused)
			{
				//! Autorun
				if (input.LocalPress("UAExpansionAutoRunToggle", false))
				{
					if (!player.GetParent() && generalSettings.EnableAutoRun)
						m_AutoRunModule.AutoRun();
				}

				//! Toggle Earplugs
				if (input.LocalPress( "UAExpansionEarplugsToggle", false )  && !viewMenu)
				{
					if (generalSettings.EnableEarPlugs)
						m_Hud.Expansion_ToggleEarplugs();
				}

				//! Toggle Player list menu
				if (input.LocalPress("UAExpansionPlayerListToggle", false) && !m_Expansion_PlayerListTogglePressed)
				{
					m_Expansion_PlayerListTogglePressed = true;
					Expansion_TogglePlayerList();
				}
				else if (input.LocalRelease("UAExpansionPlayerListToggle", false) || input.LocalValue("UAExpansionPlayerListToggle", false) == 0)
				{
					m_Expansion_PlayerListTogglePressed = false;
				}
			}

			if (m_AutoRunModule)
			{
				//! Autowalk
				if (generalSettings.EnableAutoRun)
					m_AutoRunModule.UpdateAutoWalk();

				//! Stop autorun when different inputs are pressed
				if (!m_AutoRunModule.IsDisabled() && !inputIsFocused)
				{
					if (ExpansionStatic.INPUT_FORWARD() || ExpansionStatic.INPUT_BACK() || ExpansionStatic.INPUT_LEFT() || ExpansionStatic.INPUT_RIGHT() || ExpansionStatic.INPUT_STANCE())
						m_AutoRunModule.AutoRun();
				}
			}
		}

		//! Nightvision check
		if (generalSettings.EnableHUDNightvisionOverlay)
		{
			m_Expansion_NVUpdateTick += timeslice;
			if (m_Expansion_NVUpdateTick > 0.1)
			{
				m_Expansion_NVUpdateTick = 0.0;
				Expansion_PlayerCheckNV(player);
			}
		}
	}

	void Expansion_PlayerCheckNV(PlayerBase player)
	{
		if (!m_Hud || !player)
			return;

		DayZPlayerCameraBase camera;
		if (Class.CastTo(camera, player.GetCurrentCamera()))
		{
			if (camera.IsCameraNV())
			{
				if (!m_Hud.Expansion_GetNVState())
					m_Hud.Expansion_ShowNV(true);
			}
			else
			{
				if (m_Hud.Expansion_GetNVState())
					m_Hud.Expansion_ShowNV(false);

				return;
			}
		}

		ItemBase nvItem = player.Expansion_GetNVItem();
		if (nvItem)
			m_Hud.Expansion_SetNVBatteryState(nvItem.Expansion_GetBatteryEnergy());
	}

	void Expansion_TogglePlayerList()
	{
		if (GetExpansionSettings().GetPlayerList().EnablePlayerList)
		{
			ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager(); //! Reference to expansion ui manager
			ScriptView menu	= uiManager.GetMenu(); //! Reference to current opened script view menu
			ExpansionPlayerListMenu playerListMenu = ExpansionPlayerListMenu.Cast(menu);
			if (!playerListMenu)
			{
				uiManager.CreateSVMenu("ExpansionPlayerListMenu");
			}
			else if (playerListMenu && playerListMenu.IsVisible())
			{
				uiManager.CloseMenu();
			}
		}
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		//TODO: use Input system instead
		switch (key)
		{
			case KeyCode.KC_PRIOR:
			{
				if (m_Hud.Expansion_GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp(GetExpansionClientSettings().EarplugLevel + 0.01, 0.0, 1.0);
					GetExpansionClientSettings().Save();
					m_Hud.Expansion_UpdateEarplugs();
				}
				break;
			}

			case KeyCode.KC_NEXT:
			{
				if (m_Hud.Expansion_GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp(GetExpansionClientSettings().EarplugLevel - 0.01, 0.0, 1.0);
					GetExpansionClientSettings().Save();
					m_Hud.Expansion_UpdateEarplugs();
				}
				break;
			}
		}
	}

	override void AddActiveInputRestriction(int restrictor)
	{
		super.AddActiveInputRestriction(restrictor);

		switch (restrictor)
		{
			case EInputRestrictors.INVENTORY:
			case EInputRestrictors.MAP:
				if (!m_AutoRunModule.IsDisabled())
					GetUApi().GetInputByID(UAWalkRunForced).ForceEnable(false); //! force walk off if autorunning
				break;
		}
	}
};
