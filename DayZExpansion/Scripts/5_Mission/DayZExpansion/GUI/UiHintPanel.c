/*
	Ui class for hints in in-game-menu
*/
modded class UiHintPanel
{
	private const string 			m_ExpansionDataPath			= "DayZExpansion/Scripts/Data/Hints.json";							// Json path
	
	// Load content data from json file 
	override private void LoadContentList()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUIHintPanel::LoadContentList - Start");
		#endif
		JsonFileLoader<array<ref HintPage>>.JsonLoadFile( m_ExpansionDataPath, m_ContentList );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUIHintPanel::LoadContentList - End");
		#endif
	}
}