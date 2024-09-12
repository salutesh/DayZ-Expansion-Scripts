modded class IntroSceneCharacter
{
	void Expansion_SetPosition(vector position)
	{
		m_CharacterPos = position;
		m_CharacterObj.SetPosition(m_CharacterPos);
	}

	void Expansion_SetOrientation(vector orientation)
	{
		m_CharacterRot = orientation;
		m_CharacterObj.SetOrientation(m_CharacterRot);
	}

	override void CreateNewCharacter()
	{
#ifdef EXTRACE
		auto trace = EXTrace.StartStack(EXTrace.MISC, this);
#endif

		super.CreateNewCharacter();
	}

	override protected void CharacterUnload()
	{
#ifdef EXTRACE
		auto trace = EXTrace.StartStack(EXTrace.MISC, this);
#endif

		super.CharacterUnload();
	}

	override protected void CharacterLoad(int character_id, vector char_pos, vector char_rot)
	{
#ifdef EXTRACE
		auto trace = EXTrace.StartStack(EXTrace.MISC, this, "char_pos " + char_pos, "char_rot " + char_rot);
#endif

		super.CharacterLoad(character_id, char_pos, char_rot);
	}
}
