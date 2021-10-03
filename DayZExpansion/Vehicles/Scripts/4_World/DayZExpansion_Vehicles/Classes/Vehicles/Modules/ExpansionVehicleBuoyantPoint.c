/**
 * ExpansionVehicleBuoyantPoint.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleBuoyantPoint : ExpansionVehicleModule
{
	vector m_Position;

	float m_Depth;
	vector m_Velocity;

	void ExpansionVehicleBuoyantPoint(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " point";
		m_Position = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Depth = GetGame().GetWaterDepth(m_Vehicle.ModelToWorld(m_Position));
		m_Velocity = pState.GetModelVelocityAt(m_Position);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		if (m_Depth <= 0)
			return;

#ifdef DAYZ_1_13
		vector buoyancy = Vector(0, 9.8, 0) * pState.m_Mass * m_Depth * pState.m_DeltaTime * 0.5;
#else
		vector buoyancy = -dGetGravity(m_Vehicle) * pState.m_Mass * m_Depth * pState.m_DeltaTime * 0.5;
#endif
		vector drag = -m_Velocity * pState.m_Mass * pState.m_DeltaTime * 0.25;

		vector impulse = (buoyancy + drag).InvMultiply3(pState.m_Transform);

		pState.DBGDrawImpulseMS(m_Position, impulse);

		pState.m_Impulse += impulse.Multiply3(pState.m_Transform);
		pState.m_ImpulseTorque += (m_Position * impulse).Multiply3(pState.m_Transform);
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Depth", m_Depth);

		return true;
	}
#endif
};
