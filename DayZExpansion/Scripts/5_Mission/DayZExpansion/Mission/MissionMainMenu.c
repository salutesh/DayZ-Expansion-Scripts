/**
 * MissionMainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class MissionMainMenu
{
	private ref DayZIntroSceneExpansion m_IntroSceneExpansion;
	
	void ~MissionMainMenu()
	{
		DestroyExpansionIntroScene();
	}
	
	override void OnInit()
	{
		bool customScene = true;
		string root_path = "cfgExpansionCharacterScenes " + g_Game.GetWorldName();

		int count = g_Game.ConfigGetChildrenCount(root_path);
		if (count == 0)
			customScene = false;
			
		if (!m_NoCutscene)
		{
			if (customScene)
				CreateExpansionIntroScene();
			else
				CreateIntroScene();
		}
		
		if (!m_mainmenu)
		{
			m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_MAIN, null ) );
		}
	}
	
	override void Reset()
	{
		DestroyExpansionIntroScene();
		CreateExpansionIntroScene();
	}
	
	override void OnUpdate(float timeslice)
	{
		//Print("("+ GetGame().GetTime() +") ____OnUpdate: " + timeslice);
		
		if ( g_Game.IsLoading() )
		{
			return;
		}
				
		if (m_IntroSceneExpansion )
		{
			m_IntroSceneExpansion.Update();
		}
	}
	
	void CreateExpansionIntroScene()
	{		
		//Print("misssionMainMenu CreateIntroScene");
		m_IntroSceneExpansion = new DayZIntroSceneExpansion;
	}
	
	void DestroyExpansionIntroScene()
	{
		if (m_IntroSceneExpansion)
			delete m_IntroSceneExpansion;
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
		#ifndef NAMALSK_SURVIVAL
		if ( !m_MenuMusic )
		{
			SoundParams soundParams			= new SoundParams( "Expansion_Music_2_SoundSet" );
			SoundObjectBuilder soundBuilder	= new SoundObjectBuilder( soundParams );
			SoundObject soundObject			= soundBuilder.BuildSoundObject();
			soundObject.SetKind( WaveKind.WAVEMUSIC );
			m_MenuMusic = GetGame().GetSoundScene().Play2D(soundObject, soundBuilder);
			m_MenuMusic.Loop( true );
			m_MenuMusic.Play();
		}
		#else
		super.PlayMusic();
		#endif
	}
}