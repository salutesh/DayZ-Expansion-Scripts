/**
 * ExpansionVehicleProp.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehiclePropType
{
	THRUSTER,
	WATER
};

class ExpansionVehicleProp : ExpansionVehicleModule
{
	int m_EngineIndex = 0;

	vector m_Direction;

	float m_MaxYaw;
	float m_MaxPitch;

	float m_RotorPosition;
	bool m_UseBlur;

	float m_RotorRadius;

	string m_Animation;
	string m_YawAnimation;
	string m_PitchAnimation;

	string m_HideRotor;
	string m_HideRotorBlur;

	ExpansionVehiclePropType m_Type;

	vector m_Velocity;

	void ExpansionVehicleProp(EntityAI vehicle, string rootPath)
	{
		m_RotorRadius = 1.0;

		string path;
		int type;

		path = rootPath + " type";
		if (GetGame().ConfigIsExisting(path))
		{
			int configType = GetGame().ConfigGetType(path);
			if (configType == CT_STRING)
			{
				string typeName;
				GetGame().ConfigGetText(path, typeName);
				typeName.ToUpper();
				m_Type = typename.StringToEnum(ExpansionVehiclePropType, typeName);
			}
			else
			{
				m_Type = GetGame().ConfigGetInt(path);
			}
		}

		path = rootPath + " engine";
		if (GetGame().ConfigIsExisting(path))
			m_EngineIndex = GetGame().ConfigGetInt(path);

		path = rootPath + " start";
		vector start = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		path = rootPath + " end";
		vector end = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		path = rootPath + " maxYaw";
		if (GetGame().ConfigIsExisting(path))
			m_MaxYaw = GetGame().ConfigGetFloat(path);

		path = rootPath + " maxPitch";
		if (GetGame().ConfigIsExisting(path))
			m_MaxPitch = GetGame().ConfigGetFloat(path);

		path = rootPath + " radius";
		if (GetGame().ConfigIsExisting(path))
			m_RotorRadius = GetGame().ConfigGetFloat(path);

		path = rootPath + " animation";
		if (GetGame().ConfigIsExisting(path))
			m_Animation = GetGame().ConfigGetTextOut(path);

		path = rootPath + " yawAnimation";
		if (GetGame().ConfigIsExisting(path))
			m_YawAnimation = GetGame().ConfigGetTextOut(path);

		path = rootPath + " pitchAnimation";
		if (GetGame().ConfigIsExisting(path))
			m_PitchAnimation = GetGame().ConfigGetTextOut(path);

		path = rootPath + " hiderotor";
		if (GetGame().ConfigIsExisting(path))
			m_HideRotor = GetGame().ConfigGetTextOut(path);

		path = rootPath + " hiderotorblur";
		if (GetGame().ConfigIsExisting(path))
			m_HideRotorBlur = GetGame().ConfigGetTextOut(path);

		m_Position = start;
		m_Direction = vector.Direction(start, end).Normalized();

		m_SelfDebugWindow = true;
		
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = false;
		m_PreSimulate = true;
		m_Simulate = true;
		m_Animate = true;
		m_Network = false;
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Velocity = pState.GetModelVelocityAt(m_Position);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		float torque = m_Controller.m_Torque[m_EngineIndex];
		vector direction = m_Direction;

		float yaw = m_Controller.m_Yaw * m_MaxYaw;
		float pitch = m_Controller.m_Pitch * m_MaxPitch;

		if (yaw != 0 || pitch != 0)
		{
			vector controlledTransform[4];
			Math3D.YawPitchRollMatrix(Vector(yaw, pitch, 0), controlledTransform);
			direction = direction.Multiply3(controlledTransform);
			m_Velocity = m_Velocity.Multiply3(controlledTransform);
		}
		
		vector force = direction * (torque / m_RotorRadius) * pState.m_Mass;

		float rps = m_Controller.m_RPM[m_EngineIndex] / 60.0;
		m_UseBlur = false;
		if (rps >= 1.0)
			m_UseBlur = true;

		m_RotorPosition += rps * pState.m_DeltaTime;
		if (m_RotorPosition >= 1.0)
			m_RotorPosition -= 1.0;

		//float rpm = (Math.AbsFloat(m_Velocity[2]) * 30.0) / (Math.PI * m_RotorRadius);
		//m_Controller.m_RPM[m_EngineIndex] = Math.Max(rpm, m_Controller.m_RPM[m_EngineIndex]);

		pState.DBGDrawLineDirectionMS(m_Position, direction);

		pState.DBGDrawImpulseMS(m_Position, force);
		pState.DBGDrawSphereMS(m_Position, 0.2);

		pState.m_Force += force.Multiply3(pState.m_Transform);
		pState.m_Torque += (m_Position * force).Multiply3(pState.m_Transform);
	}

	override void Animate(ExpansionPhysicsState pState, float deltaTime)
	{
		if (m_UseBlur)
		{
			m_Vehicle.ShowSelection(m_HideRotorBlur);
			m_Vehicle.HideSelection(m_HideRotor);
		}
		else
		{
			m_Vehicle.HideSelection(m_HideRotorBlur);
			m_Vehicle.ShowSelection(m_HideRotor);
		}

		m_Vehicle.SetAnimationPhase(m_Animation, m_RotorPosition);
		m_Vehicle.SetAnimationPhase(m_YawAnimation, m_Controller.m_Yaw * m_MaxYaw * Math.DEG2RAD);
		m_Vehicle.SetAnimationPhase(m_PitchAnimation, m_Controller.m_Pitch * m_MaxYaw * Math.DEG2RAD);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		//instance.Add();

		return true;
	}
#endif
};
