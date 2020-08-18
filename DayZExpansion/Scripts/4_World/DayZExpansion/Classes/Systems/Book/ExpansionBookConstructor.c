/**
 * ExpansionBookConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookConstructor
{	
	// ------------------------------------------------------------
	// Expansion GenerateTabs
	// ------------------------------------------------------------
	void GenerateTabs( out array< ref ExpansionBookTabBase > tabs )
	{
		TTypenameArray tabArrayNames = new TTypenameArray;
		RegisterTabs( tabArrayNames);

		Sort( tabArrayNames, tabArrayNames.Count() );
		
		tabs = new array< ref ExpansionBookTabBase >;
		
		for(int i = 0; i < tabArrayNames.Count(); i++)
		{
			if ( tabArrayNames[i].IsInherited( ExpansionBookTabBase ) )
			{
				tabs.Insert( ExpansionBookTabBase.Cast( tabArrayNames[i].Spawn() ) );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RegisterTabs
	// ------------------------------------------------------------
	void RegisterTabs( out TTypenameArray tabs )
	{
		tabs.Insert( ExpansionBookTabServerInfo );
		tabs.Insert( ExpansionBookTabServerRules );
		tabs.Insert( ExpansionBookTabStatus );
		tabs.Insert( ExpansionBookTabParty );
		tabs.Insert( ExpansionBookTabTerritory );
	}
}