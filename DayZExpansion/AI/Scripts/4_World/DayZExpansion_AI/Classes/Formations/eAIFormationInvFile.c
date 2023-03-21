class eAIFormationInvFile : eAIFormation
{
	override vector GetPosition(int member_no)
	{
		return Vector(0, 0, 2 * member_no);
	}
};
