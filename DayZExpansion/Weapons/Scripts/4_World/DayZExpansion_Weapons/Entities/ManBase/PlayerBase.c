/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (ammo == "Bullet_Expansion_Taser")
			Expansion_ProcessTaser();

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	// This Function is meant for modders to add custom behaviours to the taser in a more friendly way
	void Expansion_ProcessTaser()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(WakePlayer, 9000, false);
	}

	// ------------------------------------------------------------
	// PlayerBase WakePlayer
	// ------------------------------------------------------------
	void WakePlayer()
	{
		AddHealth("","Shock", 100);
	}
	
	// ------------------------------------------------------------
	// Expansion ModCommandHandlerInside
	// ------------------------------------------------------------
	override bool ModCommandHandlerInside( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		ItemBase item = GetItemInHands();
		if ( item )
		{
			item.UpdateLaser();
		}

		if ( super.ModCommandHandlerInside( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
		{
			return true;
		}

		return false;
	}
};
