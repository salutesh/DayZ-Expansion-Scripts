class eAIFormationWall : eAIFormation
{
	override vector GetPosition(int member_no)
	{
		int offset = Math.Floor((member_no + 1) / 2);
		float scaled_offset = 2 * offset * GetScale();
		if (member_no % 2 == 0) return Vector(scaled_offset, 0, 0); // Right side
		return Vector(-scaled_offset, 0, 0); // Left Side
	}
};
