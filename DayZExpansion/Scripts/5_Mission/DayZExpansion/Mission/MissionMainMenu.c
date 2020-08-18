#ifndef DISABLEMAINMENUCHANGE
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