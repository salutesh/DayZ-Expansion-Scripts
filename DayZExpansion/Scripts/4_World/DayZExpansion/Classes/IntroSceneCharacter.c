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
		auto trace = EXTrace.StartStack(EXTrace.MISC, this);

		super.CreateNewCharacter();
	}

	override protected void CharacterUnload()
	{
		auto trace = EXTrace.StartStack(EXTrace.MISC, this);

		super.CharacterUnload();
	}

	override protected void CharacterLoad(int character_id, vector char_pos, vector char_rot)
	{
		auto trace = EXTrace.StartStack(EXTrace.MISC, this, "char_pos " + char_pos, "char_rot " + char_rot);

		super.CharacterLoad(character_id, char_pos, char_rot);
	}
}
