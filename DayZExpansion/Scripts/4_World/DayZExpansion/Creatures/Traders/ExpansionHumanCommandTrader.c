class ExpansionHumanCommandTrader_ST
{
	void ExpansionHumanCommandTrader_ST( Human human )
	{
		HumanAnimInterface hai = human.GetAnimInterface();
	}
}

class ExpansionHumanCommandTrader extends HumanCommandScript
{
	private ExpansionHumanCommandTrader_ST m_Table;

	void ExpansionHumanCommandTrader( DayZPlayer player, ExpansionHumanCommandTrader_ST table )
	{
		m_Table = table;
	}
}