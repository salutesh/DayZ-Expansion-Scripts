/**
 * PlayerStats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
enum EPlayerStatsNamalskAdventure: EPlayerStatsNamalsk
{
	RADIATION,
}
#else
enum EPlayerStatsNamalskAdventure: EPlayerStats_current
{
	RADIATION,
}
#endif

modded class PlayerStatsPCO_current
{
	override void Init()
	{
		super.Init();

		RegisterStat(EPlayerStatsNamalskAdventure.RADIATION, new PlayerStat<float>(0, 100, 0, "Radiation", EPSstatsFlags.EMPTY));
	}
}