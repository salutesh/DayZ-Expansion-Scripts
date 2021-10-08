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

/**@class		ExpansionMarketSpawnPositionV1
 * @brief		Market vehicle spawn positions settings class V1
 **/
class ExpansionMarketSpawnPositionV1
{
	vector Position;
	vector Direction;

	void ExpansionMarketSpawnPositionV1(vector pos = vector.Zero, vector dir = vector.Zero)
	{
		Position = pos;
		Direction = dir;
	}

	void Copy(ExpansionMarketSpawnPositionV1 s)
	{
		Position = s.Position;
		Direction = s.Direction;
	}
}

/**@class		ExpansionMarketSpawnPosition
 * @brief		Market vehicle spawn positions settings class
 **/
class ExpansionMarketSpawnPosition
{
	vector Position;
	vector Orientation;

	void ExpansionMarketSpawnPosition(vector pos = vector.Zero, vector ori = vector.Zero)
	{
		Position = pos;
		Orientation = ori;
	}

	void Copy(ExpansionMarketSpawnPosition s)
	{
		Position = s.Position;
		Orientation = s.Orientation;
	}
}