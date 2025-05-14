modded class MiscGameplayFunctions
{
	override static vector GetHeadingVector(notnull PlayerBase player)
	{
		return player.Expansion_GetHeadingVector();
	}

	override static float GetHeadingAngle(notnull DayZPlayerImplement player)
	{
		return player.Expansion_GetHeadingAngle();
	}
}
