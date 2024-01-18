enum ExpansionVehicleAerofoilType
{
	Fixed = 0,
	Wing,
	Rudder,
	Elevator
};

enum ExpansionVehicleAerofoilTypeL
{
	fixed = 0,
	wing,
	rudder,
	elevator
};

class ExpansionVehicleAerofoil : ExpansionVehicleModule
{
	string m_Name;

	ExpansionVehicleAerofoilType m_Type;

	string m_Animation;

	vector m_Up;

	float m_Area;
	float m_Camber;
	float m_MaxControlAngle;
	float m_StallAngle;

	vector m_AirFlowNormal;
	float m_AirflowMagnitudeSq;

	float m_AngleOfAttack;

	bool m_Stalling;

	float m_LiftAngle;
	float m_DragAngle;

	float m_LiftCoef;
	float m_DragCoef;
	float m_PressureCoef;

	float m_Input;
	float m_Angle;
	int m_Flap;

	void ExpansionVehicleAerofoil(EntityAI pVehicle, string pName)
	{
		m_Name = pName;

		string rootPath = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Aerofoils " + m_Name;

		string path;
		int type;

		path = rootPath + " type";
		type = GetGame().ConfigGetType(path);
		if (type == CT_STRING)
		{
			string typeName;
			GetGame().ConfigGetText(path, typeName);
			typeName.ToLower();
			type = typename.StringToEnum(ExpansionVehicleAerofoilTypeL, typeName);
			if (type != -1)
				m_Type = type;
		}
		else
		{
			m_Type = GetGame().ConfigGetInt(path);
		}

		vector min, max;

		path = rootPath + " up";
		m_Up = GetGame().ConfigGetTextOut(path).ToVector();

		path = rootPath + " min";
		min = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		path = rootPath + " max";
		max = m_Vehicle.GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		m_Position = vector.Lerp(min, max, 0.5);

		min[0] = min[0] * (1.0 - Math.AbsFloat(m_Up[0]));
		min[1] = min[1] * (1.0 - Math.AbsFloat(m_Up[1]));
		min[2] = min[2] * (1.0 - Math.AbsFloat(m_Up[2]));

		max[0] = max[0] * (1.0 - Math.AbsFloat(m_Up[0]));
		max[1] = max[1] * (1.0 - Math.AbsFloat(m_Up[1]));
		max[2] = max[2] * (1.0 - Math.AbsFloat(m_Up[2]));

		vector size;

		size[0] = Math.AbsFloat(max[0] - min[0]);
		size[1] = Math.AbsFloat(max[1] - min[1]);
		size[2] = Math.AbsFloat(max[2] - min[2]);

		m_Area = 1.0;

		if (size[0] != 0)
			m_Area *= size[0];

		if (size[1] != 0)
			m_Area *= size[1];

		if (size[2] != 0)
			m_Area *= size[2];

		path = rootPath + " camber";
		m_Camber = GetGame().ConfigGetFloat(path);

		path = rootPath + " maxControlAngle";
		m_MaxControlAngle = GetGame().ConfigGetFloat(path);

		path = rootPath + " stallAngle";
		m_StallAngle = GetGame().ConfigGetFloat(path);

		path = rootPath + " animation";
		if (GetGame().ConfigIsExisting(path))
			m_Animation = GetGame().ConfigGetTextOut(path);

		m_SelfDebugWindow = true;
		
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = false;
		m_PreSimulate = true;
		m_Simulate = true;
		m_Animate = true;
		m_Network = false;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Name", m_Name);
		instance.Add("Type", m_Type, ExpansionVehicleAerofoilType);
		instance.Add("Stalling", m_Stalling);
		instance.Add("Lift Coef", m_LiftCoef);
		instance.Add("Drag Coef", m_DragCoef);
		instance.Add("Pressure Coef", m_PressureCoef);
		instance.Add("Area", m_Area);

		return true;
	}
#endif

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_AirFlowNormal = -pState.GetModelVelocityAt(m_Position);
		m_AirflowMagnitudeSq = m_AirFlowNormal.Normalize();
		m_AirflowMagnitudeSq *= m_AirflowMagnitudeSq;

		switch (m_Type)
		{
		case ExpansionVehicleAerofoilType.Rudder:
			m_Input = -m_Controller.m_Yaw;
			break;

		case ExpansionVehicleAerofoilType.Elevator:
			m_Input = m_Controller.m_Pitch;
			break;

		case ExpansionVehicleAerofoilType.Wing:
			if (m_Position[0] < 0.0)
			{
				m_Input = -m_Controller.m_Roll;
			}
			else
			{
				m_Input = m_Controller.m_Roll;
			}
			break;
		}

		if (Math.AbsFloat(m_AirflowMagnitudeSq) < 0.0001)
			return;

		m_AngleOfAttack = Math.Asin(vector.Dot(m_Up, m_AirFlowNormal)) * Math.RAD2DEG;

		m_Angle = (m_Input * m_MaxControlAngle) + m_Camber;

		m_PressureCoef = m_Area * 0.5 * Expansion_GetDensity(pState.m_Transform[3]) * m_AirflowMagnitudeSq;

		m_LiftAngle = m_AngleOfAttack + m_Angle;

		m_LiftCoef = 0.0;
		if (Math.AbsFloat(m_LiftAngle) <= (m_StallAngle * 2.0))
			m_LiftCoef = Math.Sin(m_LiftAngle * (Math.PI * 0.5) / m_StallAngle) * 2.0;

		m_DragAngle = m_AngleOfAttack;
		if (m_DragAngle > 90.0)
			m_DragAngle = 180.0 - m_DragAngle;
		else if (m_DragAngle < -90.0)
			m_DragAngle = -180.0 - m_DragAngle;

		m_DragCoef = (m_DragAngle + m_Angle) / (m_StallAngle + Math.AbsFloat(m_Angle));
		m_DragCoef = 0.05 + (m_DragCoef * m_DragCoef);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		if (Math.AbsFloat(m_AirflowMagnitudeSq) < 0.0001)
			return;

		vector force = (m_Up * m_PressureCoef * m_LiftCoef) + (m_AirFlowNormal * m_PressureCoef * m_DragCoef);
		force = force * pState.m_DeltaTime;

		vector position = m_Position;
		if (m_Type == ExpansionVehicleAerofoilType.Wing)
		{
			m_Stalling = m_AngleOfAttack > m_StallAngle;

			if (m_Stalling)
				position[1] = position[1] + (((m_AngleOfAttack - m_StallAngle) * 1.0 + 2.0));
			else
				position[1] = position[1] + (((m_StallAngle - m_AngleOfAttack) * 2.0 / m_StallAngle + 2.0));
		}

		pState.DBGDrawSphereMS(position, 0.3, 0xFFFFFFFF);
		pState.DBGDrawSphereMS(m_Position, 0.5, 0xFFFF00FF);

		pState.DBGDrawImpulseMS(position, force);

		pState.m_Force += force.Multiply3(pState.m_Transform);
		pState.m_Torque += (position * force).Multiply3(pState.m_Transform);
	}

	override void Animate(ExpansionPhysicsState pState, float deltaTime)
	{
		m_Vehicle.SetAnimationPhase(m_Animation, m_Input);
	}
};
