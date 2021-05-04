/**
 * ExpansionAirdropLootContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAirdropLootContainer
{
	string Container;
	int Usage;  //! 0 = missions & player called, 1 = only missions, 2 = only player called
	float Weight;
	ref array < ref ExpansionAirdropLoot > Loot;
	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;
	bool SpawnInfectedForPlayerCalledDrops;

	void ExpansionAirdropLootContainer( string container, int usage, float weight, ref array < ref ExpansionAirdropLoot > loot, ref TStringArray infected, int itemCount, int infectedCount, bool spawnInfectedForPlayerCalledDrops = false )
	{
		Container = container;
		Usage = usage;
		Weight = weight;
		Loot = loot;
		Infected = infected;
		ItemCount = itemCount;
		InfectedCount = infectedCount;
		SpawnInfectedForPlayerCalledDrops = spawnInfectedForPlayerCalledDrops;
	}

	static ExpansionAirdropLootContainer GetWeightedRandomContainer( ref array< ref ExpansionAirdropLootContainer > containers, ref array< float > weights = NULL )
	{
		if ( weights == NULL)
		{
			weights = new array< float >;

			for ( int i = 0; i < containers.Count(); i++ )
			{
				weights.Insert( containers[i].Weight );
			}
		}
		
		ExpansionAirdropLootContainer container;
		int index = GetWeightedRandom( weights );

		if ( index > -1 )
			return containers[index];

		//! Probably old settings without weights
		Print( "[ExpansionAirdropLootContainer] GetWeightedRandomContainer: All containers have a 'Weight' of zero. Selecting a random container instead." );
		return containers.GetRandomElement();
	}
}
