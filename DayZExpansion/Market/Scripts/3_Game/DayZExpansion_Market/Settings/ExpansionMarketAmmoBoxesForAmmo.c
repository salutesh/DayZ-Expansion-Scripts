/**
 * ExpansionMarketAmmoBoxesForAmmo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMarketAmmoBoxesForAmmo
 * @brief		Market ammo boxes settings class
 **/
class ExpansionMarketAmmoBoxesForAmmo
{
	string Ammo;
	string AmmoBox;
	
	void ExpansionMarketAmmoBoxesForAmmo(string ammo, string ammobox)
	{
		Ammo = ammo;
		AmmoBox = ammobox;
	}
}