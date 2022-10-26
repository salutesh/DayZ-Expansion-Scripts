/**
 * ExpansionSpawnLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnLocation
{
	string Name;
	ref array< vector > Positions;
	bool UseCooldown;
	
	[NonSerialized()]
	bool IsTerritory;
	
	void ExpansionSpawnLocation(string name, array< vector > positions, bool isTerritory = false)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionSpawnLocation");
#endif
			
		Name = name;
		
		Positions = new array< vector >;
		for ( int i = 0; i < positions.Count(); i++ )
			Positions.Insert( positions[i] );
		
		IsTerritory = isTerritory;
	}

	static ExpansionSpawnLocation Copy(  ExpansionSpawnLocation src )
	{
		ExpansionSpawnLocation dst = new ExpansionSpawnLocation( src.Name, new array< vector >() );
		return dst;
	}
	
	void SetIsTerritory(bool state)
	{
		IsTerritory = state;
	}
	
	bool IsTerritory()
	{
		return IsTerritory;
	}
	
	void SetUseCooldown(bool state)
	{
		UseCooldown = state;
	}
	
	bool UseCooldown()
	{
		return UseCooldown;
	}
};