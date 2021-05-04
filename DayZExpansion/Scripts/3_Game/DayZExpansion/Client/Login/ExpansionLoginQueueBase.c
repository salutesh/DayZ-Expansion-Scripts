/**
 * ExpansionLoginQueueBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoginQueueBase
{
	protected ImageWidget m_ImageBackground;
	protected autoptr array< ref ExpansionLoadingScreenBackground > m_Backgrounds;
	
	void LoginQueueBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::LoginQueueBase - Start");
		#endif
		
		JsonFileLoader< ref array< ref ExpansionLoadingScreenBackground > >.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingImages.json", m_Backgrounds );
	
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::LoginQueueBase - End");
		#endif
	}
	
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::Init - Start");
		#endif
			
		super.Init();
		
		m_ImageBackground = ImageWidget.Cast( layoutRoot.FindAnyWidget("Background") );

		string world_name = "default";

		if ( GetGame() )
			GetGame().GetWorldName(world_name);

		world_name.ToLower();
			
		if ( world_name == "chernarusplusgloom" ) 	world_name = "chernarusplus";
		if ( world_name == "enochgloom" ) 			world_name = "enoch";
		if ( world_name == "namalskgloom" ) 		world_name = "namalsk";
		if ( world_name == "deerislegloom" ) 		world_name = "deerisle";
		if ( world_name == "chiemseegloom" ) 		world_name = "chiemsee";
		
		ExpansionLoadingScreenBackground backgrounds = m_Backgrounds[0];

		for ( int i = 0; i < m_Backgrounds.Count(); ++i )
		{
			if ( world_name == m_Backgrounds[i].MapName )
			{
				backgrounds = m_Backgrounds[i];
				break;
			}
		}
		
		if (backgrounds)
			m_ImageBackground.LoadImageFile( 0, backgrounds.Path.GetRandomElement() );

		return layoutRoot;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::Init - End");
		#endif
	}
};