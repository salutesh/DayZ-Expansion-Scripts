/**
 * ExpansionLootContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLootContainer
{
	string Container;
	float FallSpeed;
	int Usage;  //! 0 = missions & player called, 1 = only missions, 2 = only player called
	float Weight;
	ref array < ref ExpansionLoot > Loot;
	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;
	bool SpawnInfectedForPlayerCalledDrops;

	void ExpansionLootContainer( string container, int usage, float weight, array < ref ExpansionLoot > loot, TStringArray infected, int itemCount, int infectedCount, bool spawnInfectedForPlayerCalledDrops = false, float fallSpeed = 4.5 )
	{
		Container = container;
		Usage = usage;
		Weight = weight;
		Loot = loot;
		Infected = infected;
		ItemCount = itemCount;
		InfectedCount = infectedCount;
		SpawnInfectedForPlayerCalledDrops = spawnInfectedForPlayerCalledDrops;
		FallSpeed = fallSpeed;
	}

	static ExpansionLootContainer GetWeightedRandomContainer(  array< ref ExpansionLootContainer > containers, array< float > weights = NULL )
	{
		array< float > weights_T = weights;
		if ( weights_T == NULL)
		{
			weights_T = new array< float >;

			for ( int i = 0; i < containers.Count(); i++ )
			{
				weights_T.Insert( containers[i].Weight );
			}
		}
		
		ExpansionLootContainer container;
		int index = ExpansionStatic.GetWeightedRandom( weights_T );

		if ( index > -1 )
			return containers[index];

		//! Probably old settings without weights
		Print( "[ExpansionLootContainer] GetWeightedRandomContainer: All containers have a 'Weight' of zero. Selecting a random container instead." );
		return containers.GetRandomElement();
	}
}
