#ifdef EXPANSION_MISSION_AI_ENABLE
class ExpansionMissionEventAI: ExpansionMissionEventBase
{
	ref ExpansionAIMissionMeta MissionMeta;

	string MappingFile;

#ifdef EXPANSIONMODAI
	ref array< ref ExpansionAIPatrol > AIPatrols;
#endif

	ref array < ref ExpansionAIMissionInfected > Animals;
	ref array< ref ExpansionAIMissionLoot > LootLocations;

	[NonSerialized()]
	ref ExpansionObjectSet MappingSet;

	[NonSerialized()]
	autoptr array< EntityAI > m_Containers;

#ifdef EXPANSIONMODAI
	[NonSerialized()]
	autoptr array< eAIDynamicPatrol > m_Soldiers;

	[NonSerialized()]
	ExpansionAIPatrolManager AIPatrolManager;
#endif

#ifdef EXPANSIONMODNAVIGATION
	[NonSerialized()]
	ExpansionMarkerModule m_MarkerModule;

	[NonSerialized()]
	ExpansionMarkerData m_ServerMarker;
#endif

	[NonSerialized()]
	bool m_MissionEnd;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAI
	// ------------------------------------------------------------
	void ExpansionMissionEventAI()
	{
		m_EventName = "AI";

	#ifdef EXPANSIONMODAI
		AIPatrols = new array< ref ExpansionAIPatrol >;
		m_Soldiers = new array< eAIDynamicPatrol >;
	#endif
		
		Animals = new array< ref ExpansionAIMissionInfected >;

		LootLocations = new array< ref ExpansionAIMissionLoot >;
		m_Containers = new array< EntityAI >;
	}

	void CreateNotif(ExpansionSettingNotificationData notifdata)
	{
		if ( notifdata && notifdata.Title != "" )
			ExpansionNotification(new StringLocaliser(notifdata.Title), new StringLocaliser(notifdata.Description), notifdata.Icon, notifdata.Get("Color"), 7).Create();
	}
	
	override void Event_OnStart()
	{
		m_MissionEnd = false;

		if ( GetExpansionSettings().GetNotification().ShowAIMissionStarted )
			CreateNotif(MissionMeta.NotificationStart);

		MappingSet = new ExpansionObjectSet(EXPANSION_MISSION_MISSIONOBJECTS_FOLDER,MappingFile);
		MappingSet.SpawnObjects();

	#ifdef EXPANSIONMODVEHICLE
		ExpansionCarKey key;
	#endif

		for ( int ll = 0; ll < LootLocations.Count(); ll++ ) 
		{
			ExpansionAIMissionLoot currloot = LootLocations.Get( ll );
			if (currloot)
			{
				ExpansionAIMissionContainer containerData = currloot.Containers.GetRandomElement();
				bool shouldLock = containerData.IsLocked;
				EntityAI container = EntityAI.Cast( SpawnObject(containerData.Classnames.GetRandomElement(), containerData.Position, containerData.Orientation) );
				Print("container =>"+container);
				ItemBase itembs;
				ExpansionVehicle vehicle;
				if (ExpansionVehicle.Get(vehicle, container))
				{
					vehicle.OnDebugSpawn();

					EntityAI vehicleEntity = vehicle.GetEntity();

					for (int att_i = 0; att_i < vehicleEntity.GetInventory().AttachmentCount(); ++att_i)
					{
						EntityAI attachment = vehicleEntity.GetInventory().GetAttachmentFromIndex(att_i);
						attachment.SetHealth01("", "Health", Math.RandomFloat(0.65, 1.0));
					}

					vehicle.LeakFluids();
					vehicle.FillFluids(Math.RandomFloat(0.0, 1.0));
				}
				else if (Class.CastTo(itembs, container))
				{
					itembs.Open();
				}

				ExpansionLootSpawner.SpawnLoot(container, currloot.Loot, currloot.ItemCount );

				if (itembs)
				{
					itembs.Close();

				#ifdef EXPANSIONMODBASEBUILDING
					if ( shouldLock )
					{
						if ( itembs.ExpansionFindCodeLockSlot() )
							itembs.GetInventory().CreateAttachment("ExpansionCodeLock");

						if ( itembs.ExpansionHasCodeLock() )
						{
							string code = itembs.GetPosition()[1].ToString();
							code.Substring(0, code.Length() - 4);
							itembs.SetCode(code);
							itembs.ExpansionLock();
						}
					}
				#endif
				}
			#ifdef EXPANSIONMODVEHICLE
				if (vehicle)
				{
					if ( shouldLock )
					{
						key = ExpansionCarKey.Cast( SpawnObject("ExpansionCarKey") );
						vehicle.PairKey(key);
						vehicle.Lock(key);
					}
				}
			#endif
				m_Containers.Insert(container);
			}
		}
		
	#ifdef EXPANSIONMODAI
		int pickedGroupId = Math.RandomInt(0, AIPatrols.Count() );

		for ( int sl = 0; sl < AIPatrols.Count(); sl++ ) 
		{
			ExpansionAIPatrol currAIPatrol = AIPatrols.Get( sl );
			if (currAIPatrol)
			{
				m_Soldiers.Insert(AIPatrolManager.InitPatrol(currAIPatrol));
			
			#ifdef EXPANSIONMODVEHICLE
				if ( pickedGroupId == sl )
				{
					int soldiercount = m_Soldiers.Count() - 1;
					int pickedSoldierId = Math.RandomInt(0, m_Soldiers.Count() );
					eAIGroup group = eAIGroup.Cast(m_Soldiers[soldiercount].m_Group);
					Print("soldiercount =>"+soldiercount);
					Print("pickedSoldierId =>"+pickedSoldierId);
					Print("m_Soldiers[soldiercount] =>"+m_Soldiers[soldiercount]);
					Print("group =>"+group);
					if (group)
					{
						DayZPlayerImplement member = DayZPlayerImplement.Cast(group.GetMember(pickedSoldierId));
						member.GetInventory().TakeEntityToCargo(InventoryMode.SERVER, key);
						Print("group.GetMember(pickedSoldierId) =>"+group.GetMember(pickedSoldierId));
						Print("member =>"+member);
					}
				}
			#endif
			}
		}
	#endif

		for ( int sa = 0; sa < Animals.Count(); sa++ ) 
		{
			break; //! Not Ready

			ExpansionAIMissionInfected currAnimal = Animals.Get( sa );
			if (currAnimal)
			{
				//SpawnInfected();
				//! TODO: move the airdrop infected spawn system to a more general system we can use in other missions
			}
		}

	#ifdef EXPANSIONMODNAVIGATION
		if (CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule))
			m_ServerMarker = m_MarkerModule.CreateServerMarker(MissionMeta.Marker.Name, EXPANSION_NOTIFICATION_ICON_AI_MISSION, MissionMeta.Marker.Position, COLOR_EXPANSION_NOTIFICATION_MISSION, true);
	#endif
		
		if ( GetExpansionSettings().GetNotification().ShowAIMissionEnded )
			CreateNotif(MissionMeta.NotificationEnd);
	}

	/*
	void SpawnInfected()
	{
		while ( m_InfectedCount < InfectedCount ) 
		{
			m_InfectedCount++;

			vector spawnPos = ExpansionMath.GetRandomPointInRing(m_Container.GetPosition(), InfectedSpawnRadius * 0.1, InfectedSpawnRadius);
			spawnPos[1] = GetGame().SurfaceY( spawnPos[0], spawnPos[2] );

			//! Have to convert vector to string for call queue

			int additionalDelay;
			if ( InfectedSpawnInterval > 0 )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( Send_SpawnParticle, InfectedSpawnInterval * m_InfectedCount, false, spawnPos.ToString( false ) );
				additionalDelay = Math.RandomFloat(100, 300);
			}

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( CreateSingleInfected, InfectedSpawnInterval * m_InfectedCount + additionalDelay, false, spawnPos.ToString( false ) );
		}
	}

	void CreateSingleInfected( string spawnPosStr )
	{
		vector spawnPos = spawnPosStr.ToVector();
		string type = Animals.Classnames.GetRandomElement();

		//! TODO: Create Z slightly in ground to give effect as if they emerge from underground? Also, is there a way to affect Z stance (crouching)?
		Object obj = GetGame().CreateObject( type, spawnPos, false, GetGame().IsKindOf(type, "DZ_LightAI") );

		if ( obj )
		{
			m_Infected.Insert( obj );
		} else {
			Print("[ExpansionAirdropContainerManager] Warning : '" + type + "' is not a valid type!");
		}
	}
	*/

	override bool CanEnd()
	{
		//! Check if a player is nearby any container in a 1100 meter radius
		foreach (auto container: m_Containers)
		{
			if (container && ExpansionLootSpawner.IsPlayerNearby(container, 1100))
				return false;
		}

		return true;
	}

	override void Event_OnEnd()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if ( IsMissionHost() )
		{
			foreach (auto container: m_Containers)
			{
				if (container)
					GetGame().ObjectDelete(container);
			}

			m_Containers.Clear();

		#ifdef EXPANSIONMODAI
			foreach (auto soldiers: m_Soldiers)
			{
				soldiers.Despawn();
				soldiers.Delete();
			}

			m_Soldiers.Clear();
		#endif

			//! TODO: Wipe the spawned Animals

		#ifdef EXPANSIONMODNAVIGATION
			if (m_ServerMarker && m_MarkerModule)
				m_MarkerModule.RemoveServerMarker(m_ServerMarker.GetUID());
		#endif

			if ( GetExpansionSettings().GetNotification().ShowAIMissionEnded )
				CreateNotif(MissionMeta.NotificationEnd);

			MappingSet.Delete();
		}
	}

	override void Event_OnUpdate( float delta )
	{
		if ( IsMissionHost() )
		{
			if (m_CurrentMissionTime >= MissionMaxTime && !m_MissionEnd)
			{
				m_MissionEnd = true;

				foreach (auto container: m_Containers)
				{
					if (container && container.GetHealth("", "") > 0.0)
					{
						container.SetHealth("", "", 0.0);
						container.SetLifetimeMax(0.0);
					}
				}
			}
		}
	}

	protected override void OnLoadMission()
	{
		JsonFileLoader<ExpansionMissionEventAI>.JsonLoadFile( m_FileName, this );
	}

	protected override void OnSaveMission()
	{
		JsonFileLoader<ExpansionMissionEventAI>.JsonSaveFile( m_FileName, this );
	}
	// ------------------------------------------------------------
	// Expansion MaxDefaultMissions
	// ------------------------------------------------------------
	override int MaxDefaultMissions()
	{
		switch (ExpansionStatic.GetCanonicalWorldName())
		{
			case "chernarusplus":
				return 6; //! amount of locations
			break;
			/*
			case "namalsk":
				return 1;
			break;
			case "enoch":
				return 12;
			break;
			case "deerisle":
				return 13;
			break;
			case "namalsk":
				return 9;
			break;
			case "chiemsee":
				return 13;
			break;
			case "sandbox":
				return 5;
			break;
			case "rostow":
				return 9; //! TODO
			break;
			case "esseker":
				return 9; //! TODO
			break;
			case "valning":
				return 9; //! TODO
			break;
			case "banov":
				return 9; //! TODO
			break;
			case "takistanplus":
				return 9; //! TODO
			break;
			*/
		}

		return 0;
	}

	protected override string OnDefaultMission( int index )
	{
		Enabled = true;

		Weight = 5;
		MissionMaxTime = 12000;
		
		switch (ExpansionStatic.GetCanonicalWorldName())
		{
			case "chernarusplus":
				return OnDefaultChernarusMission(index);
			break;
			/*
			case "namalsk":
				return OnDefaultNamalskMission(index);
			break;
			*/
		}

		return "Error_You_Shouldnt_Be_Seeing_This";
	}

	// ------------------------------------------------------------
	// Expansion OnDefaultChernarusMission
	// ------------------------------------------------------------
	string OnDefaultChernarusMission(int idx)
	{
		array<vector> patrol = new array<vector>();
		ExpansionAIMissionContainer container;
		array < ref ExpansionAIMissionContainer > containers 	= new array < ref ExpansionAIMissionContainer >;
		ExpansionSettingNotificationData notifStart 			= new ExpansionSettingNotificationData;
		ExpansionSettingNotificationData notifAction 			= new ExpansionSettingNotificationData;
		ExpansionSettingNotificationData notifEnd 				= new ExpansionSettingNotificationData;
		ExpansionSettingMarkerData mapMarker 					= new ExpansionSettingMarkerData;
		TStringArray Infected;
	
		switch ( idx )
		{
		default:
		case 0:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Bandit Ambush", "Bandits prepared a roadblock to ambush cars going toward Zaprudnoe !","Ambush","#b53128");
			notifAction = new ExpansionSettingNotificationData("Bandit Ambush", "Survivors are attacking the bandits !","Ambush","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Bandit Ambush", "The bandit camp is gone","Ambush","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Bandit Ambush","Ambush","#b53128", "5457.122070 173.275223 12304.987305", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

			MappingFile = "BanditRoadAmbush.map";
		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"5458.080566 173.637192 12304.130859"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"5463.901855 173.594833 12302.534180"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"5462.813965 173.794785 12292.244141"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"5466.251953 185.817581 12296.766602"}));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue"}, "5457.122070 173.275223 12304.987305"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 1:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Bandit Outpost", "Bandits made a outpost near Kranostav and are harrasing the local traders","Bandit","#b53128");
			notifAction = new ExpansionSettingNotificationData("Bandit Outpost", "Survivors are attacking the bandits !","Bandit","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Bandit Outpost", "The bandit outpost is gone","Bandit","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Bandit Outpost","Bandit","#b53128", "10813.508789 335.859680 12842.180664", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "HALT", "West", "", true, true, 1.0, 1, 1, -1, -10,{"10816.439453 365.125153 12851.209961"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10803.737305 336.157471 12852.415039"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10803.396484 335.701996 12857.082031"}));

			patrol = {"10797.850586 335.714447 12860.800781","10819.007813 334.957336 12868.475586","10829.625977 334.317841 12860.252930","10826.763672 334.054077 12835.985352","10802.163086 335.897369 12835.940430","10792.398438 335.709381 12850.662109"};
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,patrol));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue"}, "10813.508789 335.859680 12842.180664"));
			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue"}, "10810.214844 335.881378 12840.517578"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 2:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Protected Car", "PLACEHOLDER START","Car","#b53128");
			notifAction = new ExpansionSettingNotificationData("Protected Car", "PLACEHOLDER ACTION","Car","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Protected Car", "PLACEHOLDER END","Car","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Protected Car","Car","#b53128", "1935.177368 316.118530 8137.058105", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"1930.597534 316.202026 8104.156738"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"1926.229370 316.017944 8130.541504"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"1935.195068 315.958252 8122.743652"}));

			patrol = {"1935.177368 316.118530 8137.058105","1947.882202 317.576721 8123.168945","1951.465942 317.578827 8100.388184","1932.192871 315.088287 8093.985840","1910.182617 314.038513 8101.887695","1896.561523 312.488312 8120.151367","1906.121704 314.353638 8140.756348"};
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,patrol));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"CivilianSedan","CivilianSedan_Wine","CivilianSedan_Black"}, "1936.86 316.167 8116.66", "10 0 0"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 3:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Protected Truck", "PLACEHOLDER START","Car","#b53128");
			notifAction = new ExpansionSettingNotificationData("Protected Truck", "PLACEHOLDER ACTION","Car","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Protected Truck", "PLACEHOLDER END","Car","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Protected Truck","Car","#b53128", "8324.178711 292.289398 5974.528320", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"8519.078125 292.434692 6036.809082"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"8398.845703 292.025513 5978.773438"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"8278.727539 292.488708 5992.928711"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"8334.528320 292.495148 5980.446777"}));

			patrol = {"8326.836914 292.076965 5988.645508","8399.663086 292.019836 5980.704102","8395.004883 292.483154 6012.923828","8327.966797 292.219879 6019.990234","8258.790039 291.403717 5999.292969","8292.458008 291.741364 5961.176758","8370.194336 292.069519 5945.970703","8390.263672 292.264954 5991.466797","8323.718750 292.077789 5998.654297"};
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,patrol));

			patrol = {"8364.980469 292.219086 5996.721191","8352.618164 292.079529 5960.034668","8319.434570 292.086975 5953.710938","8300.264648 292.067444 5978.587402","8327.583008 292.100189 6001.889648","8366.969727 292.269257 6014.940430"};
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,patrol));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"Truck_01_Covered"}, "8324.178711 292.289398 5974.528320", "-79.88 -0.6868 -0.12599"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 4:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Helicrash Cargo", "A helicopter carrying crates crashed nearby Dubrovka !","","#b53128");
			notifAction = new ExpansionSettingNotificationData("Helicrash Cargo", "PLACEHOLDER ACTION","","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Helicrash Cargo", "PLACEHOLDER END","","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Helicrash Cargo","Helicopter","#b53128", "9070.771484 280.396698 10713.153320", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

			MappingFile = "HelicrashCargo.map";
		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "East", "", true, true, 1.0, 1, 5, -1, -10,{"9075.427734 280.188660 10718.725586"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "East", "", true, true, 1.0, 1, 5, -1, -10,{"9067.759766 280.948578 10719.856445"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "East", "", true, true, 1.0, 1, 5, -1, -10,{"9072.833008 280.211884 10703.137695"}));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"WoodenCrate"}, "9070.771484 280.396698 10713.153320"));
			containers.Insert(new ExpansionAIMissionContainer({"WoodenCrate"}, "9072.484375 280.127106 10716.075195"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -5, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 5:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Ambushed Military Convoy", "A military convoy was ambushed in the North of Elecktro","Ambush","#b53128");
			notifAction = new ExpansionSettingNotificationData("Ambushed Military Convoy", "PLACEHOLDER ACTION","Ambush","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Ambushed Military Convoy", "PLACEHOLDER END","Ambush","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Ambushed Military Convoy","Ambush","#b53128", "10671.889648 124.513763 4557.255859", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

			MappingFile = "AmbushedMilitaryConvoy.map";
		#ifdef EXPANSIONMODAI
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10677.689453 123.464012 4550.304199"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10673.118164 124.459389 4559.007813"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10674.083984 126.926834 4575.002441"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"10670.602539 123.757248 4554.224121"}));

			patrol = {"10665.684570 123.970688 4550.709473","10666.011719 128.632889 4577.446777","10689.669922 128.079041 4580.224609","10681.392578 123.235329 4544.676270","10661.429688 122.822968 4523.191406"};
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,patrol));
		#endif

			containers.Insert(new ExpansionAIMissionContainer({"WoodenCrate"}, "10671.889648 124.513763 4557.255859"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -5, ExpansionLootDefaults.Airdrop_Regular() ) );

			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(ExpansionAIMissionContainer({"Truck_01_Covered"}, "10677.7 123.279 4551.63", "-165.168 -8.42441 -0.49713"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		case 6:
			Weight = 10;
			notifStart 	= new ExpansionSettingNotificationData("Zombies Horde", "PLACEHOLDER START","Ambush","#b53128");
			notifAction = new ExpansionSettingNotificationData("Zombies Horde", "PLACEHOLDER ACTION","Ambush","#b53128");
			notifEnd 	= new ExpansionSettingNotificationData("Zombies Horde", "PLACEHOLDER END","Ambush","#b53128");
			mapMarker	= new ExpansionSettingMarkerData("Zombies Horde","Ambush","#b53128", "9072.484375 280.127106 10716.075195", true);
			MissionMeta = new ExpansionAIMissionMeta( notifStart, notifAction, notifEnd, mapMarker );

			Infected = {	
				"ZmbM_usSoldier_normal_Woodland",
				"ZmbM_SoldierNormal",
				"ZmbM_usSoldier_normal_Desert",
				"ZmbM_PatrolNormal_PautRev",
				"ZmbM_PatrolNormal_Autumn",
				"ZmbM_PatrolNormal_Flat",
				"ZmbM_PatrolNormal_Summer",
			};
			Animals.Insert(new ExpansionAIMissionInfected(Infected, "9072.484375 280.127106 10716.075195", 30, 10));

			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(ExpansionAIMissionContainer({"Offroad_02"}, "9072.484375 280.127106 10716.075195"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Military() ) );
			break;
		}

		MissionName = MissionMeta.NotificationStart.Title;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
	}

	// ------------------------------------------------------------
	// Expansion OnDefaultChernarusMission
	// ------------------------------------------------------------
	string OnDefaultNamalskMission(int idx)
	{
		return "null";
		/*
		array<vector> patrol = new array<vector>();
		array < ref ExpansionAIMissionContainer > containers = new array < ref ExpansionAIMissionContainer >;
		ExpansionAIMissionContainer container;
	
		switch ( idx )
		{
		default:
		case 0:
			Weight = 10;
			MissionMeta = new ExpansionAIMissionMeta( "Petrol Station", "A group of survivors setup camp on the station and are protecting their loot" );
			
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11882.159180 13.207184 7541.527832"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11886.900391 19.181400 7547.330078"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11893.499023 13.129795 7511.187988"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11904.500000 19.180000 7508.470215"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11909.193359 13.126508 7512.515137"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11914.932617 13.129795 7532.623047"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11906.269531 13.129794 7526.581055"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11896.461914 13.129795 7539.894531"}));
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11901.945313 18.176449 7555.589844"}));
			
			patrol.Insert("11899.374023 14.029161 7527.880859");
			patrol.Insert("11902.476563 17.037643 7522.748047");
			patrol.Insert("11913.799805 16.493601 7517.640137");
			patrol.Insert("11899.286133 20.082329 7502.711426");
			patrol.Insert("11890.314453 14.029161 7531.878418");
			AIPatrols.Insert( new ExpansionAIPatrol(-3, "WALK", "SPRINT", "ALTERNATE", "West", "", true, true, 1.0, 1, 5, -1, -10,{"11898.983398 13.129795 7550.353027", patrol, "PlayerSurvivorLoadout.json", "West", 3));

			container = new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue","WoodenCrate","ExpansionSafeLarge","ExpansionSafeMedium","ExpansionSafeSmall"}, "11905.581055 22.699667 7517.568359");
			containers.Insert(container);
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -20, ExpansionLootDefaults.Airdrop_Regular() ) );
			
			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue","WoodenCrate"}, "11891.248047 13.267243 7550.458984"));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -15, ExpansionLootDefaults.Airdrop_Regular() ) );

			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(new ExpansionAIMissionContainer({"ExpansionSafeLarge","ExpansionSafeMedium","ExpansionSafeSmall"}, "11891.200195 16.549999 7515.000000", "0 0 0", true));
			LootLocations.Insert( new ExpansionAIMissionLoot( containers, -10, ExpansionLootDefaults.Airdrop_Regular() ) );
			break;
		}

		MissionName = MissionMeta.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		*/
	}

	Object SpawnObject( string type, vector position = "0 0 0", vector orientation = "0 0 0", float lifetime = 0 )
	{
		Object obj;
		//GetGame().CreateObjectEx( type, position, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_AIRBORNE );
		obj = GetGame().CreateObject( type, position );
		
		if ( obj )
		{
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			vector roll = obj.GetOrientation();
			roll [ 2 ] = roll [ 2 ] - 1;
			obj.SetOrientation( roll );
			roll [ 2 ] = roll [ 2 ] + 1;
			obj.SetOrientation( roll );

			if (lifetime != 0)
			{
				EntityAI entity;
				if (Class.CastTo(entity, obj))
					entity.SetLifetimeMax(lifetime);
				else
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( GetGame().ObjectDelete, lifetime * 60 * 1000, false, obj );
			}
		}

		return obj;
	}
};
#endif
