class CommunityOfflineClient : MissionGameplay
{
	ref array<Object> m_CustomObjects = new array<Object>;
	
	void CommunityOfflineClient()
	{
		NewCOMModuleManager();
	}

	void ~CommunityOfflineClient()
	{
		Print("CommunityOfflineClient::~CommunityOfflineClient()");
	}

	ref array< Object > debugObjs = new array< Object >;

	void CreateDebugObject( string type, vector trans[4] )
	{
		Object dbg = GetGame().CreateObjectEx( "ExpansionDebugBox_Red", "0 0 0", ECE_CREATEPHYSICS );
		dbg.SetTransform( trans );
		debugObjs.Insert( dbg );
	}

	void GetHeadingTransform( Object obj, out vector trans[4] )
	{
		float heading = obj.GetOrientation()[0] * Math.DEG2RAD;

		trans[1] = "0 1 0";
		trans[2] = Vector( Math.Sin( heading ), 0, Math.Cos( heading ) );
		trans[0] = -(trans[2] * trans[1]);

		trans[3] = obj.GetPosition();
	}

	void GetBoundingPositionTransform( Object obj, vector position, vector wsTrans[4], out vector trans[4] )
	{
		vector bbTrans[4];
		Math3D.YawPitchRollMatrix( "0 0 0", bbTrans );
		bbTrans[3] = position;

		Math3D.MatrixMultiply4( wsTrans, bbTrans, trans );

		vector contact_pos;
		vector contact_dir
		vector hitNormal;
		int contactComponent;
		float hitFraction;
		Object hitObject;
		
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER;
		DayZPhysics.RayCastBullet( trans[3] + "0 10 0", trans[3] - "0 10 0", collisionLayerMask, obj, hitObject, contact_pos, hitNormal, hitFraction );
		trans[3] = contact_pos;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		GetCOMModuleManager().OnUpdate(timeslice);

		/*
		Object obj = CarScript.GetAll()[0];
		if ( obj )
		{
			for ( int j = 0; j < debugObjs.Count(); j++ )
			{
				GetGame().ObjectDelete( debugObjs[j] );
			}

			debugObjs.Clear();

			vector boundingBox[2];
			obj.GetCollisionBox( boundingBox );

			vector headingTransform[4];
			GetHeadingTransform( obj, headingTransform );

			float l = boundingBox[0][0];
			float r = boundingBox[1][0];
			float d = boundingBox[0][1];
			float u = boundingBox[1][1];
			float b = boundingBox[0][2];
			float f = boundingBox[1][2];

			Object dbg = NULL;

			vector bbTransFL[4];
			GetBoundingPositionTransform( obj, Vector( l, d, f ), headingTransform, bbTransFL );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransFL );

			vector bbTransFR[4];
			GetBoundingPositionTransform( obj, Vector( r, d, f ), headingTransform, bbTransFR );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransFR );

			vector bbTransBL[4];
			GetBoundingPositionTransform( obj, Vector( l, d, b ), headingTransform, bbTransBL );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransBL );

			vector bbTransBR[4];
			GetBoundingPositionTransform( obj, Vector( r, d, b ), headingTransform, bbTransBR );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransBR );
		}
		*/
	}

	override void OnKeyPress(int key)
	{
		if (key == KeyCode.KC_K)
		{
		}

		if (key == KeyCode.KC_HOME)
		{
			Object parentHouse = SpawnObjectCustom("Land_House_2W03", "0.000000 100.000000 0.000000", "0.000000 0.000000 0.000000", false);

			m_CustomObjects.Clear();

			SpawnObjectCustom("bldr_plnt_ivy_N", "9.795703 102.300140 -1.724508", "0.000000 -87.000038 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_T", "9.088506 101.550095 -1.624508", "-90.000015 0.000000 -90.000038");
			SpawnObjectCustom("bldr_plnt_ivy_dry_J", "8.145607 99.099945 -1.524508", "-178.999924 0.000000 -23.000006");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "8.783590 94.899933 -1.524508", "-179.999985 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_O", "8.518077 96.099762 -1.574508", "91.999969 -89.000046 89.999992");
			SpawnObjectCustom("bldr_plnt_ivy_I", "3.332874 94.749680 -1.624500", "179.999985 -89.000046 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_dry_B", "3.363040 95.199707 -1.524508", "179.999985 0.000000 -68.999992");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "3.517540 94.649673 -1.524508", "-179.999985 0.000000 -71.000038");
			SpawnObjectCustom("bldr_plnt_ivy_special_A", "1.127489 99.149948 -1.424819", "90.000038 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_M", "-1.204145 102.150131 -1.674819", "-0.000005 -87.000038 -0.000005");
			SpawnObjectCustom("bldr_plnt_ivy_R", "1.587622 101.200073 -1.624508", "1.999994 -89.000046 -0.000005");
			SpawnObjectCustom("bldr_plnt_ivy_dry_G", "1.928850 99.749985 -1.524508", "0.000000 0.000000 91.000038");
			SpawnObjectCustom("bldr_plnt_ivy_dry_F", "1.997854 99.299957 -1.520000", "0.000000 0.000000 85.000008");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "1.302991 99.149948 -1.524819", "-179.999985 0.000000 81.999992");
			SpawnObjectCustom("bldr_plnt_ivy_roof_A", "-2.522586 102.550156 -2.112922", "-0.000009 -89.000046 -0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_B", "0.046462 97.330177 -2.351164", "-80.000008 19.000000 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_N", "-9.843571 94.649673 -1.674509", "-0.000005 87.000038 -179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_special_A", "-8.209624 101.350082 -1.474819", "90.000000 10.000000 2.000000");
			SpawnObjectCustom("bldr_plnt_ivy_L", "-5.444373 101.900116 -1.624818", "-0.000005 -89.000046 -0.000005");
			SpawnObjectCustom("bldr_plnt_ivy_M", "0.313056 106.050369 2.703959", "179.999985 -50.000015 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_I", "-3.856619 106.049042 2.718060", "-57.999981 21.999994 149.000076");
			SpawnObjectCustom("bldr_plnt_ivy_L", "4.453686 105.399963 1.863604", "-130.000031 -26.999983 149.999985");
			SpawnObjectCustom("bldr_plnt_ivy_R", "5.098592 104.355782 0.495181", "-97.000137 -6.000000 146.000000");
			SpawnObjectCustom("bldr_plnt_ivy_dry_G", "-9.392963 97.449844 -1.510000", "0.024030 0.000000 6.000008");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "-9.229526 94.999695 -1.500000", "-179.999985 0.000000 -6.000001");
			SpawnObjectCustom("bldr_plnt_ivy_H", "-4.918296 98.130920 -1.600403", "-90.000038 41.999996 90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "-6.095337 93.549606 -1.500000", "-179.999985 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_N", "10.074900 94.199646 -1.514679", "-89.999992 88.000015 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_M", "10.024900 102.050125 1.754317", "-90.000015 -82.999992 -0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_R", "9.974899 98.499908 2.436103", "179.999985 0.000000 90.000038");
			SpawnObjectCustom("bldr_plnt_ivy_H", "10.024899 95.799744 6.014517", "179.999985 -21.999985 89.999969");
			SpawnObjectCustom("bldr_plnt_ivy_M", "9.974899 94.649673 2.969738", "-89.999969 86.000015 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_G", "10.074898 98.549911 4.006066", "0.000000 0.000000 90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "9.940000 94.049637 -0.680727", "-89.999969 -0.200000 5.000001");
			SpawnObjectCustom("bldr_plnt_ivy_dry_B", "9.910000 99.099945 1.689043", "90.000038 0.000000 23.000013");
			SpawnObjectCustom("bldr_plnt_ivy_H", "5.391165 105.209961 3.048910", "0.000000 -1.000000 -148.938400");
			SpawnObjectCustom("bldr_plnt_ivy_M", "5.901872 104.825554 4.534941", "-52.000008 -34.999992 -153.000031");
			SpawnObjectCustom("bldr_plnt_ivy_M", "0.954117 106.061150 3.084268", "0.000000 -49.000008 -179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_H", "-4.964212 104.776634 4.773229", "25.000116 -37.000008 166.000015");
			SpawnObjectCustom("bldr_plnt_ivy_K", "-2.570354 106.069839 3.129530", "0.000000 -39.999985 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_G", "-6.322302 104.677834 3.045574", "17.000011 15.000002 -26.774605");
			SpawnObjectCustom("bldr_plnt_ivy_N", "-9.965102 102.300140 -1.498841", "-90.000122 -89.000046 179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_N", "-9.965102 94.399658 -1.459938", "2.000000 0.000000 90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_N", "-10.015102 98.449905 1.361202", "90.000000 -89.000046 -0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_R", "-9.965102 98.449905 3.414384", "0.000000 0.000000 93.000031");
			SpawnObjectCustom("bldr_plnt_ivy_special_A", "-9.815101 101.300079 2.120183", "-179.999985 10.000000 2.000000");
			SpawnObjectCustom("bldr_plnt_ivy_K", "-9.965101 102.250137 5.537392", "89.999954 -83.000015 -0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_F", "-9.915102 95.749741 5.862661", "179.999985 11.000000 -89.999954");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "-9.815102 97.299835 6.556256", "-90.000000 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "-9.815102 94.549973 5.816029", "-90.000000 0.000000 -7.000006");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "-9.815102 98.649918 3.921703", "90.000000 0.000000 89.000046");
			SpawnObjectCustom("bldr_plnt_ivy_N", "-9.838178 102.250137 7.475183", "1.000000 -89.000046 179.999954");
			SpawnObjectCustom("bldr_plnt_ivy_M", "-2.696145 102.100128 7.419182", "179.999985 -89.000046 -0.000036");
			SpawnObjectCustom("bldr_plnt_ivy_special_A", "-2.296325 99.199951 7.219182", "-90.000000 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_special_B", "-5.582883 100.200012 7.369182", "-90.000000 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_special_B", "-1.385958 97.299835 7.225182", "-90.000015 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_H", "1.108575 101.312096 7.459309", "-89.999969 -61.000015 -89.999992");
			SpawnObjectCustom("bldr_plnt_ivy_M", "2.161586 98.249893 7.469100", "179.999985 -89.000046 0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_R", "3.989918 98.599915 7.375182", "90.000000 0.000000 90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_F", "9.227154 100.550034 7.425182", "90.000000 0.000000 90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_N", "9.889800 94.549667 7.375182", "-1.000000 89.000046 -0.000009");
			SpawnObjectCustom("bldr_plnt_ivy_T", "-1.331300 100.250015 -1.874819", "0.000000 -37.999996 -90.000000");
			SpawnObjectCustom("bldr_plnt_ivy_dry_C", "9.198196 94.849686 7.299000", "0.000000 0.000000 0.000000");
			SpawnObjectCustom("bldr_plnt_ivy_I", "-4.389064 94.549667 7.375181", "-179.999985 -89.000046 -0.000005");
			SpawnObjectCustom("bldr_plnt_ivy_dry_G", "-9.257981 100.900055 7.369182", "-175.999969 0.000000 -179.999985");
			SpawnObjectCustom("bldr_plnt_ivy_dry_I", "6.174156 95.849747 7.325182", "0.000000 0.000000 -19.000006");
			SpawnObjectCustom("bldr_plnt_ivy_dry_H", "1.483491 96.800110 7.320000", "0.000000 0.000000 24.000019");
			SpawnObjectCustom("bldr_plnt_ivy_dry_D", "-1.482187 98.299896 7.319181", "0.000000 0.000000 89.000038");
			SpawnObjectCustom("bldr_plnt_ivy_dry_B", "-8.966343 94.399658 7.325182", "0.000000 0.000000 0.000000");

			string export = "class " + parentHouse.GetType() + " extends BuildingWithInterior\n{\n\toverride void SpawnInterior()\n\t{";
			for (int i = 0; i < m_CustomObjects.Count(); ++i)
			{
				Object m_object = m_CustomObjects.Get(i);
				if (ItemBase.Cast(m_object))
				{
					export = export + "\n\t\tSpawnInteriorObject( \"" + m_object.GetType() + "\", \"" + ObjectEditor.VectorToStringCOM(parentHouse.WorldToModel(m_object.GetPosition())) + "\", \"" + ObjectEditor.VectorToStringCOM(m_object.GetOrientation()) + "\", false );";
				}
				else
				{
					export = export + "\n\t\tSpawnInteriorObject( \"" + m_object.GetType() + "\", \"" + ObjectEditor.VectorToStringCOM(parentHouse.WorldToModel(m_object.GetPosition())) + "\", \"" + ObjectEditor.VectorToStringCOM(m_object.GetOrientation()) + "\" );";
				}
			}

			export = export + "\n\t}\n}";

			GetGame().CopyToClipboard(export);
		}

		super.OnKeyPress(key);
	}

	override void OfflineMissionStart()
	{
		// COTIsActive = true;

		vector pPos = "0 0 0";

		//! Helicopter testing
		pPos = "3993.01 0 10146.2";

		PlayerBase player;
		Class.CastTo( player, GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), pPos, 0, "NONE" ) );
		GetGame().SelectPlayer( NULL, player );

		if (player)
		{
			EntityAI item = NULL;

			item = player.GetInventory().CreateInInventory("AviatorGlasses");
			item = player.GetInventory().CreateInInventory("MilitaryBeret_UN");
			item = player.GetInventory().CreateInInventory("M65Jacket_Black");
			item = player.GetInventory().CreateInInventory("TacticalGloves_Black");
			item = player.GetInventory().CreateInInventory("HunterPants_Autumn");
			item = player.GetInventory().CreateInInventory("MilitaryBoots_Black");
			
			item = player.GetInventory().CreateInInventory("ExpansionParachuteBag");
			item.GetInventory().CreateAttachment("ExpansionParachute");
			
			item = player.GetInventory().CreateInInventory("Shovel");

			player.COTSetGodMode(true);
		}

		pPos[1] = GetGame().SurfaceY(pPos[0], pPos[2]);
		GetGame().GetPlayer().SetPosition(pPos);

		SpawnItem( GetPlayer(), "ExpansionAdminHammer" );
		SpawnItem( GetPlayer(), "Crowbar" );

		SpawnItem( GetPlayer(), "ExpansionStairKit" );
		SpawnItem( GetPlayer(), "ExpansionStairKit" );
		SpawnItem( GetPlayer(), "ExpansionStairKit" );

		SpawnItem( GetPlayer(), "ExpansionFloorKit" );
		SpawnItem( GetPlayer(), "ExpansionFloorKit" );
		SpawnItem( GetPlayer(), "ExpansionFloorKit" );

		SpawnItem( GetPlayer(), "ExpansionWallKit" );
		SpawnItem( GetPlayer(), "ExpansionWallKit" );
		SpawnItem( GetPlayer(), "ExpansionWallKit" );

		SpawnItem( GetPlayer(), "ExpansionRampKit" );
		SpawnItem( GetPlayer(), "ExpansionRampKit" );
		SpawnItem( GetPlayer(), "ExpansionRampKit" );
	}

	override void OnInit()
	{
		ExpansionMissionModule missionModule;
		if (Class.CastTo(missionModule, GetModuleManager().GetModule(ExpansionMissionModule)))
		{
			missionModule.SetMissionConstructor(COMMissionConstructor);
		}

		super.OnInit();

		SetupWeather();

		SetupDateAndTime();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		GetCOMModuleManager().OnInit();
		GetCOMModuleManager().OnMissionStart();

		GetGame().GetUIManager().CloseMenu(MENU_INGAME);
	}

	override void OnMissionFinish()
	{
		GetCOMModuleManager().OnMissionFinish();

		CloseAllMenus();

		DestroyAllMenus();

		GetGame().GetUIManager().CloseMenu(MENU_INGAME);

		super.OnMissionFinish();
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		GetCOMModuleManager().OnMissionLoaded();
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);

		if (key == KeyCode.KC_PERIOD)
		{
			if (GetUIManager().IsMenuOpen(MENU_GESTURES))
			{
				GesturesMenu.CloseMenu();
			}
		}

		/*
		if (key == KeyCode.KC_K)
		{
			Print( "START ITEMS" );

			TStringArray configs = new TStringArray;
			configs.Insert( CFG_VEHICLESPATH );
			configs.Insert( CFG_WEAPONSPATH );
			configs.Insert( CFG_MAGAZINESPATH );

			for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
			{
				string strConfigPath = configs.Get( nConfig );

				int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

				for ( int nClass = 0; nClass < nClasses; ++nClass )
				{
					string strName;

					g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

					int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

					if ( scope != 2 )
					{
						continue;
					}

					if ( strName == "Mag_Scout_5Rnd") continue; // fix crash for this dumb item. dont spawn it

					if ( strName.Contains("Fx") )continue; // fix crash for this dumb item. dont spawn it

					if ( strName == "ItemOptics" ) 
					{
						continue; // Fix crash
					}

					if ( strName.Contains("Fx") ) 
					{
					//	continue; // Fix crash
					}

					Print( "" + strName );
				}
			}

			Print( "END ITEMS" );
		}
		*/
	}

	static void SetupWeather()
	{
		Weather weather = g_Game.GetWeather();

		weather.GetOvercast().SetLimits(0.0, 2.0);
		weather.GetRain().SetLimits(0.0, 2.0);
		weather.GetFog().SetLimits(0.0, 2.0);

		weather.GetOvercast().SetForecastChangeLimits(0.0, 0.0);
		weather.GetRain().SetForecastChangeLimits(0.0, 0.0);
		weather.GetFog().SetForecastChangeLimits(0.0, 0.0);

		weather.GetOvercast().SetForecastTimeLimits(1800, 1800);
		weather.GetRain().SetForecastTimeLimits(600, 600);
		weather.GetFog().SetForecastTimeLimits(600, 600);

		weather.GetOvercast().Set(0.0, 0, 0);
		weather.GetRain().Set(0.0, 0, 0);
		weather.GetFog().Set(0.0, 0, 0);

		weather.SetWindMaximumSpeed( 20 );
		weather.SetWindFunctionParams( 0.1, 1.0, 20 );
		weather.SetWindSpeed( 0.1 );
	}

	static void SetupDateAndTime()
	{
		int year, month, day, hour, minute;
		int reset_month = 9, reset_day = 20;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		if ((month == reset_month) && (day < reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month == reset_month + 1) && (day > reset_day))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
			else
			{
				if ((month < reset_month) || (month > reset_month + 1))
				{
					GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
				}
			}
		}
	}

	Object SpawnObjectCustom(string type, vector position, vector orientation, bool insertInArray = true)
	{
		Object obj = GetGame().CreateObject(type, position);
		if (obj)
		{
			obj.SetPosition(position);
			obj.SetOrientation(orientation);
			vector roll = obj.GetOrientation();
			roll[2] = roll[2] - 1;
			obj.SetOrientation(roll);
			roll[2] = roll[2] + 1;
			obj.SetOrientation(roll);

			if (insertInArray)
				m_CustomObjects.Insert(obj);
		}

		return obj;
	}

	void RemoveAttachment(PlayerBase player, string slot_name)
	{
		int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
		if (slot_id == InventorySlots.INVALID)
			return;

		Object oitm = player.GetInventory().FindAttachment(slot_id);
		if (oitm)
		{
			GetGame().ObjectDelete(oitm);
		}
	}

	void SpawnItem(PlayerBase player, string item)
	{
		Object oitm = player.GetInventory().CreateInInventory(item);
		if (!oitm)
		{
			vector position = player.GetPosition();
			oitm = GetGame().CreateObject(item, position);

			if (oitm)
			{
				position[0] = position[0] + Math.RandomFloatInclusive(-5, 5);
				position[2] = position[2] + Math.RandomFloatInclusive(-5, 5);
				position[1] = GetGame().SurfaceY(position[0], position[2]);
				oitm.SetPosition(position);
			}
		}
	}

	void SpawnKit(string kit)
	{
		SpawnItem(GetGame().GetPlayer(), kit);
		SpawnItem(GetGame().GetPlayer(), "Nail");
		SpawnItem(GetGame().GetPlayer(), "WoodenPlank");
	}
}