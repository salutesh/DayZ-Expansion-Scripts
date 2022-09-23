#ifdef EXPANSIONMODAI
class ExpansionMissionEventAI extends ExpansionMissionEventBase
{
	ref ExpansionAIMissionMeta MissionMeta;

	string MappingFile;

	ref array< ref ExpansionSoldierLocation > SoldierLocation;
	ref array< ref ExpansionLootLocation > LootLocation;

	[NonSerialized()]
	ref ExpansionObjectSet MappingSet;

	[NonSerialized()]
	autoptr array< EntityAI > m_Containers;

	[NonSerialized()]
	autoptr array< eAIDynamicPatrol > m_Soldiers;

	#ifdef EXPANSIONMODNAVIGATION
	[NonSerialized()]
	ExpansionMarkerModule m_MarkerModule;

	[NonSerialized()]
	ExpansionMarkerData m_ServerMarker;
	#endif

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAI
	// ------------------------------------------------------------
	void ExpansionMissionEventAI()
	{
		m_EventName = "AI";

		SoldierLocation = new array< ref ExpansionSoldierLocation >;
		LootLocation = new array< ref ExpansionLootLocation >;

		m_Containers = new array< EntityAI >;
		m_Soldiers = new array< eAIDynamicPatrol >;
	}
	
	override void Event_OnStart()
	{
		MappingSet = new ExpansionObjectSet(EXPANSION_MISSIONS_OBJECTS_FOLDER,MappingFile);
		MappingSet.SpawnObjects();
		#ifdef EXPANSIONMODVEHICLE
		ExpansionCarKey key;
		#endif
		for ( int ll = 0; ll < LootLocation.Count(); ll++ ) 
		{
			ExpansionLootLocation lootLocation = LootLocation.Get( ll );
			if (lootLocation)
			{
				ExpansionAIMissionContainer containerData = lootLocation.Containers.GetRandomElement();
				bool canbelocked = containerData.CanBeLocked;
				EntityAI container = EntityAI.Cast( SpawnObject(containerData.Classnames.GetRandomElement(), containerData.Position, containerData.Orientation) );
				ItemBase itembs;
				CarScript car;
				ItemBase itemcargo;
				if (container.IsInherited(ItemBase))
				{
					itembs = ItemBase.Cast(container);
					itembs.Open();
				}
				else if (container.IsInherited(CarScript))
				{
					car = CarScript.Cast(container);
					car.OnDebugSpawn();

					for (int att_i = 0; att_i < car.GetInventory().AttachmentCount(); ++att_i)
					{
						EntityAI attachment = car.GetInventory().GetAttachmentFromIndex(att_i);
						attachment.SetHealth01("", "Health", Math.RandomFloat(0.65, 1.0));
					}

					car.LeakAll( CarFluid.FUEL );
					car.Fill( CarFluid.FUEL, Math.RandomFloat(0.0, car.GetFluidCapacity( CarFluid.FUEL ) ) );
				}

				ExpansionLootSpawner.SpawnLoot(container, lootLocation.Loot, lootLocation.ItemCount );

				if (itembs)
				{
					itembs.Close();

					#ifdef EXPANSIONMODBASEBUILDING
					if ( canbelocked )
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
				} else if (car)
				{
					if ( canbelocked )
					{
					#ifdef EXPANSIONMODVEHICLE
						key = ExpansionCarKey.Cast( SpawnObject("ExpansionCarKey") );
						car.PairKeyTo(key);
						car.LockCar(key);
					#endif
					}
				}
				m_Containers.Insert(container);
			}
		}

		int pickedGroupId = Math.RandomInt(0, SoldierLocation.Count() );

		for ( int sl = 0; sl < SoldierLocation.Count(); sl++ ) 
		{
			ref ExpansionSoldierLocation soldierLocation = SoldierLocation.Get( sl );
			if (soldierLocation)
			{
				m_Soldiers.Insert(CreateExpansionAI(soldierLocation.StartPosition, soldierLocation.Waypoints, soldierLocation.Loadout, soldierLocation.GroupSize, soldierLocation.Faction));
			
				#ifdef EXPANSIONMODVEHICLE
				if ( pickedGroupId == sl )
				{
					int soldiercount = m_Soldiers.Count() - 1;
					int pickedSoldierId = Math.RandomInt(0, m_Soldiers.Count() );
					eAIGroup group = eAIGroup.Cast(m_Soldiers[soldiercount].m_Group);
					DayZPlayerImplement member = DayZPlayerImplement.Cast(group.GetMember(pickedSoldierId));
					member.GetInventory().TakeEntityToCargo(InventoryMode.SERVER, key);
				}
				#endif
			}
		}

		#ifdef EXPANSIONMODNAVIGATION
		if (CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule))
			m_ServerMarker = m_MarkerModule.CreateServerMarker(MissionMeta.Name, EXPANSION_NOTIFICATION_ICON_AI_MISSION, SoldierLocation[0].StartPosition, COLOR_EXPANSION_NOTIFICATION_MISSION, true);
		#endif
		
		if ( GetExpansionSettings().GetNotification().ShowAIMissionEnded )
			CreateNotification(new StringLocaliser( "STR_EXPANSION_MISSION_AI_ENDED", MissionMeta.Name), EXPANSION_NOTIFICATION_ICON_AI_MISSION, 7);

	}

	override void Event_OnEnd()
	{
		if ( IsMissionHost() )
		{
			//! After mission ends check all 60 seconds if a player is nearby the airdrop crate and if not delete the container
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CleanupCheck, 60 * 1000, true );
		}
	}
	
	void CleanupCheck()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAI::CleanupCheck - Start");
		#endif
		
		if ( IsMissionHost() )
		{			
			//! Check if a player is nearby the container in a 1000 meter radius
			for ( int j = 0; j < m_Containers.Count(); j++ )
			{
				if ( !ExpansionLootSpawner.IsPlayerNearby(m_Containers[j], 1000) )
				{
					ExpansionLootSpawner.RemoveContainer( m_Containers[j] );
					m_Containers.Remove(j);
				}
			}

			if ( m_Containers.Count() == 0 )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CleanupCheck );
				for ( int i = 0; i < m_Soldiers.Count(); i++ )
				{
					m_Soldiers[i].Despawn();
					m_Soldiers[i].Delete();
					m_Soldiers.Remove(i);
				}

				#ifdef EXPANSIONMODNAVIGATION
				if (m_ServerMarker && m_MarkerModule)
					m_MarkerModule.RemoveServerMarker(m_ServerMarker.GetUID());
				#endif

				if ( GetExpansionSettings().GetNotification().ShowAIMissionEnded )
					CreateNotification(new StringLocaliser( "STR_EXPANSION_MISSION_AI_ENDED", MissionMeta.Name), EXPANSION_NOTIFICATION_ICON_AI_MISSION, 7);

				MappingSet.Delete();
				return;
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAI::CleanupCheck - End");
		#endif
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
				return 1; //! amount of locations
			break;
			case "namalsk":
				return 1;
			break;
			/*
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
			case "expansiontest":
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
			case "namalsk":
				return OnDefaultNamalskMission(index);
			break;
		}

		return "Error_You_Shouldnt_Be_Seeing_This";
	}

	// ------------------------------------------------------------
	// Expansion OnDefaultChernarusMission
	// ------------------------------------------------------------
	string OnDefaultChernarusMission(int idx)
	{
		vector start_position;
		array<vector> patrol = new array<vector>();
		array < ref ExpansionAIMissionContainer > containers = new array < ref ExpansionAIMissionContainer >;
		ExpansionAIMissionContainer container;
	
		switch ( idx )
		{
		default:
		case 0:
			Weight = 10;
			MissionMeta = new ExpansionAIMissionMeta( "Bandit Ambush", "Bandits prepared a roadblock to ambush cars going toward Zaprudnoe !", "" );

			MappingFile = "BanditRoadAmbush";
			SoldierLocation.Insert( new ExpansionSoldierLocation("5458.080566 173.637192 12304.130859", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("5463.901855 173.594833 12302.534180", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("5462.813965 173.794785 12292.244141", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("5466.251953 185.817581 12296.766602", patrol, "PlayerSurvivorLoadout.json", "West"));

			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue"}, "5457.122070 173.275223 12304.987305"));
			LootLocation.Insert( new ExpansionLootLocation( containers, -20, DefaultRegular() ) );
			break;/*
		case 1:
			Weight = 10;
			MissionMeta = new ExpansionAIMissionMeta( "Helicrash Cargo", "A helicopter carrying crates crashed nearby Dubrovka !" );

			containers.Insert("WoodenCrate");
			MappingFile = "HelicrashCargo";
			SoldierLocation.Insert( new ExpansionSoldierLocation("9075.427734 280.188660 10718.725586", patrol, "PlayerSurvivorLoadout.json", "East"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("9067.759766 280.948578 10719.856445", patrol, "PlayerSurvivorLoadout.json", "East"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("9072.833008 280.211884 10703.137695", patrol, "PlayerSurvivorLoadout.json", "East"));

			LootLocation.Insert( new ExpansionLootLocation( containers, "9070.771484 280.396698 10713.153320", -5, DefaultRegular() ) );
			LootLocation.Insert( new ExpansionLootLocation( containers, "9072.484375 280.127106 10716.075195", -5, DefaultRegular() ) );
			break;
		case 2:
			Weight = 10;
			MissionMeta = new ExpansionAIMissionMeta( "Ambushed Military Convoy", "A military convoy was ambushed in the North of Elecktro" );

			containers.Insert("WoodenCrate");
			MappingFile = "AmbushedMilitaryConvoy";
			SoldierLocation.Insert( new ExpansionSoldierLocation("10677.689453 123.464012 4550.304199", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("10673.118164 124.459389 4559.007813", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("10674.083984 126.926834 4575.002441", patrol, "PlayerSurvivorLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation("10670.602539 123.757248 4554.224121", patrol, "PlayerSurvivorLoadout.json", "West"));

			patrol = {"10665.684570 123.970688 4550.709473","10666.011719 128.632889 4577.446777","10689.669922 128.079041 4580.224609","10681.392578 123.235329 4544.676270","10661.429688 122.822968 4523.191406"};
			SoldierLocation.Insert( new ExpansionSoldierLocation("910669.708008 122.418236 4539.055664", patrol, "PlayerSurvivorLoadout.json", "West"));

			LootLocation.Insert( new ExpansionLootLocation( containers, "10671.889648 124.513763 4557.255859", -5, DefaultRegular() ) );
			containers = new TStringArray;
			containers.Insert("Truck_01_Covered");
			LootLocation.Insert( new ExpansionLootLocation( containers, "10677.373047 125.411873 4559.961426", -20, DefaultRegular() ) );
			break;*/
		}

		MissionName = MissionMeta.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
	}

	// ------------------------------------------------------------
	// Expansion OnDefaultChernarusMission
	// ------------------------------------------------------------
	string OnDefaultNamalskMission(int idx)
	{
		vector start_position;
		array<vector> patrol = new array<vector>();
		array < ref ExpansionAIMissionContainer > containers = new array < ref ExpansionAIMissionContainer >;
		ExpansionAIMissionContainer container;
	
		switch ( idx )
		{
		default:
		case 0:
			Weight = 10;
			MissionMeta = new ExpansionAIMissionMeta( "Petrol Station", "A group of survivors setup camp on the station and are protecting their loot" );
			
			SoldierLocation.Insert( new ExpansionSoldierLocation("11882.159180 13.207184 7541.527832", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11886.900391 19.181400 7547.330078", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11893.499023 13.129795 7511.187988", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11904.500000 19.180000 7508.470215", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11909.193359 13.126508 7512.515137", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11914.932617 13.129795 7532.623047", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11906.269531 13.129794 7526.581055", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11896.461914 13.129795 7539.894531", patrol, "PlayerSurvivorLoadout.json", "West"));
			SoldierLocation.Insert( new ExpansionSoldierLocation("11901.945313 18.176449 7555.589844", patrol, "PlayerSurvivorLoadout.json", "West"));
			
			patrol.Insert("11899.374023 14.029161 7527.880859");
			patrol.Insert("11902.476563 17.037643 7522.748047");
			patrol.Insert("11913.799805 16.493601 7517.640137");
			patrol.Insert("11899.286133 20.082329 7502.711426");
			patrol.Insert("11890.314453 14.029161 7531.878418");
			SoldierLocation.Insert( new ExpansionSoldierLocation("11898.983398 13.129795 7550.353027", patrol, "PlayerSurvivorLoadout.json", "West", 3));

			container = new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue","WoodenCrate","ExpansionSafeLarge","ExpansionSafeMedium","ExpansionSafeSmall"}, "11905.581055 22.699667 7517.568359");
			containers.Insert(container);
			LootLocation.Insert( new ExpansionLootLocation( containers, -20, DefaultRegular() ) );
			
			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(new ExpansionAIMissionContainer({"Barrel_Red","Barrel_Yellow","Barrel_Green","Barrel_Blue","WoodenCrate"}, "11891.248047 13.267243 7550.458984"));
			LootLocation.Insert( new ExpansionLootLocation( containers, -15, DefaultRegular() ) );

			containers = new array < ref ExpansionAIMissionContainer >;
			containers.Insert(new ExpansionAIMissionContainer({"ExpansionSafeLarge","ExpansionSafeMedium","ExpansionSafeSmall"}, "11891.200195 16.549999 7515.000000", "0 0 0", true));
			LootLocation.Insert( new ExpansionLootLocation( containers, -10, DefaultRegular() ) );
			break;
		}

		MissionName = MissionMeta.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
	}
	
	array < ref ExpansionAirdropLoot > DefaultRegular()
	{
		array < ref ExpansionAirdropLoot > Loot;
		
		TStringArray ak74u_1 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_WoodBttstck", "AK_WoodHndgrd" };

		TStringArray sks_1 = { "PUScopeOptic" };
		TStringArray sks_2 = { "SKS_Bayonet" };

		array< ref ExpansionAirdropLootVariant > sksVariants = {
			new ExpansionAirdropLootVariant( "SKS", sks_2, 0.2 ),
			new ExpansionAirdropLootVariant( "SKS", NULL, 0.6 ),
		};

		TStringArray ump_1 = { "UniversalLight" };

		array< ref ExpansionAirdropLootVariant > umpVariants = {
			new ExpansionAirdropLootVariant( "UMP45", NULL, 0.6 ),
		};

		TStringArray mosin_1 = { "PUScopeOptic" };

		array< ref ExpansionAirdropLootVariant > mosinVariants = {
			new ExpansionAirdropLootVariant( "Mosin9130", NULL, 0.75 ),
		};

		TStringArray b95_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > b95Variants = {
			new ExpansionAirdropLootVariant( "B95", NULL, 0.666667 ),
		};

		TStringArray cz527_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > cz527Variants = {
			new ExpansionAirdropLootVariant( "CZ527", NULL, 0.4 ),
		};

		TStringArray cz75_1 = { "TLRLight" };

		array< ref ExpansionAirdropLootVariant > cz75Variants = {
			new ExpansionAirdropLootVariant( "CZ75", NULL, 0.5 ),
		};

		TStringArray fnx1_1 = { "TLRLight" };

		array< ref ExpansionAirdropLootVariant > fnxVariants = {
			new ExpansionAirdropLootVariant( "FNX45", NULL, 0.75 ),
		};
		
		TStringArray kedr_1 = { "PistolSuppressor" };  

		array< ref ExpansionAirdropLootVariant > kedrVariants = {
			new ExpansionAirdropLootVariant( "Expansion_Kedr", NULL, 0.6 ),
		};

		TStringArray winchester70_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > winchester70Variants = {
			new ExpansionAirdropLootVariant( "Winchester70", NULL, 0.8 ),
		};

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };
		TStringArray visor = { "DirtBikeHelmet_Visor" };
		
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","SalineBagIV" };
		TStringArray firstaidkit_3 = { "CharcoalTablets","Morphine" };
		TStringArray firstaidkit_4 = { "Epinephrine","StartKitIV" };
		TStringArray firstaidkit_5 = { "Morphine","SalineBagIV" };
		TStringArray firstaidkit_6 = { "PainkillerTablets","Epinephrine" };
		TStringArray firstaidkit_7 = { "TetracyclineAntiBiotics","Morphine" };
		TStringArray firstaidkit_8 = { "BandageDressing","VitaminBottle" };

		array< ref ExpansionAirdropLootVariant > firstaidkitVariants = {
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_2, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_3, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_4, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_5, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_6, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_7, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_8, 0.131579 ),
		};

		Loot = {
			new ExpansionAirdropLoot( "AKS74U", ak74u_1, 0.08 ),
			
			new ExpansionAirdropLoot( "AKM", akm_1, 0.05 ),

			new ExpansionAirdropLoot( "SKS", sks_1, 0.5, -1, sksVariants ),

			new ExpansionAirdropLoot( "UMP45", ump_1, 0.25, -1, umpVariants ),
 
			new ExpansionAirdropLoot( "Mosin9130", mosin_1, 0.4, -1, mosinVariants ),
 
			new ExpansionAirdropLoot( "B95", b95_1, 0.3, -1, b95Variants ),

			new ExpansionAirdropLoot( "CZ527", cz527_1, 0.5, -1, cz527Variants ),

			new ExpansionAirdropLoot( "CZ75", cz75_1, 0.2, -1, cz75Variants ),

			new ExpansionAirdropLoot( "FNX45", fnx1_1, 0.4, -1, fnxVariants ),

			new ExpansionAirdropLoot( "Expansion_Kedr", kedr_1, 0.5, -1, kedrVariants ),

   			new ExpansionAirdropLoot( "Mp133Shotgun", NULL, 0.8 ),

			new ExpansionAirdropLoot( "Winchester70", winchester70_1, 0.5, -1, winchester70Variants ),
				
			new ExpansionAirdropLoot( "Expansion_DT11", NULL, 0.3 ),
			
			new ExpansionAirdropLoot( "Binoculars", NULL, 0.3 ),
			
			#ifdef NAMALSK_SURVIVAL
			new ExpansionAirdropLoot( "dzn_map_namalsk", NULL, 0.3 ),
			#else
			new ExpansionAirdropLoot( "ChernarusMap", NULL, 0.3 ),
			#endif
			new ExpansionAirdropLoot( "Rangefinder", battery, 0.05 ),
			new ExpansionAirdropLoot( "ExpansionGPS", NULL, 0.05 ),
			
			new ExpansionAirdropLoot( "BoxCerealCrunchin", NULL, 0.05 ),
			new ExpansionAirdropLoot( "PeachesCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "BakedBeansCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "SpaghettiCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "SardinesCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "TunaCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "WaterBottle", NULL, 0.5 ),
			
			new ExpansionAirdropLoot( "CanOpener", NULL, 0.5 ),
			new ExpansionAirdropLoot( "KitchenKnife", NULL, 0.3 ),
			
			new ExpansionAirdropLoot( "BallisticHelmet_UN", NULL, 0.08 ),
			new ExpansionAirdropLoot( "DirtBikeHelmet_Chernarus", visor, 0.3 ),
			
			new ExpansionAirdropLoot( "SewingKit", NULL, 0.25 ),
			new ExpansionAirdropLoot( "LeatherSewingKit", NULL, 0.25 ),
			new ExpansionAirdropLoot( "WeaponCleaningKit", NULL, 0.05 ),
			new ExpansionAirdropLoot( "Lockpick", NULL, 0.05 ),
			
			new ExpansionAirdropLoot( "GhillieAtt_Mossy", NULL, 0.05 ),

			new ExpansionAirdropLoot( "Mag_Expansion_Kedr_20Rnd", NULL, 0.8 ),
			new ExpansionAirdropLoot( "Mag_CZ527_5rnd", NULL, 0.9 ),
			new ExpansionAirdropLoot( "Mag_CZ75_15Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "Mag_FNX45_15Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "Mag_UMP_25Rnd", NULL, 0.5 ),

			new ExpansionAirdropLoot( "AmmoBox_9x39_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_9x19_25Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_762x39Tracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_762x39_20Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_45ACP_25Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_308WinTracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_308Win_20Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaRubberSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaPellets_10Rnd", NULL, 1 ),
			
			new ExpansionAirdropLoot( "Ammo_9x39", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_762x39Tracer", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_762x39", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_45ACP", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_308WinTracer", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_308Win", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaSlug", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaRubberSlug", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaPellets", NULL,  0.5 ),
			
			new ExpansionAirdropLoot( "FirstAidKit", firstaidkit_1, 0.38, -1, firstaidkitVariants ),
		};

		return Loot;
	}

	eAIDynamicPatrol CreateExpansionAI(vector startpos, array<vector> waypoints, string loadout = "", int groupsize = -1, string faction = "")
	{
		eAIFaction efaction;
		eAIWaypointBehavior waypointbehavior = eAIWaypointBehavior.ALTERNATE;
		switch (faction)
		{
			case "West":
			{
				efaction = new eAIFactionWest();
				break;
			}
			case "East":
			{
				efaction = new eAIFactionEast();
				break;
			}
			case "Raiders":
			{
				efaction = new eAIFactionRaiders();
				break;
			}
			case "Civilian":
			{
				efaction = new eAIFactionCivilian();
				break;
			}
			default:
			{
				efaction = new eAIFactionCivilian();
				break;
			}
		}

		if ( groupsize < 0 )
			groupsize = Math.RandomInt(1,-groupsize);

		if ( waypoints.Count() <= 1 )
			waypointbehavior = eAIWaypointBehavior.HALT;

		return eAIDynamicPatrol.Create(startpos, waypoints, waypointbehavior, loadout, groupsize, -1, efaction, true, 10, 800, 2);
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
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( GetGame().ObjectDelete, lifetime * 60 * 1000, false, obj );
			}
		}

		return obj;
	}
}
#endif