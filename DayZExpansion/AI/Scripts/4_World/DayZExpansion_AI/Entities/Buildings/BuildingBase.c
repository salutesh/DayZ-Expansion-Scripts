class ExpansionLadder
{
	string m_Name;
	int m_Index;
	string m_Type;
	//int m_PosCount;
	int m_ConCount;
	int m_ConDirCount;
	//vector m_Pos[2];
	vector m_Con[2];
	vector m_ConDir[2];
	//vector m_Btm;
	//vector m_Top;
	//vector m_Dir;

	void ExpansionLadder(string name, int index, string type)
	{
		m_Name = name;
		m_Index = index;
		m_Type = type;
	}

	static void InsertVertex(vector vertex, inout vector target[2], inout int count)
	{
		if (count == 0)
		{
			target[count++] = vertex;
		}
		else
		{
			vector v0 = target[0];

			if (vertex[1] < v0[1])
			{
				target[0] = vertex;
				target[1] = v0;
			}
			else
			{
				target[1] = vertex;
			}
		}
	}
}

[eAIRegisterDynamicPatrolSpawner(BuildingBase)]
modded class BuildingBase
{
	static ref TStringArray s_eAI_PreventClimb;
	static ref map<string, ref map<int, ref ExpansionLadder>> s_Expansion_BuildingsWithLadders = new map<string, ref map<int, ref ExpansionLadder>>;

	ref eAIDynamicPatrolSpawner<BuildingBase> m_eAI_DynamicPatrolSpawner;
	ref map<int, int> m_eAI_LastDoorInteractionTime = new map<int, int>;
	ref map<int, ref eAIDoorTargetInformation> m_eAI_DoorTargetInformation;
	bool m_eAI_PreventClimb;
	ref map<int, float> m_eAI_DoorAnimationTime;
	ref map<int, ref ExpansionLadder> m_Expansion_Ladders;
	int m_Expansion_LaddersCount = -1;

	void BuildingBase()
	{
		if (g_Game.IsServer())
			m_eAI_DynamicPatrolSpawner = new eAIDynamicPatrolSpawner<BuildingBase>(this);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (!g_Game.IsServer())
			return;

		//! Do not init patrols on CrashBase unless created by CE (EEOnCECreate/AfterStoreLoad will take care of init in that case)
		if (!IsInherited(CrashBase) && m_eAI_DynamicPatrolSpawner)
		{
			m_eAI_DynamicPatrolSpawner.Init();
		}

		if (!s_eAI_PreventClimb)
		{
			//! Hardcoded defaults (AI will vault those buildings' edges otherwise or climb on ledges unnecessarily)
			//! Entries ending with $ will match to end, others can match anywhere
			//! "foo_bar" will match "foo_bar", "xyz_foo_bar_baz" etc.
			//! "foo_bar$" will match "foo_bar", "xyz_foo_bar", but not "foo_bar_baz" etc.
			s_eAI_PreventClimb = {
				"land_boat_",  //! Sakhal, pathfinding won't find a path off the boat
				"land_busstop",
				"land_camp_house",
				"land_castle_bergfrit",
				//"land_cementworks",
				"land_city_firestation",
				"land_city_stand",
				"land_container",
				"land_dieselpowerplant_tank_big",
				//"land_farm_cowsheda",
				//"land_farm_cowshedb",
				//"land_farm_cowshedc",
				"land_garage",
				//"land_geoplant_coolingstack",  //! Sakhal
				//"land_geoplant_mainhall_right",  //! Sakhal
				"land_guardhouse",
				"land_house",
				"land_lighthouse",
				"land_misc_polytunnel",
				"land_misc_greenhouse",
				"land_misc_trailroof",
				"land_mil_airfield_hq",
				"land_mil_atc",
				"land_mil_barracks",
				"land_mil_camonet_roof",
				"land_mil_firestation",
				"land_mil_guardbox",
				"land_mil_guardhouse",
				"land_mil_guardshed",
				"land_mil_tent",
				"land_office1",
				"land_office2",
				"land_pier_crane2_base",  //! Sakhal
				"land_rail_station",
				"land_shed",
				"land_slum_house6",
				"land_tank_big3",  //! Sakhal
				"land_tenement",
				"land_train_wagon_box",  //! Pathfinding won't find a path out of the wagon, that's the only reason it's excluded
				"land_village_healthcare",
				"land_village_store",
				"land_wall_fenforest_gate",
				"land_wall_gate_feng$",
				"land_wall_gate_feng_open$",
				"land_wall_gate_fenr$",
				"land_wall_gate_fenr_open$",
				"land_wall_gate_opendam",
				"land_wall_gate_wood",
				"land_warheadstorage_entrance",  //! Sakhal
				"land_water_station",
				"land_workshop",
				"sign"
			};

			s_eAI_PreventClimb.InsertAll(GetExpansionSettings().GetAI().PreventClimb);
		}

		string type = GetType();
		type.ToLower();
		foreach (string preventClimb: s_eAI_PreventClimb)
		{
			int lastChar = preventClimb.Length() - 1;
			if (preventClimb[lastChar] == "$")
			{
				if (preventClimb.Substring(0, lastChar).IndexOf(type) >= 0)
				{
					m_eAI_PreventClimb = true;
					break;
				}
			}
			else if (type.IndexOf(preventClimb) >= 0)
			{
				m_eAI_PreventClimb = true;
				break;
			}
		}
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_eAI_DynamicPatrolSpawner.Init();
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_eAI_DynamicPatrolSpawner.Init();
	}

#ifdef DIAG_DEVELOPER
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		EXTrace.PrintHit(EXTrace.AI, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
#endif

	eAIDoorTargetInformation eAI_GetDoorTargetInformation(int doorIndex, vector position)
	{
		if (!m_eAI_DoorTargetInformation)
			m_eAI_DoorTargetInformation = new map<int, ref eAIDoorTargetInformation>;

		eAIDoorTargetInformation info;
		if (!m_eAI_DoorTargetInformation.Find(doorIndex, info))
		{
			info = new eAIDoorTargetInformation(this, doorIndex, position);
			m_eAI_DoorTargetInformation[doorIndex] = info;
		}

		return info;
	}

	float eAI_GetDoorAnimationTime(int doorIndex)
	{
		float time;

		if (!m_eAI_DoorAnimationTime)
			m_eAI_DoorAnimationTime = new map<int, float>;

		if (!m_eAI_DoorAnimationTime.Find(doorIndex, time))
		{
			string path = CFG_VEHICLESPATH + " " + GetType() + " Doors";
			int count = g_Game.ConfigGetChildrenCount(path);

			if (doorIndex < count)
			{
				string name;
				g_Game.ConfigGetChildName(path, doorIndex, name);
				time = g_Game.ConfigGetFloat(path + " " + name + " animPeriod");
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.AI, this, "eAI_GetDoorAnimationTime(" + doorIndex + ") " + GetType() + " Doors " + name + " animPeriod " + time);
			#endif
			}

			time = Math.Max(time, 1.0);

			m_eAI_DoorAnimationTime[doorIndex] = time;
		}

		return time;
	}

	vector eAI_GetRoamingPosition(vector position)
	{
		vector destinationPosition = GetPosition();

		vector minMax[2];
		if (!Expansion_IsEnterable() && GetCollisionBox(minMax))
		{
			minMax[0][1] = 0.0;
			minMax[1][1] = 0.0;
			float extension = vector.Distance(minMax[0], minMax[1]) * 0.5 + 1.0;

			position[1] = destinationPosition[1];
			vector dir = vector.Direction(position, destinationPosition);
			destinationPosition = destinationPosition - dir.Normalized() * extension;
		}

		return destinationPosition;
	}

	/**
	 * @brief If this is a building, return whether it can be entered or not
	 */
	bool Expansion_IsEnterable()
	{
		//! If it has no doors, assume not enterable
		if (GetDoorCount() == 0)
			return false;

		//! Ugly to have to hardcode this, but I fail to see a better way
		string type = GetType();
		type.ToLower();
		if (type.IndexOf("land_wreck_") == 0)
		{
			if (type.IndexOf("land_wreck_caravan") == 0)
				return true;

			if (type.IndexOf("land_wreck_v3s") == 0)
				return true;

			return false;
		}

		return true;
	}

	bool Expansion_IsWreck()
	{
		string type = GetType();
		type.ToLower();
		if (type.IndexOf("land_wreck_") == 0)
			return true;

		return false;
	}

	bool Expansion_IsUndergroundEntrance()
	{
		return false;
	}

	//! Vanilla GetLaddersCount seems to always return 0?
	int Expansion_GetLaddersCount()
	{
		if (m_Expansion_LaddersCount != -1)
			return m_Expansion_LaddersCount;

		string type = GetType();
		if (s_Expansion_BuildingsWithLadders.Find(type, m_Expansion_Ladders))
		{
			m_Expansion_LaddersCount = m_Expansion_Ladders.Count();
			return m_Expansion_LaddersCount;
		}

	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, type);
	#endif

		m_Expansion_Ladders = new map<int, ref ExpansionLadder>;
		s_Expansion_BuildingsWithLadders[type] = m_Expansion_Ladders;
		m_Expansion_LaddersCount = 0;

		int i;

		LOD memory = GetLODByName(LOD.NAME_MEMORY);
		if (memory)
		{
			array<Selection> selections = {};
			if (memory.GetSelections(selections))
			{
				LOD geometry = GetLODByName(LOD.NAME_GEOMETRY);
				string ladderType = "metal";

				for (i = 0; i < geometry.GetPropertyCount(); i++)
				{
					if (geometry.GetPropertyName(i) == "laddertype")
					{
						ladderType = geometry.GetPropertyValue(i);
						break;
					}
				}

				ExpansionLadder currentLadder;

				foreach (Selection selection: selections)
				{
					string name = selection.GetName();

					if (name.IndexOf("ladder") == 0)
					{
						string ladderName;
						int ladderIndex = -1;

						int underscoreIndex = name.IndexOf("_");
						if (underscoreIndex > 6)
						{
							ladderName = name.Substring(0, underscoreIndex);
							ladderIndex = name.Substring(6, underscoreIndex - 6).ToInt();
						}
						else if (name.Length() > 6)
						{
							ladderName = name;
							ladderIndex = name.Substring(6, name.Length() - 6).ToInt();
						}

						if (ladderIndex > -1)
						{
							if (!m_Expansion_Ladders.Find(ladderIndex, currentLadder))
							{
								currentLadder = new ExpansionLadder(ladderName, ladderIndex, ladderType);
								m_Expansion_Ladders[ladderIndex] = currentLadder;

							#ifdef DIAG_DEVELOPER
								EXTrace.Print(EXTrace.AI, this, ladderName);
								EXTrace.Print(EXTrace.AI, this, " index " + ladderIndex);
							#endif
							}

						#ifdef DIAG_DEVELOPER
							EXTrace.Print(EXTrace.AI, this, " " + name);
						#endif

							int vertexCount = selection.GetVertexCount();
							for (i = 0; i < vertexCount; i++)
							{
								int vertexIndex = selection.GetLODVertexIndex(i);
								if (vertexIndex > -1)
								{
									vector vertex = memory.GetVertexPosition(vertexIndex);

									switch (name)
									{
										//case ladderName:
											//currentLadder.InsertVertex(vertex, currentLadder.m_Pos, currentLadder.m_PosCount);
											//break;
										//case ladderName + "_bottom_front":
											//currentLadder.m_Btm = vertex;
											//break;
										//case ladderName + "_top_front":
											//currentLadder.m_Top = vertex;
											//break;
										case ladderName + "_con":
											currentLadder.InsertVertex(vertex, currentLadder.m_Con, currentLadder.m_ConCount);
											break;
										case ladderName + "_con_dir":
											currentLadder.InsertVertex(vertex, currentLadder.m_ConDir, currentLadder.m_ConDirCount);
											break;
										//case ladderName + "_dir":
											//currentLadder.m_Dir = vertex;
											//break;
									}

								#ifdef DIAG_DEVELOPER
									EXTrace.Print(EXTrace.AI, this, "  vertex " + vertex);
								#endif
								}
							}
						}
					}
				}

			#ifdef DIAG_DEVELOPER
				foreach (ExpansionLadder ladder: m_Expansion_Ladders)
				{
					EXTrace.Print(EXTrace.AI, this, ladder.m_Name);
					EXTrace.Print(EXTrace.AI, this, " index " + ladder.m_Index);
					EXTrace.Print(EXTrace.AI, this, " type " + ladder.m_Type);
					EXTrace.Print(EXTrace.AI, this, " con[0] " + ladder.m_Con[0]);
					EXTrace.Print(EXTrace.AI, this, " con[1] " + ladder.m_Con[1]);
					EXTrace.Print(EXTrace.AI, this, " con_dir[0] " + ladder.m_ConDir[0]);
					EXTrace.Print(EXTrace.AI, this, " con_dir[1] " + ladder.m_ConDir[1]);
				}
			#endif

				m_Expansion_LaddersCount = m_Expansion_Ladders.Count();
			}
		}

		return m_Expansion_LaddersCount;
	}
};

//! Bit annoying to have to do this, it's just for error checking of AI patrol settings which uses IsKindOf
//! and BuildingBase is not an rVConfig type...
[eAIRegisterDynamicPatrolSpawner(House)]
modded class House
{
};
