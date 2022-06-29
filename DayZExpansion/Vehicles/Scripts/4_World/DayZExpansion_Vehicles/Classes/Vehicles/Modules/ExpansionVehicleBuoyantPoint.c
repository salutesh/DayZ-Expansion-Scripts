/**
 * ExpansionVehicleBuoyantPoint.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleBuoyantPoint : ExpansionVehicleModule
{
	float m_Depth;
	vector m_Velocity;

	void ExpansionVehicleBuoyantPoint(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " point";
		m_Position = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));
		
		m_SelfDebugWindow = true;
		
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = false;
		m_PreSimulate = true;
		m_Simulate = true;
		m_Animate = false;
		m_Network = false;
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

		vector buoyancy = -dGetGravity(m_Vehicle) * pState.m_Mass * m_Depth * 0.5;

		vector drag = -m_Velocity * pState.m_Mass * 0.25;

		vector force = (buoyancy + drag).InvMultiply3(pState.m_Transform);

		pState.DBGDrawImpulseMS(m_Position, force);

		pState.m_Force += force.Multiply3(pState.m_Transform);
		pState.m_Torque += (m_Position * force).Multiply3(pState.m_Transform);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Depth", m_Depth);

		return true;
	}
#endif
};
