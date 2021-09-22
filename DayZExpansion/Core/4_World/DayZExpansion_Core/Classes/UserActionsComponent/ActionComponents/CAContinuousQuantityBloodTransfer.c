/**
 * CAContinuousQuantityBloodTransfer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CAContinuousQuantityBloodTransfer
{
	override void CalcAndSetQuantity(ActionData action_data)
	{
		//! Make giving blood to self in safezones actually increase blood by temporarily disabling godmode
		if (GetGame().IsServer() && action_data.m_Player.IsInSafeZone())
			action_data.m_Player.SetAllowDamage(true);

		super.CalcAndSetQuantity(action_data);

		if (GetGame().IsServer() && action_data.m_Player.IsInSafeZone())
			action_data.m_Player.SetAllowDamage(false);
	}
}
