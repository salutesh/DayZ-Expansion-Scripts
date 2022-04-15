class ExpansionLHD : ExpansionZodiacBoat
{
	autoptr array<Object> m_Parts = new array<Object>();

	vector m_SizeMin;
	vector m_SizeMax;

	void ExpansionLHD()
	{
		//! Vanilla
		m_dmgContactCoef = 0.018;

		//! Custom
		m_MaxSpeed = 25.0;

		m_TurnCoef = 0.2;

		m_Offset = 4.0;

		SetAllowDamage(false);

		if (!IsMissionHost()) return;

		Create("ExpansionLHD1", "LHD_1");
		Create("ExpansionLHD2", "LHD_2");
		Create("ExpansionLHD3", "LHD_3");
		Create("ExpansionLHD4", "LHD_4");
		Create("ExpansionLHD5", "LHD_5");
		Create("ExpansionLHD6", "LHD_6");

		Create("ExpansionLHDHouse1", "LHD_House_1");
		Create("ExpansionLHDHouse2", "LHD_House_2");

		Create("ExpansionLHDInterior1", "LHD_Interior_1");
		Create("ExpansionLHDInterior2", "LHD_Interior_2");
		Create("ExpansionLHDInterior3", "LHD_Interior_3");

		//Create("ExpansionLHDElevatorL", "LHD_Elevator_L");
		//Create("ExpansionLHDElevatorR", "LHD_Elevator_R");

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateModels, 5000, true);

		float lengthF = 128.0;
		float lengthB = 128.0;
		float width = 20.0;
		float height = 50.0;
		float depth = 20.0;

		m_SizeMin = Vector(width, depth, lengthB);
		m_SizeMax = Vector(width, height, lengthF);

		Update();
	}

	// ------------------------------------------------------------
	void ~ExpansionLHD()
	{
		if (!IsMissionHost()) return;

		foreach (auto part : m_Parts)
		{
			part.Delete();
		}
	}

	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_LHD;
	}
	
	override void EOnSimulate(IEntity owner, float dt)
	{
		if (GetGame().IsServer())
		{
			vector min = ModelToWorld(m_SizeMin);
			vector max = ModelToWorld(m_SizeMax);

		//	GetGame().UpdatePathgraphRegion(min, max);
		}
	}

	private void DeletePart(out Object obj)
	{
		if (obj)
		{
			GetGame().ObjectDelete(obj);
		}

		obj = NULL;
	}

	private Object Create(string part, string memoryPoint)
	{
		vector position = GetMemoryPointPos(memoryPoint);

		vector transform[3];
		Math3D.YawPitchRollMatrix("180 0 0", transform);

		position = position.InvMultiply3(transform);

		Object obj = GetGame().CreateObject(part, "0 0 0");

		obj.SetPosition(position);
		//obj.SetOrientation("180 0 0");

		AddChild(obj, -1, false);

		obj.Update();

		m_Parts.Insert(obj);

		return obj;
	}

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		return true;
	}

	override float GetCameraHeight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraHeight");
#endif

		return 40;
	}

	override float GetCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraDistance");
#endif

		return 200;
	}

	override void EOnContact(IEntity other, Contact extra)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "EOnContact").Add(other).Add(extra);
#endif
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "EOnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif
	}

	void UpdateModels()
	{
		this.Update();
	}
};
