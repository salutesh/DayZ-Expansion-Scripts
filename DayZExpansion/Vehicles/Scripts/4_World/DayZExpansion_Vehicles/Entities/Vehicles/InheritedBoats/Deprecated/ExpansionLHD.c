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
		
		#ifndef DAYZ_1_18
		//! 1.19
		//! Sound set unsuitable for short horn sound (breaks player interaction and infinite bugged playback)
		m_CarHornLongSoundName	= "Expansion_LHD_Horn_Ext_SoundSet";
		#endif

		if (IsMissionHost())
			SetAllowDamage(false);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (!IsMissionHost())
			return;

		ExpansionLHD_CreatePart("ExpansionLHD1", "LHD_1");
		ExpansionLHD_CreatePart("ExpansionLHD2", "LHD_2");
		ExpansionLHD_CreatePart("ExpansionLHD3", "LHD_3");
		ExpansionLHD_CreatePart("ExpansionLHD4", "LHD_4");
		ExpansionLHD_CreatePart("ExpansionLHD5", "LHD_5");
		ExpansionLHD_CreatePart("ExpansionLHD6", "LHD_6");

		ExpansionLHD_CreatePart("ExpansionLHDHouse1", "LHD_House_1");
		ExpansionLHD_CreatePart("ExpansionLHDHouse2", "LHD_House_2");

		ExpansionLHD_CreatePart("ExpansionLHDInterior1", "LHD_Interior_1");
		ExpansionLHD_CreatePart("ExpansionLHDInterior2", "LHD_Interior_2");
		ExpansionLHD_CreatePart("ExpansionLHDInterior3", "LHD_Interior_3");

		//ExpansionLHD_CreatePart("ExpansionLHDElevatorL", "LHD_Elevator_L");
		//ExpansionLHD_CreatePart("ExpansionLHDElevatorR", "LHD_Elevator_R");

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Update, 5000, true);

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
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

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
	
	override void EOnSimulate(IEntity other, float dt)
	{
		if (GetGame().IsServer())
		{
			vector min = ModelToWorld(m_SizeMin);
			vector max = ModelToWorld(m_SizeMax);

		//	GetGame().UpdatePathgraphRegion(min, max);
		}
	}

	private void ExpansionLHD_CreatePart(string part, string memoryPoint)
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
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "OnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif
	}
};
