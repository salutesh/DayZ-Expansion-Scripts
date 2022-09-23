/**
 * ExpansionDebugObjects.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDebugRod: Building
{
	float m_Expansion_DebugOriginX;
	float m_Expansion_DebugOriginY;
	float m_Expansion_DebugOriginZ;
	int m_Expansion_DebugColor;
	Shape m_Expansion_DebugLine;

	void ExpansionDebugRod()
	{
		RegisterNetSyncVariableFloat("m_Expansion_DebugOriginX", 0, 0, 2);
		RegisterNetSyncVariableFloat("m_Expansion_DebugOriginY", 0, 0, 2);
		RegisterNetSyncVariableFloat("m_Expansion_DebugOriginZ", 0, 0, 2);

#ifndef SERVER
		ExpansionString es = new ExpansionString(GetType());

		if (es.EndsWith("_Red"))
			m_Expansion_DebugColor = 0xFFFF0000;
		else if (es.EndsWith("_Orange"))
			m_Expansion_DebugColor = 0xFF800000;
		else if (es.EndsWith("_Blue"))
			m_Expansion_DebugColor = 0xFF0000FF;
		else if (es.EndsWith("_Purple"))
			m_Expansion_DebugColor = 0xFF8000FF;
		else
			m_Expansion_DebugColor = 0xFF00FF00;
#endif
	}

	override void EEInit()
	{
		super.EEInit();

		if (dBodyIsSet(this))
			dBodyDestroy(this);  //! Remove physics
	}

	void ~ExpansionDebugRod()
	{
#ifndef SERVER
		Debug.RemoveShape(m_Expansion_DebugLine);
#endif
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		Expansion_DrawDebugLine(Vector(m_Expansion_DebugOriginX, m_Expansion_DebugOriginY, m_Expansion_DebugOriginZ));
	}

	void Expansion_DrawDebugLine(vector origin)
	{
#ifdef SERVER
		m_Expansion_DebugOriginX = origin[0];
		m_Expansion_DebugOriginY = origin[1];
		m_Expansion_DebugOriginZ = origin[2];

		SetSynchDirty();
#else
		Debug.RemoveShape(m_Expansion_DebugLine);
		m_Expansion_DebugLine = Debug.DrawLine(origin, GetPosition(), m_Expansion_DebugColor);
#endif
	}
}