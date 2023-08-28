modded class CharacterCreationMenu
{
	void CharacterCreationMenu()
	{
		if (m_Scene.Expansion_IsCustomScene())
		{
			if (m_Scene.Expansion_CanUseCustomSceneInCharacterCreation())
				m_Scene.Expansion_SetupCharacter(true);
			else if (!m_Scene.Expansion_LoadRandomScene("cfgExpansionCharacterScenes " + g_Game.GetWorldName(), true))
				m_Scene.Expansion_LoadRandomSceneVanilla();
		}
	}

	override void Back()
	{
		super.Back();

		if (m_Scene.Expansion_IsCustomScene())
			m_Scene.Expansion_SetupCharacter();
	}
}
