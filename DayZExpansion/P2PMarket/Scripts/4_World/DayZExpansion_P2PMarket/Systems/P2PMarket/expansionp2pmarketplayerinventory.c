/**
 * ExpansionP2PMarketPlayerInventory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketPlayerInventory extends ExpansionMarketPlayerInventory
{
	override bool IsVehicleNearby(Object vehicle)
	{
		float maxDistance = 150.0;
		if (vector.Distance(m_Player.GetPosition(), vehicle.GetPosition()) <= maxDistance)
			return true;
		return false;
	}
};