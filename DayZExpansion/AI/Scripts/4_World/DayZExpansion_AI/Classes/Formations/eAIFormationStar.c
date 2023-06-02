class eAIFormationStar : eAIFormation
{
	override vector GetPosition(int member_no)
	{
		float x = 0;
		float z = 0;
		float angle;
		int count = m_Group.Count();
		float radius = Math.Max((count * 2.828427) / (Math.PI * 2), 2.0);

		if (count)
			angle = 360 / count * member_no;

		if (member_no % 2 == 0)
		{
			x = Math.Sin(Math.DEG2RAD * angle) * radius * GetScale();
			z = Math.Cos(Math.DEG2RAD * angle) * radius * GetScale();
		}
		else
		{
			x = Math.Sin(Math.DEG2RAD * angle) * radius * 0.5 * GetScale();
			z = Math.Cos(Math.DEG2RAD * angle) * radius * 0.5 * GetScale();
		}

		return Vector(x, 0, z - radius);
	}
};
