/**
 * ActionTurnOffWeaponFlashlight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTurnOffWeaponFlashlight
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsInherited(Rifle_Base) )
		{
			m_flashlight = ItemBase.Cast(item.FindAttachmentBySlotName("weaponFlashlight"));
		}
		else if (item.IsInherited(Pistol_Base))
		{
			m_flashlight = ItemBase.Cast(item.FindAttachmentBySlotName("pistolFlashlight"));
		}
		else
		{
			return false;
		}

		if (m_flashlight.GetType() == "ExpansionPropUniversalLight")
			return false;

		return super.ActionCondition(player, target, item);
	}
};