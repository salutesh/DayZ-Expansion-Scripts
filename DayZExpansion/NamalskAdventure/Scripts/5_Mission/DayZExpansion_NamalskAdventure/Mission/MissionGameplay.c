/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionGameplay
{
	protected ImageWidget m_Radiation;
	protected float m_CurrentRadiation;
	protected ref ExpansionRadiationIndicator m_ExpansionRadiationIndicator;

	override void OnInit()
	{
		super.OnInit();

		InitExpansionRadiationIndicator();
	}

	void InitExpansionRadiationIndicator()
	{
		if (!m_ExpansionRadiationIndicator)
		{
			m_ExpansionRadiationIndicator = new ExpansionRadiationIndicator(m_Hud);
		}
	}
	
	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		if (menu && menu.GetID() == MENU_INVENTORY)
		{
			if (m_Hud.GetHudVisibility().IsContextFlagActive(EHudContextFlags.VEHICLE))
				m_Hud.GetHudVisibility().SetContextFlag(EHudContextFlags.VEHICLE_DISABLE, true);
		}
		else
		{
			HumanCommandVehicle cmdVehicle = player.GetCommand_Vehicle();
			if (cmdVehicle && cmdVehicle.GetTransport() && m_Hud.Expansion_CanShowHUDElements(player))
			{
				if (m_Hud.GetHudVisibility().IsContextFlagActive(EHudContextFlags.VEHICLE_DISABLE) && m_Hud.GetHudVisibility().IsContextFlagActive(EHudContextFlags.VEHICLE))
					m_Hud.GetHudVisibility().SetContextFlag(EHudContextFlags.VEHICLE_DISABLE, false);
			}
		}
	}
};