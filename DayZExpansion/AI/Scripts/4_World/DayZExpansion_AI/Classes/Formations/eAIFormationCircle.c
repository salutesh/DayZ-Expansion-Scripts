class eAIFormationCircle : eAIFormation
{
	override vector GetPosition(int member_no)
	{
		float angle;
		int count = m_Group.Count();
		float radius = Math.Max((count * 2.828427) / (Math.PI * 2), 2.0);

		if (count)
			angle = 360 / count * member_no;

		float x = Math.Sin(Math.DEG2RAD * angle) * radius * GetScale();
		float z = Math.Cos(Math.DEG2RAD * angle) * radius * GetScale();

		return Vector(x, 0, z - radius);
	}
};
