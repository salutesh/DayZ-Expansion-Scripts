class ExpansionZombieCommandPathFake_ST
{
	private int m_Var_MovementSpeed;
	private int m_Var_MovementDirection;
	private int m_Var_TurnDir;
	
	private DayZInfected m_Infected;
	
	void ExpansionZombieCommandPathFake_ST(DayZInfected infected)
	{
		m_Infected = infected;
		
		DayZCreatureAnimInterface anim = m_Infected.GetAnimInterface();
		
		m_Var_MovementSpeed = anim.BindVariableFloat("MovementSpeed");
		m_Var_MovementDirection = anim.BindVariableFloat("MovementDirection");
		m_Var_TurnDir = anim.BindVariableFloat("TurnDir");
	}
	
	void SetMovementSpeed(DayZInfectedCommandScript script, float val)
	{
		script.PreAnim_SetFloat(m_Var_MovementSpeed, val);
	}
	
	void SetMovementDirection(DayZInfectedCommandScript script, float val)
	{
		script.PreAnim_SetFloat(m_Var_MovementDirection, val);
	}
	
	void SetTurnDir(DayZInfectedCommandScript script, float val)
	{
		script.PreAnim_SetFloat(m_Var_TurnDir, val);
	}
};

class ExpansionZombieCommandPathFake extends DayZInfectedCommandScript
{
	
};