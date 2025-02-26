class ExpansionMissionEventContaminatedArea: ExpansionMissionEventBase
{
	ref JsonDataAreaData Data = new JsonDataAreaData();
	ref JsonDataPlayerData PlayerData = new JsonDataPlayerData();

	float StartDecayLifetime;
	float FinishDecayLifetime;

	[NonSerialized()] ExpansionContaminatedArea m_Entity;

	[NonSerialized()] autoptr ExpansionZone m_Zone;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventContaminatedArea
	// ------------------------------------------------------------
	void ExpansionMissionEventContaminatedArea()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		m_EventName = "ContaminatedArea";
	}

	override void Event_OnStart()
	{
		vector pos = Vector(Data.Pos[0], Data.Pos[1], Data.Pos[2]);
		int flag = ECE_NONE;

		if (pos[1] == 0)
		{
			pos[1] = GetGame().SurfaceRoadY(pos[0], pos[2]);
			flag = ECE_PLACE_ON_SURFACE;
		}

		Class.CastTo(m_Entity, GetGame().CreateObjectEx("ExpansionContaminatedArea", pos, flag));

		pos[1] = pos[1] - Data.NegHeight;

		m_Zone = new ExpansionZoneCylinder(ExpansionZoneType.CONTAMINATED, pos, Data.Radius, Data.NegHeight + Data.PosHeight);

		m_Entity.m_Radius = Data.Radius;
		m_Entity.m_PositiveHeight = Data.PosHeight;
		m_Entity.m_NegativeHeight = Data.NegHeight;
		m_Entity.m_InnerRings = Data.InnerRingCount;
		m_Entity.m_InnerSpacing = Data.InnerPartDist;
		m_Entity.m_OuterRingToggle = Data.OuterRingToggle;
		m_Entity.m_OuterSpacing = Data.OuterPartDist;
		m_Entity.m_OuterRingOffset = Data.OuterOffset;
		m_Entity.m_VerticalLayers = Data.VerticalLayers;
		m_Entity.m_VerticalOffset = Data.VerticalOffset;
		string particleName = Data.ParticleName;

		string aroundPartName = PlayerData.AroundPartName;
		string tinyPartName = PlayerData.TinyPartName;
		string ppeRequesterType = PlayerData.PPERequesterType;

		m_Entity.m_ParticleID = ParticleList.GetParticleID(particleName);
		m_Entity.m_AroundParticleID = ParticleList.GetParticleID(aroundPartName);
		m_Entity.m_TinyParticleID = ParticleList.GetParticleID(tinyPartName);
		m_Entity.m_PPERequesterType = ppeRequesterType;

		m_Entity.m_PPERequesterIdx = PPERequesterBank.GetRequester(m_Entity.m_PPERequesterType.ToType()).GetRequesterIDX();

		m_Entity.SetSynchDirty();
	}

	override void Event_OnEnd()
	{
		m_Zone = null;

		GetGame().ObjectDelete(m_Entity);
	}

	override void Event_OnUpdate(float delta)
	{
		if (GetMaxRemainingTime() < FinishDecayLifetime)
		{
			m_Entity.SetDecayState(eAreaDecayStage.DECAY_END);
			return;
		}

		if (GetMaxRemainingTime() < StartDecayLifetime)
		{
			m_Entity.SetDecayState(eAreaDecayStage.DECAY_START);
			return;
		}
	}

	override protected void OnLoadMission()
	{
		if (IsMissionHost())
		{
			JsonFileLoader<ExpansionMissionEventContaminatedArea>.JsonLoadFile(m_FileName, this);
		}
	}

	override protected void OnSaveMission()
	{
		if (IsMissionHost())
		{
			JsonFileLoader<ExpansionMissionEventContaminatedArea>.JsonSaveFile(m_FileName, this);
		}
	}

	override protected string OnDefaultMission(int index)
	{
		Enabled = false;

		PlayerData.AroundPartName = "graphics/particles/contaminated_area_gas_around";
		PlayerData.TinyPartName = "graphics/particles/contaminated_area_gas_around_tiny";
		PlayerData.PPERequesterType = "PPERequester_ContaminatedAreaTint";

		MissionMaxTime = 900;
		StartDecayLifetime = 600;
		FinishDecayLifetime = 300;

		Data.Radius = 100;
		Data.PosHeight = 22;
		Data.NegHeight = 10;
		Data.InnerRingCount = 1;
		Data.InnerPartDist = 50;
		Data.OuterRingToggle = 1;
		Data.OuterPartDist = 50;
		Data.OuterOffset = 0;
		Data.VerticalLayers = 0;
		Data.VerticalOffset = 0;
		Data.ParticleName = "graphics/particles/contaminated_area_gas_bigass";

		return ExpansionMissionContaminatedRandom( index );
	}

	override protected string DefaultMission( int index )
	{
		return ExpansionMissionContaminatedChernarus(index);
	}

	protected string ExpansionMissionContaminatedChernarus(int index)
	{
		if (index == 0)
		{
			MissionName = "Ship-SW";

			Data.Pos = {13684, 0, 11073};
			Data.Radius = 100;
			Data.PosHeight = 22;
			Data.NegHeight = 10;
			Data.InnerRingCount = 1;
			Data.InnerPartDist = 50;
			Data.OuterRingToggle = 1;
			Data.OuterPartDist = 50;
		}
		else if (index == 1)
		{
			MissionName = "Ship-NE";

			Data.Pos = {13881, 0, 11172};
			Data.Radius = 100;
			Data.PosHeight = 26;
			Data.NegHeight = 10;
			Data.InnerRingCount = 1;
			Data.InnerPartDist = 50;
			Data.OuterRingToggle = 1;
			Data.OuterPartDist = 50;
		}
		else if (index == 2)
		{
			MissionName = "Ship-Central";

			Data.Pos = {13752, 0, 11164};
			Data.Radius = 100;
			Data.PosHeight = 22;
			Data.NegHeight = 2;
			Data.InnerRingCount = 1;
			Data.InnerPartDist = 50;
			Data.OuterRingToggle = 1;
			Data.OuterPartDist = 60;
		}
		else if (index == 3)
		{
			MissionName = "Pavlovo-North";

			Data.Pos = {2043, 0, 3485};
			Data.Radius = 150;
			Data.PosHeight = 25;
			Data.NegHeight = 20;
			Data.InnerRingCount = 2;
			Data.InnerPartDist = 50;
			Data.OuterRingToggle = 1;
			Data.OuterPartDist = 40;
		}
		else if (index == 4)
		{
			MissionName = "Pavlovo-South";

			Data.Pos = {2164, 0, 3335};
			Data.Radius = 150;
			Data.PosHeight = 11;
			Data.NegHeight = 10;
			Data.InnerRingCount = 2;
			Data.InnerPartDist = 50;
			Data.OuterRingToggle = 1;
			Data.OuterPartDist = 40;
		}

		string fname = MissionName;
		fname.Replace(" ", "-");
		return fname;
	}

	string ExpansionMissionContaminatedRandom(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		ExpansionLocation loc;
		string fname = RandomMission(idx, loc);
		if (!loc)
			return fname;

		Data.Pos = {loc.Position[0], 0, loc.Position[2]};
		Data.Radius = loc.Radius;
		Data.PosHeight = 26;
		Data.NegHeight = 10;
		Data.InnerRingCount = 1;
		Data.InnerPartDist = 50;
		Data.OuterRingToggle = 1;
		Data.OuterPartDist = 50;

		return fname;
	}
};
