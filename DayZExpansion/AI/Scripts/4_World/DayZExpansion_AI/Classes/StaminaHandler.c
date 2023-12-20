/**
 * StaminaHandler.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class StaminaHandler
{
	override void Update(float deltaT, int pCurrentCommandID)
	{
		if (!m_Player.IsAI() || !m_Player.GetGroup().GetFaction().HasUnlimitedStamina())
			super.Update(deltaT, pCurrentCommandID);
	}

	override void DepleteStamina(EStaminaModifiers modifier, float dT = -1)
	{
		if (!m_Player.IsAI() || !m_Player.GetGroup().GetFaction().HasUnlimitedStamina())
			super.DepleteStamina(modifier, dT);
	}
}
