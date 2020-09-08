/**
 * MissionMainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class MissionMainMenu
{
	private ref DayZIntroSceneExpansion m_IntroSceneExpansion;
	
	override void OnInit()
	{
		if (!m_NoCutscene)
		{
			CreateExpansionIntroScene();
		}
		
		if (!m_mainmenu)
		{
			m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_MAIN, null ) );
		}
	}
	override void Reset()
	{
		delete m_IntroSceneExpansion;
		
		CreateExpansionIntroScene();
	}
	
	override void OnUpdate(float timeslice)
	{
		//Print("("+ GetGame().GetTime() +") ____OnUpdate: " + timeslice);
		
		if ( g_Game.IsLoading() )
		{
			return;
		}
				
		if (m_IntroSceneExpansion)
		{
			m_IntroSceneExpansion.Update();
		}
	}
	void CreateExpansionIntroScene()
	{		
		//Print("misssionMainMenu CreateIntroScene");
		m_IntroSceneExpansion = new DayZIntroSceneExpansion;
	}
	
	DayZIntroSceneExpansion GetIntroSceneExpansion()
	{
		return m_IntroSceneExpansion;
	}
};
#endif

modded class MissionMainMenu
{
	override void PlayMusic()
	{
		if ( !m_MenuMusic )
		{
			ref SoundParams soundParams			= new SoundParams( "Expansion_Music_1_SoundSet" );
			ref SoundObjectBuilder soundBuilder	= new SoundObjectBuilder( soundParams );
			ref SoundObject soundObject			= soundBuilder.BuildSoundObject();
			soundObject.SetKind( WaveKind.WAVEMUSIC );
			m_MenuMusic = GetGame().GetSoundScene().Play2D(soundObject, soundBuilder);
			m_MenuMusic.Loop( true );
			m_MenuMusic.Play();
		}
	}
}