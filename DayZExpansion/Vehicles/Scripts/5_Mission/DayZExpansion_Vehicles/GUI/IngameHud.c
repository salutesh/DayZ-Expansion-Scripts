/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_VEHICLES_HUD_OLD
modded class IngameHud
{
	override void Init(Widget hud_panel_widget)
	{
		ExpansionHelicopterHud helicopterHud = new ExpansionHelicopterHud();
		m_VehicleHudMap.Insert("ExpansionVehicleTypeHelicopter", helicopterHud);
		
		ExpansionBoatHud boatHud = new ExpansionBoatHud();
		m_VehicleHudMap.Insert("ExpansionVehicleTypeBoat", boatHud);

		super.Init(hud_panel_widget);
	}

#ifdef DAYZ_1_26
	override void HideVehicleInfo()
	{
		if (m_ActiveVehicleHUD)
		{
			m_CurrentVehicle	= null;
			m_VehicleGearCount	= -1;
		}

		super.HideVehicleInfo();
	}
#endif
};
#endif
