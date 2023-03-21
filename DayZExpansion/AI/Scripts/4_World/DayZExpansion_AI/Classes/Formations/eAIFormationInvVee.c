class eAIFormationInvVee : eAIFormation
{
	override vector GetPosition(int member_no)
	{
		int offset = Math.Floor((member_no + 1) / 2);
		float scaled_offset = 2 * offset * GetScale();
		if (member_no % 2 == 0) return Vector(scaled_offset, 0, scaled_offset); // Right side
		return Vector(-scaled_offset, 0, scaled_offset); // Left Side
	}
};
