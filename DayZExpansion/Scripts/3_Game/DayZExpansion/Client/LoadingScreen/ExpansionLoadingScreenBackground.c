/**
 * ExpansionLoadingScreenBackground.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLoadingScreenBackground
{
	string MapName;
	ref array<string> Path;

	[NonSerialized()]
	ref ExpansionArray<string> m_Path;

	// ------------------------------------------------------------
	// ExpansionLoadingScreenBackground Constructor
	// ------------------------------------------------------------
	void ExpansionLoadingScreenBackground( string map_name, array<string> texture_path ) 
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "ExpansionLoadingScreenBackground");
#endif
		
		MapName = map_name;
		Path = texture_path;
	}

	//! Get a random path, preferring ones that haven't been picked yet
	string GetRandomPath()
	{
		if (!m_Path)
			m_Path = new ExpansionArray<string>(Path);

		return m_Path.GetQuasiRandomElementAvoidRepetition();
	}
};

class ExpansionLoadingScreenBackgrounds
{
	static ref array<ref ExpansionLoadingScreenBackground> s_Backgrounds = new array<ref ExpansionLoadingScreenBackground>;

	static array<ref ExpansionLoadingScreenBackground> Get()
	{
		if (!s_Backgrounds.Count())
			JsonFileLoader<array<ref ExpansionLoadingScreenBackground>>.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingImages.json", s_Backgrounds );

		return s_Backgrounds;
	}
}
